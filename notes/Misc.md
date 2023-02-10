# Miscellaneous

## _WIN32 vs _MSC_VER Predefined Macros
As stated in the [reference](https://github.com/osmcode/libosmium/issues/224#issuecomment-323811903):

> Well, there is no right or wrong. Both are correct - each for their purpose.

- `_WIN32` is for generally checking if the app is built for/on Windows.
- `_MSC_VER` is specifically targeted towards the Microsoft compiler (aka Visual Studio or C++ Build Tools) and checking the version thereof as each version might have different bugs aehm features 😏
```txt
MSVC++ 4.x  _MSC_VER == 1000
MSVC++ 5.0  _MSC_VER == 1100
MSVC++ 6.0  _MSC_VER == 1200
MSVC++ 7.0  _MSC_VER == 1300
MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
MSVC++ 14.1 _MSC_VER == 1910 (Visual Studio 2017)
```