# Alexandra Video Library v1.5
##### Small, fast, but powerful video library, written in C++ with Qt5.
[![Build Status](https://travis-ci.org/jeka-js/alexandra.svg)](https://travis-ci.org/jeka-js/alexandra)


Alexandra Video Library — a simple and comfortable program for organization and management of the your video library. You can completely customize your movies in a list or a grid, add detailed information, tags, posters, view technical information about the file and use advanced search.

**Features:**

 - Lot of fields with information about the film.
 - Automatically retrieving data from the internet.
 - Fast filtration in the view and powerful advanced search.
 - Fully customizable user interface.
 - Selection of your favourite player.
 - Hard drive scanner.
 - Random film selection.
 - Views statistics.
 - MediaInfo built-in.

![Main window List](/doc/screenshots/01_MainWindow_List.png "Main window -- list")

![Main window Grid](/doc/screenshots/02_MainWindow_Grid.png "Main window -- grid")

View, manage, collect. Alexandra Video Library.

=

**Build depends:**

 - Qt5Core
 - Qt5Gui
 - Qt5Network
 - Qt5Widgets
 - libmediainfo (optional)

**Build:**

    qmake
    make

**Install:**

    sudo make install

or

    sudo checkinstall

