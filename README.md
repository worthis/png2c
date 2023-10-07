# png2c

This tool is able to convert a font of png format to binary font format and c raw data for RetroArch rgui and mame2003plus.

1 Install libraries

> to build in linux environment


    sudo apt install libsdl1.2-dev libsdl-image1.2-dev


> to build in windows environment (msys2 mingw64)

    pacman -S mingw-w64-x86_64-SDL
    pacman -S mingw-w64-x86_64-SDL_image

2 Make binary

    make

3 Convert png file to c

./png2c <png file>

    ./png2c bitmap_cyrillic.png

`bitmap_cyrillic.h` - c raw data file and 
`bitmap_cyrillic.bin` - RetroArch binary font file are generated.
    
    /* FontName = bitmap_cyrillic.png */
    /* generated with png2c by trngaje */
    
    #define FONT_WIDTH 6
    #define FONT_HEIGHT 10

    static const unsigned char bitmap_bin[] = {
    0x02, 0x01, 0x7C, 0xC1, 0x17, 0x7C, 0x00, 0x00,  
    0x80, 0x02, 0x7C, 0xC1, 0x17, 0x7C, 0x00, 0x00,  
    0xC0, 0x21, 0x38, 0x92, 0x24, 0x29, 0x00, 0x00,  
    0x08, 0x01, 0x7C, 0x41, 0x10, 0x04, 0x00, 0x00,  
    0x80, 0x13, 0x05, 0x47, 0x10, 0x39, 0x00, 0x00,
    ...

4 To build for mame2003plus, add define in Makefile

    CFLAGS = $(shell sdl-config --cflags) -DMAME2003PLUS
