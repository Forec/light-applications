# LAN iChat
> This project is a simple video chat tool.   
I decided to write this because I want to have a free ichat in school lan with my GF at first. However, since we always meet, ichat is not required. Recently, one of my friends spent much data flow on ichat with his girlfriend, so I pick it again. It supports IPv4 and IPv6, uses TCP and not supports audio temporarily. **Not finished yet** , next step is to add audio and change into UDP. Maybe we could define the current version as `v0.1`. So worried about your data flow fee on ichat? Use this tool to avoid that.

## Platform
This small script is written in python 3.5, using some packages, all listed below.
* **cv2** : cv2 is the openCV packages for python. You can download `openCV` from its [official site](http://opencv.org/). Also, install by pip `pip install opencv-python` is needed. Then, install `ffmpeg`. Download it from [here](http://ffmpeg.org/), I suggest you chose the binary file and just install it simply. After you install `cv2`, `numpy` and `matplotlib` will also be installed.
* Built-in packages: `sys`, `struct`, `pickle`, `time`, `threading`, `argparse` and `socket`.

## Usage
Just run `python3 ichat.py <parameters>`. Also, you can download the executable binary file `ichat.exe` from my cloud storage: [here](http://7xktmz.com1.z0.glb.clouddn.com/ichat.exe). Press `Esc` or `Ctrl-C` to stop this tool. The parameters are:
* --host: the remote host address you want to connect. Support IPv4 and IPv6. Default is `127.0.0.1`.
* --port: the port you want to use. The remote should use the same port as yours. Default is 10087.
* --version or -v: the IP version you want to use. Default is 4, if you want to use IPv6, remember setting `--version=6`.
* --noself: The tool will display both the remote and your videos in default. If you do not need to show yourself on your screen, use `--noself=True`. Do not set any value to this flag if you need to observe your image.
* Press `Esc` to quit.
* One screenshot for usage is below. I had an ichat with myself.   
<img src="http://7xktmz.com1.z0.glb.clouddn.com/lan-video-v0.1.png" width = "400px">


## Update-logs
* 2016-9-23: Add this project, video is ok. Define `v0.1`.