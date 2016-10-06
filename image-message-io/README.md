# Image Message IO （图片信息读写）
> This project is a small package for **writing messages into images to avoid detection**. You can also regard it as Steganography.

## Platform
This small script is written in python 3.5, python 2.7 is supported too, using `PIL`. You can install `PIL` by executing an executable binary file from [here](http://effbot.org/media/downloads/PIL-1.1.7.win32-py2.7.exe) if you use Python 2.7, otherwise you may need to install `pillow` instead of `PIL` by `pip3 install pillow`.

## Usage
The file `image_message_io.py` declares two classes.
* **`ImageMessageWriter`** : This class is used to encode your message into an image. It has several public methods, list below.
 * `loadImage(image_or_path)`: load an image into its memory. The parameter can be an `Image.Image` class or just a path to the image file.
 * `encode(message)` : to use this method, make sure you have loaded an image before, otherwise it will raise an exception. The image after encoding will be saved in the class. You can use method `saveImage` to save it to disk.
 * `saveImage(filepath)` : save the encoded image to disk. If you haven't encoded, an exception will be raised.

* **`ImageMessageReader`** : This class is used to decode your message from an image. It has two public methods, list below.
 * `loadImage(image_or_path)` : same to what it is in `ImageMessageWriter`.
 * `decode()` : make sure you have loaded an image before using this method. It will return a string, which is the message hiding in the image.
 * `getMessage()` : return the last message decoded. If you haven't decoded yet, exception will be raised.

## Example
An example is given in `image_message_io.py`.

## Update-logs
* 2016-9-30: Add this project.
* 2016-10-6: Build repository.