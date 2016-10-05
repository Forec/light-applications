# Monitor Recorder
> This project is a tool similar with those programs executing by monitors. Monitoring somewhere, and recording what happened when actions detected.

## Platform
* I wrote these codes with VS Ultimate 2013, version `12.0.21005.1 REL`.
* To compile these codes, you need to install `OpenCV`, and configure them in your Visual Studio. Download `OpenCV` from [here](http://opencv.org/downloads.html), make sure the version you downloaded. The steps are:
 * Add `<OPENCV_PATH>\build\x64\vc12\bin` to your environment path.
 * Add `<OPENCV_PATH>\build\include`, `<OPENCV_PATH>\build\include\opencv` and `<OPENCV_PATH>\build\include\opencv2` to your VS including path. Right click the project and select `Settings` or `Attributes`, the including path is in `VC++ path`.
 * Add all files under `<OPENCV_PATH>\build\x64\vc12\lib` to your link inputs. That could be found in `LINK` -> `INPUT`. You can also separate the files into two parts by whether having suffix `d`, and add those files with suffix `d` into your `DEBUG` mode, add others into `RELEASE` mode.

## EXPLANATION
* This tool can build your camera capable devices into monitors. It can monitor a place, and detects whether there are unnormal actions. If it detects actions, those actions will be recorded into an `.avi` file.

## Usage
You can download the executable file from [here](http://7xktmz.com1.z0.glb.clouddn.com/MonitorRecorder.exe).
* The tool will starts after 2s. You need to make sure that when it starts, the first frame it captured is the background you want to monitor. If you don't specify the `VIS` option, the monitor window won't appear and it will just run background.
* Run `MonitorRecorder.exe --help` to see how to use. The copy of help information is below. You can run `MonitorRecorder -vis -mog2 24 10 test.avi` to have a try. This command will show what the camera monitors with MOG2 mode and records with fps=24, and will starts recording after 10 frames of unnormal actions detected. The recording file is `test.avi`. The view of MOG2 mode is put in Example.
```
----------------------------------------------------------------------------
Usage:
 ./MonitorRecorder.exe [VIS] [MODE] [FPS] [THRESHOLD] [OUTPUTFILE]
   [VIS]  : use -vis to show the monitor window, or it will run background.
   [MODE] : -src   shows the original frame;
            -mog1       shows the MOG frame;
            -mog2      shows the MOG2 frame.
   [FPS]  : set the fps of record file, default is 24.
   [THRESHOLD]
          : set the number x that the monitor will start recording after
            x unnormal frames passed.
   [OUTPUTFILE]
          : assign the output recording file. It must be .avi format.
----------------------------------------------------------------------------
```

## Example
* An screenshot for the command `MonitorRecorder -vis -mog2 24 10 test.avi`.   
<img src="http://7xktmz.com1.z0.glb.clouddn.com/monitor-recorder.png" width = 400/>

## Update-logs
* 2016-10-05: Add this project.
* 2016-10-06: Add license.