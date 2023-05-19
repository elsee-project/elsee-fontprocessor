# Elsee-FontProcessor
Font Processor for integration with Elsee. The font processor takes a .tff font file, dpi and font size as an argument. It is meant to create characters to be displayed in the document viewer of Elsee any other part of Elsee that has
study content for the user. Currently, this submodule is being built for minimal functionality to process the OpenDyslexia3.tff file to use as a default font. This minimal functionality was determined by viewing the tables inside of it through use of a hex editor.


## Standalone Installation
The Font Processor, like other parts of Elsee, uses CMake to generate a build system. In the following instructions "out" can be replaced with your preferred name for your build.  Instructions for Windows will be made available at a later time.


### Linux
Open your preferred terminal interface and enter into root of the module's directory. You must first make a folder for your build by entering `mkdir -p .out/` into the terminal; this will be where the final binary will be available. Afterwards, enter the created folder with `cd out/` then enter `cmake -S ../`. Return to the previous directory by entering `cd ../`, and lastly, enter `make`. Upon completion, the compiled c object file will be inside the out folder.
