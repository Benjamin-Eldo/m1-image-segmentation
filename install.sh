#!/bin/bash
# This script will setup the openCV2 lib for C++.

#Install for every prerequisites
sudo apt update;
sudo apt-get install g++;
sudo apt-get install cmake;
sudo apt-get install make;
sudo apt-get install wget;
sudo apt-get install unzip;
sudo apt-get install libgtk2.0-dev;
sudo apt-get install pkg-config;
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev;
sudo apt-get install ffmpeg;

wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip;
unzip opencv.zip;
mv opencv-4.x opencv;
mkdir -p build && cd build;
cmake -DWITH_FFMPEG=ON -DWITH_OPENMP=ON -DWITH_CUDA=OFF ../opencv
make -j4;
sudo make install -j6;
cd ../;
cd src;
cmake .;

#Cleaning remains
cd ../
rm opencv.zip
