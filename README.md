# loggerDemo

This is a small module for logging. It can do asynchronous writing of strings to a file.

### Features:

 - A call to Write returns immediately so that the calling application can get on with its work without waiting for the log to be written.
 - If come cross midnight a new file with a new timestamp will be made
 - Possible to stop the component in two ways:
     - stop right away and if any outstanding logs they are not written
     - stop by the wait for it to finish writing outstanding logs if any
 -  If an error occurs the calling application won't be put down.

### Compile

Run `make` in the root and it will generate two executables, `main` and `test`, and a corresponding static library `liblogger.a`
