# LivePP2
A simple Unreal Engine integration plugin for Molecular Matters Live++ 2.  

This is much faster to iterate with than the default Live++ integration, but it comes with an additional restriction: Neither UBT nor UHT will run. If you make any changes to headers that change the line numbers UHT macros are on, it will fail to compile the patch. Basic rule: Just don't use hot reload for header changes.

# Supported Platforms
The plugin will work in the editor aswell as packaged builds in Debug, DebugGame and Development config. I only implemented support for Windows.

# Supported Live++ Versions
You must use Live++ version >= 2.7.0 due to breaking API changes.

# Installation
1) Install this plugin to your project.
2) Download the official Live++ [release](https://liveplusplus.tech/releases.html) and place the `LivePP` folder next to the `PLACE_LIVEPP_FOLDER_HERE` file in `Source/ThirdParty/LivePPSDK`.
3) Disable Live Coding in Unreal Editor from Editor Preferences -> Live Coding.
4) Add `-LivePP2` to the launch arguments in your IDE.

# Usage (Editor)
1) Start the editor.
2) Modify a file and use Ctrl-Alt-F11 shortcut as usual.

# Usage (Packaged)
1) Start the packaged build with `-LivePP2` argument.
2) Modify a file and use Ctrl-Alt-F11 shortcut as usual.
