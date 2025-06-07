# ccrack
A simple hash cracker using a wordlist. The program uses multithreading, so it should run fast enough then reading a massive wordlist.

## Usage
> WARNING: When loading a big wordlist (like hundreds of megabytes or even gigabytes), make sure you have enough free memory. The program will load the entire file all at once.

```
Usage: ccrack <hash> <wordlist>
```

## Compile
> NOTE: Only compiles on Linux (maybe MacOS too). I have no plans to make the code cross-platform
```bash
mkdir build
cd build
cmake ..
cmake --build .
```
The executable will be named `ccrack`.
