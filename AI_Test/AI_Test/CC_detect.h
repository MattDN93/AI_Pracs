#pragma once

#pragma region Includes
#include <stdio.h>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
#pragma endregion

#pragma region Variable Defs

//For file management
std::string filePath;		//name of file path
std::string quitStr = "quit";
bool fileSuccess = false;			//whether file opened OK

//OpenCV vars
Mat inputImgMat;			//input matrix from image
#pragma endregion
