# Spud Tune
Convert Oculus Rift SPUD files (ohctfu/ohctfm) from / to PNG

## Compile on Linux

First make sure you have the ImageMagick development files. Then run:

```
gcc `pkg-config --cflags --libs MagickWand` spud_to_header_png.c -o spud_to_header_png
gcc `pkg-config --cflags --libs MagickWand` spud_from_header_png.c -o spud_from_header_png
```

## Dependencies

- ImageMagick (https://www.imagemagick.org/script/index.php)

## Usage

First unpack the SPUD archive using a Zip-file unpacker (unzip, 7zip) which you can find at:

```
C:/<Username>/AppData/Local/Oculus/Spud/HMD_WMHDXXXXXXXXXXX.spud
```

The following will create a ".png" and ".header" file from a ".ohctfu" or ".ohctfm" file: 
```
./spud_to_header_png samples/Global/XXXXXXXXXXXXXX-red_25-20170710_SinglePixel.ohctfm
```

The following will create a ".ohctfu.new" or ".ohctfm.new" file from the ".png" and ".header" file (it will find those when specifying the original SPUD filename):
```
./spud_from_header_png samples/Global/XXXXXXXXXXXXXX-red_25-20170710_SinglePixel.ohctfm
```
