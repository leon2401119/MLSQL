#!/bin/bash
javac Parser.java
java Parser config
g++ -shared -fPIC plugin.cpp myaug.cpp -o plugin.so -I /usr/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
sudo cp plugin.so /usr/lib/mysql/plugin/plugin.so
systemctl restart mysql
python3 plug_func.py
