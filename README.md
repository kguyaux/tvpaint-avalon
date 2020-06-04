# TVPaint-avalon

This project is dedicated to make TVPaint work with Avalon.
It works by having special tvpaint-plugins that are made for executing a specific avalon-ui like 'workfiles' or 'publish'. The plugins should also be compiled with this project. (manually still, look below for instructions)  
When the plugins are loaded when tvpaint starts, they provide the functionality to communicate with
python-scripts. These pythonscripts should further integrate TVPaint with Avalon.

This project is very much in progress, parts are still in the proof-of-concept-fase. It has been mainly tested on fedora(linux). So you can say it is still in the testing-fase.
In my testing-env I have the module `avalon-core` & `pyblish`, etc. pointed to, by putting it in $PYTHONPATH  
the rest of the dependacies are in `requirements.txt`

### Download:
`git clone https://github.com/kguyaux/tvpaint-avalon.git --recurse-submodules`  
The recurse-option is used to download a submodule that enables the plugins to write jsonstrings

### Usage
To have it working, you must first compile&install, then launch tvpaint with the wrapper, like this:
`python3 pytvpaint_avalon/launch.py`
The wrapper imports all the necessary avalon stuff, and also starts the socket-server that handles
the stuff that is sent from TVPaint.

### How it works:
1. Wrapper(launch.py) is executed & avalon-core is imported
2. py-socketserver starts & listens for commands on port 4242
3. tvpaint launches
4. tvpplugins are loaded
5. user runs plugin, for example; the "open workfiles-ui-plugin"
6. TVPAint blocks user-input & the string "open avalon-workfiles-ui" is sent to py-socketserver
7. TVPaint starts listening on port #### for return-messages(strings)
7. the python-Wrapper(launch.py) executes the pyqt5-gui for the workfiles.
8. User selects a workfile, gui closes, string with path is sent back to TVPaint(still listening)
9. TVPaint(ie, the plugin) receives the string with the path & loads the file
10. TVPaint unblocks user-input & the user can now proceed working on the file

### About the python module(pytvpaint_avalon)
The module provides a wrapper(`launch.py`) that launches tvpaint with the serverservice that listens for commands send by tvpaint.
It also communicates back to tvpaint. `functions.py` provides the functions that do things with tvpaint, like retreiving information or other things.


### About the TVPaint-plugins(build & install)
Cmake must be installed, and the TVPaint_SDK must be present also.
You have to change these first lines of `plugins/CMakelists.txt` to let them point to where the TVP_sdk-stuff is located, by changing:  
```
set(TVP_SDK_LIB <path of lib-dir of TVP-sdk-folder>)
set(TVP_SDK_INCLUDE <path of include-dir of TVP-sdk-folder>)

```

To build these for each system, do the following:
#### Linux
`cd python-avalon/build
cmake ../
make`
In the Cmakelists.txt-file you can see the install locations. Check it with your own tvpaint-install and change it, if necessary.
To install the plugins into tvpaint(so that they load when tvpaint starts):
`sudo make install`

#### Windows
I have not tested it on windows yet. One way I know is to install the Windows-version of Cmake and generate
a VisualStudio-solution-project, that can be opened & built from within VS.
I don't know where the pluigns will end up, but when you found them, you can copy them nanually to your TVPaint-pluginfolder :-)



