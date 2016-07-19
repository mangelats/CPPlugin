# CPPlugin 
Small C++11 library for Windows and POSIX compilant operative systems which allows to create drag-and-drop fully compiled plugins.

## How do you use it?
The main project designs an interface for the plugins to call to and all the events that the plugin might support. Any developer that wants to make a plugin will use the interface and its events to extend the main project.

## How does it work?
It works by compiling the plugin into a dynamic shared library (.dll/.so) which the main program can dynamically load.

## Security warning
The plugin will be loaded with the same permissions as the process that load it. Like a program you have to trust the developer which made this plugin before using it.
