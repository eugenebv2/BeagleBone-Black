BeagleBone_VideoCapture_DE
======

Beaglebone video capture example Repository using the Logitech C310.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that source code redistributions retain this notice.

This software comes with no warranties of any type. 

The description and modifications of the code presented here is on the website: www.deeplyembedded.org.

Demo video for the working of this source code are retrieved from https://youtu.be/mVBuAQ1twXc

Please find more information on this video at:http://deeplyembedded.org/capture-videos-using-beaglebone/  


# BeagleBone_VideoCapture_DE

sudo apt install pkg-config
sudo apt install v4l-utils
sudo apt install libv4l-dev
sudo apt install ffmpeg

v4l2-ctl –list-devices
v4l2-ctl --all

v4l2-ctl --set-ctrl=brightness=50
v4l2-ctl --set-ctrl=contrast=50
v4l2-ctl --set-ctrl=white_balance_automatic=4

./video_capture -f -c 150 -o > output.raw
ffmpeg -video_size 320x240 -pixel_format yuv420p -i output.raw  -preset slow  output.mp4


ffmpeg -video_size 320x240 -pixel_format yuv420p -i output.raw output.jpg