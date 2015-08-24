/*		ENEL4AI
		Basic Image -> Matrix Component
		Matthew de Neef
		212503024
*/

#include "CC_detect.h"	//user-defined header with classes and var defs

using namespace cv;		//for ease using openCV namespace
						//stdlib fns will be ref'd explicitly

/* The user calls this exe with arguments for the filename*/

int main(int argc)
{
	std::cout << "Artificial Intelligence Prac 1\nMatthew de Neef\n212503024\n" << std::endl;

	while (fileSuccess == false)		//while there's an error opening the file, keep asking
	{
		/*Request filename from user*/

		std::cout << "\nPlease enter a file path to open, fully qualified. (Type 'quit' to quit): " << std::endl;
		std::getline(std::cin, filePath);

		/*use CV libraries to deal with image
		imread function takes any colour depth image with filename in argv
		*/
		
		inputImgMat = imread(filePath, CV_LOAD_IMAGE_COLOR); //loads up image in grayscale format
		if (inputImgMat.empty())
		{
			std::cout << "\n-------------------------------------------" << std::endl;
			std::cerr << "\nError. Image filename '" << filePath << "' is invalid or could not be opened.\n Try again." << std::endl;
			std::cout << "\n-------------------------------------------" << std::endl;
			fileSuccess = false;
		}
		else {
			fileSuccess = true;
		}
	}
	
	//else we assume file opening OK...

	//convert the image matrix to greyscale (basically resize it)
	Mat inputImgGreyScale(inputImgMat.size(), CV_8U);

	//convert the image matrix to a type of matrix defined by the greyscale matrix
	cvtColor(inputImgMat, inputImgGreyScale, CV_BGR2GRAY);

	//binary matrix of values from image
	Mat binaryMat(inputImgGreyScale.size(), inputImgGreyScale.type());

	//apply threshold values (everything either goes to 0 or 255)
	threshold(inputImgGreyScale, binaryMat, 100, 255, cv::THRESH_BINARY);

	//display the results using OCV's window render
	namedWindow("Binary matrix", WINDOW_AUTOSIZE);
	imshow("Binary Matrix", binaryMat);

	std::cout << binaryMat /255;

	std::cout << "\n\nDone. Press any key to quit." << std::endl;

	waitKey(0);

	return 0;
}