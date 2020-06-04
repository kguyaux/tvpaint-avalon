import os
import csv
import sys
import socket
import tempfile
from pprint import pprint


HOST = '127.0.0.1'  # The server's hostname or IP address
SERVERPORT = 8906        # The port used by the server


def connection(commandfunc):
    def connect(*args, **kwargs):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((HOST, SERVERPORT))
        except ConnectionRefusedError:
            return "Connction refused"

        msg = commandfunc(*args, **kwargs)
        sock.sendall(bytes(msg, encoding='ascii'))
        
        response = sock.recv(1024)
        return str(response, encoding="utf-8")
    return connect


@connection
def get_currentfile():
    return "get currentfile"


@connection
def set_userstring(section, name, data):
    """Sets a tvp_userstring in the running tvpaint-application
    (which gets saved during quit)

    """
    return f"tv_setUserstring {section} {name} {data}"


@connection
def open_tvproject(path):
    """Sets a tvp_userstring in the running tvpaint-application
    (which gets saved during quit)

    """
    return f"open tvproject {path}"

@connection
def save_tvproject(path):
    """Sets a tvp_userstring in the running tvpaint-application
    (which gets saved during quit)

    """
    return f"save tvproject {path}"


@connection
def import_imgsequence(path):
    """Sets a tvp_userstring in the running tvpaint-application
    (which gets saved during quit)

    """
    return f"tvpcmd: tv_LoadSequence \"{path}\""

@connection
def layerdata(temp_datafile):
    return f"get currentstructure {temp_datafile}"



def get_current_layers():
    """Get info of all layers. If no project-id is given.
    then all info of current project is returned
    returns: <dict> that contains all layerdata"""
    fd, temp_datafile = tempfile.mkstemp(prefix="tvpdata_", suffix=".csv")
    temp_layerdir = os.path.splitext(os.path.basename(temp_datafile))[0] + ".layers"
    response = layerdata(temp_datafile)
    print("TVPAINT responded with:", repr(response))
    with open(temp_datafile, 'r') as f:
        for row in csv.reader(f, delimiter=','):
            print(repr(row))


    if os.path.exists(temp_layerdir):
        os.rmdir(temp_layerdir)
    os.unlink(temp_datafile)


@connection
def send_release():
    """
    sending a release-signal so that the tvpaintplugin stops listening
    So tvpaint can continue..
    """
    return "<RELEASE>"


def get_pluginlist():
    """Since we are only able to talk with plugins, you must define them
    and thus know what are the plugins we can talk to..
    This function returns a list with what we can talk to:

    :returns: a list with [{"name":, "port":}]
    """
    pass



