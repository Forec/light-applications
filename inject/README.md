# Inject
> This program is a tool for DLL injection work under windows platform. However, it stills remains many serious problems. One of them is: **THIS TOOL IS USELESS UNDER WINDOWS 8 AND HIGHER VERSIONS**. If you have any good ideas, please [contact me](mailto:forec@bupt.edu.cn), or open your PR. I feel honored to learn from your help.

## Platform
* The Visual Studio project file `inject.sln` is in the current folder. I wrote these code with VS Ultimate 2013, version `12.0.21005.1 REL`.

## Usage
* Compile: I remove the `inject.sdf` from the repository since it's too big. If you have VS2013 (or higher version), just open `inject.sln`, else please build a new project with other IDEs.
* Assume the program we get after compiled is `app.exe`, it should be used in a `cmd` or `powershell` window. You can right click the blank with `SHIFT` pressed under the same folder of `app.exe`, and choose `Open CMD here`, or just `cd` into the path of `app.exe`.
* Usage: `app.exe <process-you-want-to-inject> <DLLfile-you-want-to-use>`. It will print `succeed..` if the injection succeed, else `failed..`.
* **Attention**: If you want to use this tool under **x64** system, make sure the binary file you compiled is x64. This can be set in VS platform configuration. Also, the DLL file you want to inject must be x64 too.

## Update-logs
* 2016-9-14: Add `inject.h/cpp`, with functions including auto-start, inject dlls into process, list running processes, etc.
* 2016-9-15: Finish this project, with bugs under Windows8 and higher versions.