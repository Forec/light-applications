# TXT Player
> This project is a small script for **turn a video into many txts and display them in CMD just like the origin video**.

## Platform
This small script is written in python 2.7, using many packages, all listed below.
* **PIL** : You can install `PIL` by executing an executable binary file from [here](http://effbot.org/media/downloads/PIL-1.1.7.win32-py2.7.exe).
* **cv2** : cv2 is the openCV packages for python. You can download `openCV` from its [official site](http://opencv.org/). Also, install by pip `pip install opencv-python` is needed. Then, install `ffmpeg`. Download it from [here](http://ffmpeg.org/), I suggest you chose the binary file and just install it simply. After you install `cv2`, `numpy` and `matplotlib` will also be installed.

## Usage
### Turn video into txts
Just run `python2 video2txt.py <parameters>`. Also, you can download the executable binary file `video2txt.exe` from my cloud storage: [here](http://7xktmz.com1.z0.glb.clouddn.com/video2txt.exe). The parameters are:
* --file: the video you want to transform.
* --output or -o: the directory you want to store the txts, you can use absolute path or relative path.
* --frame: how many frames you want to transform.
* --interval: transform a frame between interval frames. For example, when interval is 0, the program will transform every frame. This parameter should not conflict with `play.py`, here, is interval is 0, then the frame parameter in `play.py` should be 25( if your video is 25 frames/s ), else if won't display well. Its default value is 24, it means 1 frame/s if video is 25 frames/s.
* --width: the width of txt image.
* --height: the height of txt image.
* Example: `python2 video2txt.py --file=test.avi --output=pic --frame=600 --interval=0 --width=80 --height=60`, this command will transform first 600 frames of `test.avi` into txts, these txts will be stored in `./pic`, with `80x60` dpi.

### Play
run `python2 play.py <parameters>`. Also, you can download the executable binary file `play.exe` from my cloud storage: [here](http://7xktmz.com1.z0.glb.clouddn.com/play.exe). The parameters are:
* --dir: the txts' path, the directory should only contain the txts.
* --frame: how many txts displayed per sec.
* Example: `python2 play.py --dir=pic --frame=24` means display all txts in `pic` with 24 txts per sec.

### Tips
You may find `openCV` for python is too hard to install. Several tips here to help you, or give you another choice.
* Cannot open video with my tool? You can add `openCV\soureces\3rdparty\ffmpeg`  into your environment variable, or copy `opencv_ffmpeg.dll` and `opencv_ffmpeg_64.dll` from `openCV\soureces\3rdparty\ffmpeg` to your install directory of `python2.7`. Also, change their filenames. Check your `opencv-python` version by `import cv2`, `cv2.__version__`, for example, 3.1.0, then change `opencv_ffmpeg.dll` into`opencv_ffmpeg310.dll` and change `opencv_ffmpeg_64.dll` into `opencv_ffmpeg310_64.dll`. Try again!
* Afraid of so confused steps? Just use `ffmpeg -i foo.avi -r 1 -s WxH -f image2 foo-%03d.png` to change your video into several `.png` images. Here `WxH` is your dpi, `foo.avi` should be replaced by your video. After that, change my code and read those images.

## Update-logs
* 2016-9-22: Add this project.