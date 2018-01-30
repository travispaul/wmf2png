// Node.js wrapper
//
// Example usage:
//   const
//      {readFileSync, writeFileSync} = require("fs"),
//      wmf2png = require("./wmf2png");
//
//   wmf2png(readFileSync("./SIGNATURE.wmf"), (error, output) => {
//      if (error) throw error;
//      writeFileSync("./sig.png", output);
//   });
//
const
    fs = require("fs"),
    tmp = require("tmp"),
    { join } = require("path"),
    { pipeline } = require("vasync"),
    { execFile } = require("child_process");

function createTmpInputFile (context, next) {
    tmp.tmpName((error, path) => {
        if (error) {
            return next(error);
        }
        context.tmpInputFile = path;
        next();
    });
}

function createTmpOutputFile (context, next) {
    tmp.tmpName((error, path) => {
        if (error) {
            return next(error);
        }
        context.tmpOutputFile = path;
        next();
    });
}

function saveInputBufferToTmpFile (context, next) {
    fs.writeFile(context.tmpInputFile, context.inputBuffer, next);
}

function execWmf2PngExe (context, next) {
    execFile(join(__dirname, "wmf2png.exe"), [context.tmpInputFile, context.tmpOutputFile], next);
}

function readTmpOutputFileIntoBuffer (context, next) {
    fs.readFile(context.tmpOutputFile, next);
}

function unlinkTmpInputFile (context, next) {
    fs.unlink(context.tmpInputFile, next);
}

function unlinkTmpOutputFile (context, next) {
    fs.unlink(context.tmpOutputFile, next);
}

module.exports = function wmf2png (input, callback) {
    if (process.platform !== "win32") {
        return callback(new Error("WMF conversion only works on the win32 platform"));
    }
    let result = pipeline({
        arg: {
            inputBuffer: input
        },
        funcs: [
            createTmpInputFile,
            createTmpOutputFile,
            saveInputBufferToTmpFile,
            execWmf2PngExe,
            readTmpOutputFileIntoBuffer,
            unlinkTmpInputFile,
            unlinkTmpOutputFile
        ]
    }, (error, results) => {
        if (error) {
            return callback(error);
        }
        results.operations.forEach((operation) => {
            if (operation.funcname === "readTmpOutputFileIntoBuffer" && operation.status === "ok") {
                callback(null, operation.result);
            }
        });
    });
};