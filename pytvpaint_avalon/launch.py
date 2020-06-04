#! /usr/bin/env python3
""""""
import subprocess
import importlib
import importlib.util
import sys
import _thread
import os
import re
import psutil

from avalon.tools import loader
from avalon.tools import workfiles
from avalon.tools import publish
from avalon.tools import creator
import pyblish.api
from avalon.vendor.Qt import QtWidgets
from avalon import api
import avalon.tvpaint                                                   
                                                                        

#from avalon.vendor.Qt.QtNetwork import *
import pytvpaint_avalon.functions as tvp
import socketserver

QT_APP = QtWidgets.QApplication.instance() or QtWidgets.QApplication([])

class MyRequestHandler(socketserver.BaseRequestHandler):
    """
    The request handler class for our server.
    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        # self.request is the TCP socket connected to the client
        self.data = self.request.recv(1024).strip()
        print(f"{self.client_address[0]} wrote: {self.data}")
        global QT_APP
        if self.data == b"START":
            self.request.sendall(b"confirmed")
        if self.data == b"OPEN workfiles":
            self.request.sendall(b"Wrapper will open the workfiles-tool")
            self.openworkfiles()
        if self.data == b"OPEN creator":
            self.request.sendall(b"Wrapper will open the Creator-tool")
            self.opencreator()
        if self.data == b"OPEN loader":
            loader.show()
            QT_APP.exec()

        if self.data == b"OPEN publish":
            pyblish.api.register_target("submarine.tvpaint")
            publish.show()
            #QT_APP.exec()


        if self.data == b"END":
            def shut_me_down(server):
                server.shutdown()
            _thread.start_new_thread(shut_me_down, (wrapperserver,))
            self.request.sendall(b"Shutting down the python_wrapper-server")

        tvp.send_release()


    def openworkfiles(self):
        try:
            global QT_APP
            workfiles.show()
            QT_APP.exec() 
        except Exception as e:
            print("e", repr(e))
            sys.stdout.flush()
            tvp.send_release()


    def opencreator(self):
        try:
            global QT_APP
            creator.show()
            QT_APP.exec() 
        except Exception as e:
            print("e", repr(e))
            tvp.send_release()


def run_any_startupscript():
    if os.getenv("PYTHONTVPAINT_STARTUPSCRIPT") is not None:
        startupscript = os.environ['PYTHONTVPAINT_STARTUPSCRIPT']
        try:
            spec = importlib.util.spec_from_file_location("jaja", startupscript)
            startupmodule = importlib.util.module_from_spec(spec)
            sys.stdout.write("Running startupscript:main()\n")
            sys.stdout.flush()
            spec.loader.exec_module(startupmodule)
            startupmodule.main()
        except:
            raise


if __name__ == "__main__":
    # first, check if already running:
    pids = [p.pid for p in psutil.process_iter()
                if bool(re.search(__file__, " ".join(p.cmdline()), re.I))
                    and p.pid != os.getpid() ]
    if len(pids) > 0:
        print("'%s' is already running(pids=%s), so exiting.." % (__file__, pids))
        sys.exit(0)

    if 'TVP_EXEC' in os.environ:
        program_exec = os.environ['TVPAINT_EXEC']
    else:
        program_exec = "/usr/share/tvpaint-developpement/tvp-animation-11-pro/tvp-animation-11-pro"

    run_any_startupscript()
    socketserver.TCPServer.allow_reuse_address = True
    wrapperserver = socketserver.TCPServer(("127.0.0.1", 4242), MyRequestHandler)

    my_env = os.environ.copy()  # reserve portnumbers later for comm?
    tvprocess = subprocess.Popen(program_exec, env=my_env)
    sys.stdout.write("TVPaint started: pid=%d\n" % tvprocess.pid)
    sys.stdout.flush()

    # now start our server
    try:
        wrapperserver.serve_forever()
    except KeyboardInterrupt:
        pass

    wrapperserver.server_close()

    ret = tvprocess.wait()
    sys.stdout.write("TVPaint ended with returncode: %d\n" % ret)
    sys.stdout.flush()
    print("launch.py ended")

