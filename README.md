# wmf2png

This is just a simple EXE wrapped around the [GDI+ Metafile class](https://msdn.microsoft.com/en-us/library/windows/desktop/ms536391(v=vs.85).aspx). I needed to get image data out of some [.WMF files](https://msdn.microsoft.com/en-us/library/cc250370.aspx) and maybe you do too.

You can [download a Windows binary](https://us-east.manta.joyent.com/tpaul/public/windows/wmf2png.zip) or build from source as shown below.

## EXE Usage

```
> wmf2png.exe

Usage:
  wmf2png.exe INPUTFILE.WMF OUTPUTFILE.PNG
```

## Node.js Usage

This only works on Windows and is simply a wrapper around the EXE, but it's useful if you need to perform the conversion in Node.js (exporting records from a legacy database, etc):

```
const
    {readFileSync, writeFileSync} = require("fs"),
    wmf2png = require("./wmf2png"),
    input = readFileSync("./somefile.wmf");

// `input` must be a Buffer containing the WMF contents
wmf2png(input, (error, output) => {
    if (error) {
        throw error;
    }
    // `output` is a new buffer with the PNG image data
    writeFileSync("./sig.png", output);
});
```

## Build from source

If you're familiar with Visual Studio then you probably don't **NEED** [CMake](https://cmake.org/download/) but it's convienent for me:

```
> mkdir build
> cd build
> cmake ..
> wmf2png.sln
```


