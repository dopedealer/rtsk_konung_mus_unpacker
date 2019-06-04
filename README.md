# Rtsk Konung 'Legens of the North' music resource unpacker
Konung 'Legend of the north' resource file music unpacker.
Extracts music from MUSICS.RES file in current directory to current directory.
Now can only extract ogg files from russian collection edition and possibly
'Legend of the north' edition.

Building
=========

Create temporary build dir, go to it, generate make files with cmake (or your
favourite ide project), make.

Example - ms nmake:
-------------------

~~~~
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles"
nmake
~~~~

Example - msvc 2017:
--------------------
~~~~
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017 Win64"
~~~~
