# InjectDLL
> This project is a template for DLLs which can be used in injection tools.

## Platform
* The Visual Studio project file `injectDLL.sln` is in the current folder. I wrote these code with VS Ultimate 2013, version `12.0.21005.1 REL`.

## Usage
* Compile: I remove the `injectDLL.sdf` from the repository since it's too big. If you have VS2013 (or higher version), just open `injectDLL.sln`, else please build a new project with other IDEs.
* **Attention**: If you want to inject your DLL file under **x64** system, make sure the DLL file you compiled is x64. This can be set in VS platform configuration. Also, the process you want to inject into must be x64 too.

## Update-logs
* 2016-9-15: Add the project.