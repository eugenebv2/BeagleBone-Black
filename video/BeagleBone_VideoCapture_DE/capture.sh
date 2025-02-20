v4l2-ctl --set-ctrl=brightness=45
./video_capture -f -c 30 -o > output.raw
ffmpeg -video_size 320x240 -pixel_format yuv420p -i output.raw  -preset slow  output.mp4
