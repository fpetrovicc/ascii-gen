# ascii-gen

Simple C++ tool powered by ImageMagick that can convert images to their ASCII variants.

It works by converting every pixel RGB to luminosity value, that is then used to map to ASCII values.  
Program does scale image to terminal size, but you need to unzoom it in order to see it properly.

Potential ideas: colored output, selection of multiple luminosity mappings, print to file

Usage: ./ascii-gen [path to image]

## Before:  
![Before image](https://github.com/fpetrovicc/ascii-gen/blob/main/before.jpeg)

## After:  
![After image](https://github.com/fpetrovicc/ascii-gen/blob/main/after.png)
