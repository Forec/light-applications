# Port Scanner
> This project is a tool for **scanning opened ports** . This tool has three implementations by different languages: `Python 3.5`, `C++` and `Golang 1.6`. The `Go` version is the fastest. The `C++` version remains some bugs, one of them is **program will be blocked when the number of threads exceeds 1600**. If you have any ideas or suggestions, please [e-mail](mailto:forec@bupt.edu.cn) me or open your PR.

## Platform
This small script is written in three languages: `Python 3.5`, `Golang 1.6` and `C++`.
* The Visual Studio project file `port-scanner.sln` is in folder `C++`. I wrote these code with VS Ultimate 2013, version `12.0.21005.1 REL`.
* `port-scanner.go` is the implementation of `Golang 1.6`, in folder `Go`.
* `port-scanner.py` is the implementation of `Python 3.5`, in folder `Python`.
* All packages used are built-in.

## Usage
* Python version: `python3 port-scanner.py ip_address start_port-end_port`. For example, `python3 port-scanner.py localhost 0-65535`, then the tool will scan all the ports opened in your PC. You can change the timeout value in code.
* Go version: `go run port-scanner.go --host=ip_address --cores=cores`, here `--cores` parameter is used to assigning how many cpu cores to use. Default is 2 CPU cores. You can change the timeout value in code. This version is the fastest.
* C++ version: You need to build the project first. In `main` function, you should create a `Scanner` object, you can specify the ip address, port range and threads you want to use. However, the tool may be slower when the number of threads is beyond 1600. Also, sometimes the `C++` version cannot scan all the opend ports. Maybe I will fix this later. Call it `v0.1` temporarily.

## Update-logs
* 2016-9-25: Add this tool.