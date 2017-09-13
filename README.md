# Spud Tune
Convert Oculus Rift SPUD files (ohctfu/ohctfm) from / to PNG

## Compile on Linux

First make sure you have the ImageMagick development files, then run:

```
make
```

## Compile on Linux for Windows

First make sure you have the ImageMagick development files and MinGW, then run:

```
make TOOLCHAIN=/usr/x86_64-w64-mingw32/ CC=x86_64-w64-mingw32-gcc STRIP=x86_64-w64-mingw32-strip PKGCONFIG=x86_64-w64-mingw32-pkg-config EXTENSION=.exe archive-windows
```

## Dependencies

- ImageMagick 7 (https://www.imagemagick.org/script/index.php)

## Usage

First unpack the SPUD archive using a Zip-file unpacker (unzip, 7zip).
You can find the archive to unpack at:

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
