# Real-time Filtering
The Real-time Filtering project is a Cpp-based application that focuses on processing and analyzing live video streams. It leverages the power of OpenCV and various custom filters to manipulate images in real-time.

Functionality
The project extracts frames from a live video feed, applies a series of filters to each frame, and then displays the filtered frames in real-time. This process is facilitated by multithreading to ensure smooth performance and avoid any potential lag.

Here are the filters used in the project and what they do:

**Grayscale Filter**: This filter converts the colored image into shades of gray. It does this by removing the hue and saturation information while retaining the luminance.

<img src="https://github.com/SyntaxButcher/Real-time-Filtering/blob/main/Results/Grayscale.png" width="30%" height="30%">

**Gaussian Blur Filter**: This filter is used to blur the image, reducing its high-frequency components. The name comes from the Gaussian kernel, which is used to create the effect. It can help in noise reduction and is often used in reducing detail and noise, and preparing an image for further processing.

<img src="https://github.com/SyntaxButcher/Real-time-Filtering/blob/main/Results/Blur.png" width="30%" height="30%">

**Edge Detection Filter**: This filter is designed to detect the edges within an image. It works by identifying the points in an image where the brightness changes sharply or has discontinuities.

<img src="https://github.com/SyntaxButcher/Real-time-Filtering/blob/main/Results/Magnitude.png" width="30%" height="30%">

**Blur and Quantize Filter**: This filter first applies a blur effect to soften the image by averaging the pixel values of surrounding pixels. It then applies a quantization process to reduce the number of distinct colors in the image, thus creating a stylized, poster-like effect.

<img src="https://github.com/SyntaxButcher/Real-time-Filtering/blob/main/Results/BlurQuantize.png" width="30%" height="30%">

**Cartoonize Filter**: This filter sets each pixel in the image to black if it's less than some fixed value (the threshold) or to quantized version if it's more.

<img src="https://github.com/SyntaxButcher/Real-time-Filtering/blob/main/Results/Cartoonize.png" width="30%" height="30%">


The order of these filters can be modified, and new filters can be added as per the requirement. The goal of this project is to create a flexible, extensible system for real-time video filtering.

Requirements
Cpp
OpenCV library

Usage
Run main.cpp in your Python environment. Make sure your webcam is working properly, as it will provide the video stream for filtering.


 
