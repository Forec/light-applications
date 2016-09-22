# GPA-Calculator
> This project is a script for calculating **GPA** automatically, you just need to input your username and password, even CAPTCHA is not needed, the script can deal with CAPTCHA itself.

## Platform
This small script is written in python 2.7, using several packages, list below.
* `urllib` , `urllib2`, `re`, `cookielib` and `sys` are built-in packages, once you install python 2.7, you can import them without any other actions.
* **`BeautifulSoup`** : I use this package to parse html files, you can install this package by this command: **`pip install beautifulsoup`** .
* **`pytesseract`** : I use this package to deal with CAPTCHA. `pytesseract` is a simple package depending on `PIL` and `tesseract-ocr`, even you have installed `pytesseract`, the two depended packages need to be installed too. You can first intall `pytesseract` by **`pip install pytesseract`** . After that, **you need to install `PIL`** by executing an executable binary file from [here](http://effbot.org/media/downloads/PIL-1.1.7.win32-py2.7.exe). At last, **you need to install `tesseract-ocr`**, this project can be found in github at [here](https://github.com/tesseract-ocr/tesseract), however, its wiki has announced that  no official Windows installer for newer versions. So you can download old versions from [Sourceforge](https://sourceforge.net/projects/tesseract-ocr-alt/files/). Choose your version. 
* **Attentions**: Without `PIL` and `tesseract-ocr`, the script cannot automatically fill in the CAPTCHA for you. You may need to modify my script and verify the CAPTCHA yourself. Another very import notice: **THE CODEC I USE IS GBK**, change it yourself if your terminal is utf8 or other codecs.

## Usage
The python version is 2.7, so just `python gpa.py <id> <pwd>`, the script will return your gpa. Also, I build an executable binary file, you can just download `gpa.exe` from my cloud storage: [here](http://7xktmz.com1.z0.glb.clouddn.com/gpa.exe). It will look like this, for BUPT students, optional classes in the evening will not be counted into gpa. Here I hide my Username, Password and Scores.  
<img src="http://7xktmz.com1.z0.glb.clouddn.com/gpa-get.png" width="500px">

## Update-logs
* 2016-9-18: Add `gpa.py`.