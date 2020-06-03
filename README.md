# python-avalon
-----------

_under construction_

This project is dedicated to make TVPaint work with Avalon.
It works by having special tvpaint-plugins that can be compiled with this project.
When the plugins are loaded when tvpaint starts, they provide the functionality to communicate with
python-scripts. These pythonscripts should integrate TVPaint with Avalon.

This project is in progress, some parts are still in the proof-of-concept-stage.

To have it working, you must launch tvpaint with the wrapper, like this:
`python3 pytvpaint/launch.py`
The wrapper imports all the necessary avalon stuff, and also starts the socket-server that handles
the stuff that is sent from TVPaint.




### Download:
`git clone dfgasfasdf --recurse-submodules`
recurse-option is used to download a submodule that enables the plugins to write jsonstrings

### The TVPaint-plugins
Cmake must be installed, and the TVPaint_SDK must be present also.
You have to change these first lines of `plugins/CMakelists.txt` to let them point to where the TVP_sdk-stuff is located, by changing:  
```
set(TVP_SDK_LIB <path of lib-dir of TVP-sdk-folder>)
set(TVP_SDK_INCLUDE <path of include-dir of TVP-sdk-folder>)

```

To build these,  you must have cmake installed. For each system, do the following:
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



