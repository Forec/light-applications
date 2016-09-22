# TXT Player
> This project is a small script for **turn a video into many txts and display them in CMD just like the origin video**.

## Platform
This small script is written in python 2.7, using many packages, all listed below.
* **PIL** : You can install `PIL` by executing an executable binary file from [here](http://effbot.org/media/downloads/PIL-1.1.7.win32-py2.7.exe).
* **cv2** : cv2 is the openCV packages for python. You can download `openCV` from its [official site](http://opencv.org/). Also, install by pip `pip install opencv-python` is needed. Then, install `ffmpeg`. Download it from [here](http://ffmpeg.org/), I suggest you chose the binary file and just install it simply. After you install `cv2`, `numpy` and `matplotlib` will also be installed.

## Usage
### Turn video into txts
Just run `python2 video2txt.py <parameters>`. Also, you can download the executable binary file `video2txt.exe` from my cloud storage: [here](). The parameters are:
* --file: the video you want to transform.
* --output or -o: the directory you want to store the txts, you can use absolute path or relative path.
* --frame: how many frames you want to transform.
* --interval: transform a frame between interval frames. For example, when interval is 0, the program will transform every frame. This parameter should be same with that in `play.py`, else if won't display well.
* --width: the width of txt image.
* --height: the height of txt image.
* Example: `python2 video2txt.py --file=test.avi --output=pic --frame=600 --interval=0 --width=80 --height=60`, this command will transform first 600 frames of `test.avi` into txts, these txts will be stored in `./pic`, with `80x60` dpi.

### Play
run `python2 play.py <parameters>`. Also, you can download the executable binary file `play.exe` from my cloud storage: [here]().

## Update-logs
* 2016-9-22: Add this project.