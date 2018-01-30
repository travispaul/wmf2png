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

function spawnWmf2PngExe (context, next) {
    execFile("wmf2png.exe", [context.tmpInputFile, context.tmpOutputFile], next);
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
    let result = pipeline({
        arg: {
            inputBuffer: input
        },
        funcs: [
            createTmpInputFile,
            createTmpOutputFile,
            saveInputBufferToTmpFile,
            spawnWmf2PngExe,
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