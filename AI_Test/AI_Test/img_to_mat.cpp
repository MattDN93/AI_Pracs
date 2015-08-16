/*		ENEL4AI
		Basic Image -> Matrix Component
		Matthew de Neef
		212503024
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;		//for ease using openCV namespace
						//stdlib fns will be ref'd explicitly

/* The user calls this exe with arguments for the filename*/

int main(int argc, char **argv)
{
	if (argc != 2) //if the user spec'd argument is wrong
	{
		std::cout << "Syntax error. Use '" << argv[0] << " <path_to_file>. Try again." << std::endl;
		return 1;	//throw a syntax error and quit
	}


	/* Expect argument as "img_to_mat.exe C:\my\directory\filename.jpg" */

	/*use CV libraries to deal with image
		imread function takes any colour depth image with filename in argv
	*/

	Mat inputImgMat = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	if (inputImgMat.empty())
	{
		std::cerr << "Error. Image filename '" << argv[1] << "' is invalid or could not be opened." << std::endl;
		return 1;
	}

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

	std::cout << binaryMat;

	std::cout << "Done. Press any key to quit." << std::endl;

	waitKey(0);

	return 0;
}