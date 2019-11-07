/**
 * This file adds some Methods to the Module generated by emscripten.
 * The file should be included to the buildprocess by adding the --pre-js option to emcc.
 */

/**
 * Adds MyStdout to the Module-Object.
 * MyStdout is used to buffer the output (stdOut) of the wasm.
 */
let setupMyStdOut = function () {

    let MyStdout = {}

    // buffer contains the string since the last flush
    MyStdout._buffer = "";

    // contents contains the flushed string
    MyStdout._contents = "";

    // We setup emscripten later to call this function if stdout is called from c-Code
    MyStdout.print = function (charCode) {
        // is this a flush? see https://emscripten.org/docs/api_reference/Filesystem-API.html#FS.init
        if (charCode == null) {
            MyStdout._contents += MyStdout._buffer;
            MyStdout._buffer = "";
            return;
        }
        // this is not a flush
        MyStdout._buffer += String.fromCharCode(charCode);
    }

    // returns the contents
    MyStdout.getContents = function () {
        return MyStdout._contents;
    }

    // clears the content
    MyStdout.clear = function () {
        MyStdout._contents = "";
    }

    // flushes the buffer to content
    MyStdout.flush = function () {
        MyStdout.print(null);
    }

    Module.MyStdout = MyStdout;

    FS.init((charcode) => { Module.MyStdout.print(charcode) }, (charcode) => { Module.MyStdout.print(charcode) }, (charcode) => { Module.MyStdout.print(charcode) });
}

/**
 * Add a function that wraps the main Method call
 */
let addRunMiniSATMethod = function () {
    /**
     * Run miniSats's Main-Method. options can be used to pass commandline arguments.
     * Data should contain the contents of the file to be solved. 
     */
    Module.runMiniSAT = function (data, options) {
        args = options || [];
        data = data || "";
       
        // clear what MiniSAT has printed (hopefully nothing) so far.
        Module.MyStdout.clear();
        // Load the data to Memory using emscriptens emulated filesystem
        FS.writeFile('input', data);
        // create a File to write the output to
        FS.writeFile('output', "");
        // the last argument for MiniSAT is the location of the file to be solved
        args.push("/input");
        args.push("/output");
        // now we call main. All prints go to MyStdout
        callMain(args);
        let outputFileContents = FS.readFile("output", { encoding: 'utf8' });
        // flush the buffer
        Module.MyStdout.flush();
        // we do not need the virtual file anymore. So we release it.
        FS.unlink("input");
        FS.unlink("output");
        // Return what MiniSAT has printed during its run.
        return [Module.MyStdout.getContents(), outputFileContents];
    }
}

// Module.prerun should be an array of functions called before the module is initalized
if (!Module.preRun) {
    Module.preRun = [];
}
Module.preRun.push(setupMyStdOut);
Module.preRun.push(addRunMiniSATMethod);
Module["noInitialRun"] = true;