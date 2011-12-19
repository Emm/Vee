===
Vee
===

------------
What is Vee?
------------

Vee is a toy browser, written in Qt4. It uses WebKit as a rendering engine, like Chrome and Safari. Vee has several differences with the major browsers in the market:

    - it's immature and will eat your children. In fact, it doesn't even do "make install" (yet)
    - it uses one process per tab (using xembed and DBUS for IPC)
    - it has a built-in basic vim input bar and vim shortcuts

Also, this is probably horrible C++ (I'm not particularly fluent in it).

------------
Building Vee
------------

You will need CMake, the development libraries of Qt4, QtWebKit, QtDBUS,
QtAlgrorithms, TClap (command-line options)

You probably want to do an out-of-source build. To do so, in the root directory
of Vee, do::
    
    mkdir build
    cd build
    cmake ..
    make


----------
Using Vee
----------

After building Vee, you will find two executables in your build directory:
**src/vee-web/vee-web** and **src/vee-frontend/vee**. The former is the executable
used to display the contents of a tab. You can use it standalone:
`src/vee-web/vee-web github.com`. However, it's only useful for testing. To
start vee, do `src/vee-frontend/vee github.com`. You can use *:* at any time to
switch to command mode. This will put the focus on the input bar and let you
enter vim commands. At this point in time, Vee only recognizes `o $url`/`open
$url` (will open *$url* in the current tab) and `t $url`/`tab $url` (will open
*$url$* in a new tab).

Have fun.
