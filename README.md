# Alexandra Video Library v1.3
##### A small opensource video library with great potential, written in C++ using Qt5 framework.

=

Alexandra Video Library - a simple and convenient program for the organization and management of the home video library. You can completely customize the catalog movies in a list or a grid, add a description and detailed information about the movie in a lot of fields available, attach posters and much more. You can also use the films scanner that will find and add all video files on your disk.

Directly from the program, you can start playing the film, with the available choice of any video player on your computer if you wish.

If you move your films to another folder, the master of displaced files will detect them and you can continue to easily use the program.

There are two types of search: quick - enter words from the keyboard immediately after starting the application, and advanced - select the key fields and the search terms.

It is worth noting the random play function of the film, which is especially handy when you can not determine which watch movies from your video library.

The program also displays detailed information about a file using the MediaInfo library, so it does not need to resort to using additional software for view information about codecs, resolutions, audio tracks, subtitles and other data from the selected movie.

=

**Build depends:**

 - Qt5Core
 - Qt5Gui
 - Qt5Widgets
 - Qt5Concurrent
 - libmediainfo
 - libstdc++

**Build:**

    cd ./src/
    qmake
    make

**Install:**

    sudo make install

