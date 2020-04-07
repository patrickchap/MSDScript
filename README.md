# MSDScript


Getting started: installation and basic usage

 1. Download MSDscript file
     1. In the terminal cd into MSDscript/buildSrcTest/build
     2. Type in the command $ cmake ../
     	1. This will look for the CMakeList.txt to configure, generate, and write the build files to the build directory
	2. The CMakeList.txt has instructions to build an executable and a static library
     3. Next type in the command $ cmake --build .
     4. If you ls you can see that there is the executable MSDscript and the static library libMSDLib.a
     5. The header for the static library MSDScript/single header/msdscript.hpp
     5. If you type $ ./MSDscript 
     	1. Run MSDscript in the terminal
     6. $ ./MSDscript -opt
     	1. Run MSDscript in optimization mode 
     7. $ ./MSDscript -step
     	1. Runs MSDscript using continuations
     8. MSDscript/single header/msdscript.hpp
     	1. Header file for the libMSDLib.a static library


API documentation: classes, functions, User Guide, and Description

 1. MSDscript/buildSrcTest/doc/html
