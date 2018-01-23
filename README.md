# wmf2png

This is just a simple exe wrapped around the [GDI+ Metafile class](https://msdn.microsoft.com/en-us/library/windows/desktop/ms536391(v=vs.85).aspx). I needed to get image data out of some .WMF files and maybe you do too.

You can [download a Windows binary](https://us-east.manta.joyent.com/tpaul/public/windows/wmf2png.zip) or build from source as shown below.

## Usage

```
> wmf2png.exe

Usage:
  wmf2png.exe INPUTFILE.WMF OUTPUTFILE.PNG
```

## Build from source

If you're familiar with Visual Studio then you probably don't **NEED** [CMake](https://cmake.org/download/) but it's convienent for me:

```
> mkdir build
> cd build
> cmake ..
> wmf2png.sln
```
