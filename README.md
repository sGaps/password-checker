# Password Checker

Simple password checker built with _C_, _flex_ and _bison_.

### Installing

Just type in the terminal:

```
git clone https://github.com/sGaps/password-checker.git
```

### Compiling:

As easy as write:
```
make
```
or
```
make all
```
if you want to generate the executable and all unit tests.

### Pack to redistribute

Use this command for that purpose:
```
make package
```
It will create a new folder called `package` which will contain a zip file with the same name (`package.zip`).

> _**Note:**_ _the zip file will only cointain: the `makefile` and the subdirectories `src/`, `src-bin/` and `tests/*`._

change the name of the zip-file by using:
```
make package ZIPFILE=compressed
```
And it will create the file `package/compressed.zip`

### Remove the generated files

Clean the directory by typing:
```
make clean
```

