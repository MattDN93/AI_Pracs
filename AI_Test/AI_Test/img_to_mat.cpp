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

	//----------------file management and opening image-----------------
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

	//create an output file
	std::ofstream output("ConnectedCompoData.txt"); 

	//----------------image processing to matrix with OPENCV-----------------
	/*instantiate class object
	CC connectedCompo;*/
	
	//else we assume file opening OK...

	//convert the image matrix to greyscale (basically resize it)
	Mat inputImgGreyScale(inputImgMat.size(), CV_8U);

	//convert the image matrix to a type of matrix defined by the greyscale matrix
	cvtColor(inputImgMat, inputImgGreyScale, CV_BGR2GRAY);

	//binary matrix of values from image
	Mat binaryMat(inputImgGreyScale.size(), inputImgGreyScale.type());

	//apply threshold values (everything either goes to 0 or 255)
	threshold(inputImgGreyScale, binaryMat, 50, 255, cv::THRESH_BINARY);

	//display the results using OCV's window render
	namedWindow("Binary matrix", WINDOW_AUTOSIZE);
	imshow("Binary Matrix", binaryMat);

	//create a copy of the image matrix to mark completed pixels
	//the dimensions are copied but its set to FALSE
	tempProcMat = (Mat_<bool>(binaryMat.rows, binaryMat.cols)==false);
	//DEBUG!
	//tempProcMat = binaryMat.clone();

	//-------------------------start processing connected components----------
	//this list is dynamically allocated; it holds all the connected component objects found
	std::list<CC> listOfConnectedCompos;

	uchar *row_ptr = NULL;
	for (int i = 0; i < binaryMat.rows; i++)
	{
		//row_ptr = binaryMat.ptr<uchar>(i);		//get the current row
		for (int j = 0; j < binaryMat.cols; j++)
		{
			//look for an initial unprocessed pixel?
			if (binaryMat.at<int>(i,j) == 0)
			{
				//instantiated object of CC class
				CC connectedCompo;
				//run DFS to find the connected components
				findCC(binaryMat,tempProcMat, i, binaryMat.rows, j, &connectedCompo);
				//if found, add to the list of CCs at the front
				listOfConnectedCompos.push_front(connectedCompo);
				
				std::cout << "\n Connected Component Found!" << std::endl;
			}
		}
	}
	//DEBUG!
	if (DEBUG)
	{
		std::cout << binaryMat;
	}

	//Mat newBinMat = ScanImageAndReduce(binaryMat);
	//int i, j;
	//for (int i = 0; i < binaryMat.rows; i++)
	//{
	//	for (int j = 0; j < binaryMat.cols; j++)
	//	{
	//		binaryMat.at()
	//	}
	//}

	//--------------perform the calculation for invariat moments----
	

	//--------------output result to file------------------
	std::cout << "\n\nDone. Outputting to file....." << std::endl;
	std::cout << "\nA total of " << listOfConnectedCompos.size() << " connected components found." << std::endl;


	for (int i = 0; i < listOfConnectedCompos.size(); i++)
	{
		output << "----------Component #" << i + 1 << "-------------" << std::endl;
		//using overloaded stream operator to print all details out
		(listOfConnectedCompos.front()).getInvariantMoments();
		output << listOfConnectedCompos.front() << std::endl;
		listOfConnectedCompos.pop_front();
	}


	waitKey(0);
	//close file
	output.close();
	return 0;
}

//method to execute nested DFS on the matrix
void findCC(Mat& pixelMat,Mat& progressMat, int row, int rowSize, int col, CC *currCompo)
{
	//the index of the current pixel in the array, treats the matrix as "wraparound"
	bool onPerim = false;

	//mark the current pixel as visited (true)
	progressMat.at<bool>(row, col) = true;

	//check all 8 adjacent pixels! e.g. finding Xs about A
	/*	row-1,col-1	->	X	X	X
						X	A	X
						X	X	X <- row+1,col+1
	*/
	for (int adjX = row -1; adjX < row +1 ; adjX++)
	{
		for (int adjY = col -1; adjY < col+1; adjY++)
		{
			//if we have found a background pixel (0=foreground, 255=background)
			if (pixelMat.at<uchar>(adjX,adjY) == '255')
			{
				//if this pixel X is b/g and A is f/g it means X is on perimeter!
				onPerim = true;
			}
			//else run a recursive DFS on the next adjacent pixel that HAS NOT been recorded!
			if ((pixelMat.at<uchar>(adjX, adjY) == '0') && (progressMat.at<bool>(adjX,adjY) == false))
			{
				findCC(pixelMat, progressMat, adjX, rowSize, adjY, currCompo);
			}
			
		}

	}

	//once DFS is done, we can add the pixel to the list of connected components
	(*currCompo).addPixToCC(row, col, onPerim);
}

std::ostream &operator<< (std::ostream &outFile, const CC c)
{
	outFile << "\nThe Centroid is: [" << c.centroid[x_bar] << "," << c.centroid[y_bar] << "]" << std::endl;

	outFile << "\nThe invariant Moments are: ";
	for (int i = 0; i < 7; i++)
	{
		outFile << c.invarMoment[i] << ", " << std::endl;
	}
		outFile << "\nThe Surface Area is: " << c.ccPixels.size() << std::endl;
		outFile << "\nThe Perimeter is: " << c.perimeter<< std::endl;

		return outFile;
	
}
