# Miscellaneous

## Get Executing Assembly Path
As stated in the [reference](https://stackoverflow.com/questions/1528298/get-path-of-executable):
```cpp
#ifdef _WIN32
    #include <windows.h>
#else
    #include <limits.h>
    #include <unistd.h>
#endif

std::filesystem::path get_executing_assembly_path()
{
    std::string path_str;
#ifdef _WIN32
    char path[MAX_PATH];
    size_t len = GetModuleFileName(NULL, path, MAX_PATH);
    path_str = std::string(path, len);
#else
    char path[PATH_MAX];
    /*POSIX*/ssize_t len = readlink("/proc/self/exe", path, PATH_MAX);
    path_str = std::string(path, (len > 0) ? len : 0);
#endif

    std::filesystem::path file_path(path_str)
    return file_path.remove_filename();
}
```
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