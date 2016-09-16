# Trojan (Remote-Control)
> This project is a simple remote-control tool with GUI written by Qt. **It only works for Windows platform**. The project still remains some bugs. One of them is: **TRANSFER BIG FILE (eg. OVER 200MB) MAY CAUSE CRASH IN CLIENT**. If you have any good ideas, please [contact me](mailto:forec@bupt.edu.cn), or open your PR. I feel honored to learn from your help.

## Platform
* This project contains two parts. The **SERVER** , which should be executed in remote hosts, is in folder `server`, and the **CLIENT** , is used to control the remote hosts, in folder `client`.
* The Visual Studio project file `remote.sln` is in the `server` folder. I wrote this project with VS Ultimate 2013, version `12.0.21005.1 REL`.
* The Visual Studio project file `client.sln` is in the `client` folder. I wrote this project with Qt-windows-x86-msvc2013_64, version `5.6.0`. The Qt Creator is 32bit based on MSVC2013, version `3.6.1`. Some configurations may have relationship with the tool.

## Usage
### SERVER
* Compile: I remove the `remote.sdf` from the repository since it's too big. If you have VS2013 (or higher version), just open `remote.sln`, else please build a new project with other IDEs.
* Assume the program we get after released is `remote.exe`, it's exactly a CMD window, however, I hide the window at the entrance of function `main()`. You can change the first two lines code in function `main()` into comments, then its form will appear. Even it's hidding itself, you can stil find it by `CTRL+ALT+DEL`. I didn't hide the process from taskmgr.
* Usage: Just run the executable file `remote.exe`( you need to compile it yourself ). If you delete the code for hidding in `main.cpp`, you can see it print `Server run at IPv4 address <your-ip-address>...`.
* **Attention**: If you want to use `remote.exe` under **x64** system, I suggest that you should compile it as x64.
* Two files will be created under `E:` in remote hosts, I didn't hide these files. You can use the function provided in `trojan.cpp` which definition is `bool hideFile(const char *path)`. The two files are `E:\key.log` and `E:\screen.tmp`, which will be created by `remote.exe` when client ask for keyboard record or screenshots. **If you want to hide them, remember to use function `hideFile` to hide `E:\screen.tmp` whenever you ask for screenshot.** Since the new screenshot overrides the past screenshot, but the keyboard record is an append operation.

### CLIENT
* Compile: I put all files needed in folder `client`, if you have Qt Creator, just open `client.pro`, else please build a new project with other IDEs.
* Assume the program we get after released is `client.exe`, it will show you a window like this. Buttons left are `Refresh` and `Create`. You can click `Create` to add a remote host, click `Refresh` to refresh all hosts' status.
<img src="/picture-for-readme/remote-control-client-1.png" width = "400px"/>
* You can right click under a remote host, several functions provided like the following picture. Since the remote host you choose is not connected yet, you cannot do any operation to the remote host before you connect it.
<img src="/picture-for-readme/remote-control-client-2.png" width = "400px"/>
* After connect to a remote host such as the picture followed, you can do some basic operations to the remote host.
<img src="/picture-for-readme/remote-control-client-3.png" width="400px"/>
* **Functions**
 * **Connect/Disconnect**: Build or release connections with selected remote host.
 * **Set IPv4 Address**: Set Ipv4 address for remote host. The IPv4 address must  be valid or the table won't accept.
 * **Modify Notes**: Make notes for the selected remote host.
 * **Get Username**: Get the remote host's username.
 * **Get File**: Input the **absolute path of file** you want to get in remote host, then click `Get File`. The client will ask you to confirm your operation  since **the client will change into disabled mode when transferring files**. That means, when receiving file from remote host, you can't operate it. Also, this function remains a serious bug, **very big file (perhaps over 200MB) transmission will cause crash**. Be careful when use this function, if crashed, restart the client.
 * **Get Processes List**: Get all the processes running in remote host, the client will display these information in the `QTextEdit`, which is the widget with `>` in the last picture. That widget cannot be edited, but you can copy its content.
 * **Get ScreenShot**: Get the screenshot of remote host. After receving the screenshot, client will open it automaticly.
 * **Get Keyboard Record**: Get the remote host's keyboard record, records will be displayed in `QTextEdit` too.
 * **Export Logs**: Export your operation history to the selected remote host to a `.log` file.
 * **Delete**: Delete the remote host from your database.

## Attentions
* **Project Configuration**: Since I use `QSqlDatabase` to query and store data, a line `Qt += sql` has been added to the project file `client.pro`.
* **Icon Configuration**: The client's icon can be changed, you just need to replace my `icon.ico` with your own icon. However, my girlfriend likes the icon of `V for Vendetta`, I don't think other icons could be better.
* **Path**: Several files and directories will be created when you run `client.exe`, I list them below.
 * **files**: Directory for storing the files you get from remote hosts. They are stored just as they like in remote hosts. So remember when you want to get a new file from remote hosts, make sure there is not file with same name in dir  `files` already.
 * **screenshots**: Directory for storing the screenshots you get from remote hosts. They are stored with name format: `<remote-ip-address>-yyyy-MM-dd-hh-mm-ss.bmp`.
 * **logs**: Logs you exported will store in this directory. Their names are same with their related ip addresses.
 * **data.db**: The sqlite3 database we use to store all the remote hosts' informations( ip address, notes, status, names...).
 * **temp.tmp**: Used to buffer data. Don't care about it.

## Examples For Use
* Get Processes List. As you can see, I ran `remote.exe` in my PC to show how the client works. It can still work well in remote hosts.
<img src="/picture-for-readme/remote-control-client-4.png" width="400px"/>
* Get KeyBoard Record. Here I hide my record in the picture.
<img src="/picture-for-readme/remote-control-client-5.png" width="400px"/>
* Get File. Here I get `G:\Backup\Wireshark-win64-2.0.0.exe` from remote host. It's 38.9 MB, and the client saves it as `%PATH_TO_CLIENT%/files/Wireshark-win64-2.0.0.exe`.
<img src="/picture-for-readme/remote-control-client-6.png" width="400px"/>
<img src="/picture-for-readme/remote-control-client-7.png" width="400px"/>
* Get Screenshot. The client opens the screenshot after it received.
<img src="/picture-for-readme/remote-control-client-8.png" width="400px"/>

## Bugs
* When send big files, the client and remote server are hard to synchronizate. **Temporarily, when sending data, I ask both the client and remote server waiting for a response to send next packet.** Obviously it's not a correct method. So crash happened when transmitting big files.
* **TODO**: It should be possible that user can get remote file and do other operations at the same time. However, to achieve this function, I need to start another thread, it's not hard, but the current function is enough for temporary use, so I didn't add this part. Maybe later.

## Update-logs
* 2016-9-11~12: Write GUI for client, with some basic tests.
* 2016-9-14: Add `trojan.h/cpp`, with basic functions including `stringToLPCWSTR`, `sendFile`, `readFileIntoBuf`, etc.
* 2016-9-15: Add several main functions, deal with keyboard, get screenshot, etc.
* 2016-9-16: Finish all functions, fix some bugs, improve the client. Some basic tests passed.