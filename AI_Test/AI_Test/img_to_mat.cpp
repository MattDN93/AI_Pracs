/* Application detail:		ENEL4AI
							Artificial Intelligence
	For:					Practical 1
	Purpose:				Finding Connected Components
	Student:				Matthew de Neef
							212503024

	Description:			This program receives an image in BMP, JPG or PNG format
							and returns the # connected components in the image for
							each of the connected components, also finds:
							*7 invariant moments
							*Centroid of images
							*Perimeter and surface area in pixels
	Prerequisites:			VC++ redist files for OpenCV
							This code also available on http://github.com/mattdn93/AI_Pracs
*/

#include "CC_detect.h"	//user-defined header with classes and var defs

using namespace cv;		//for ease using openCV namespace
						//stdlib fns will be ref'd explicitly

//----------------------time-keeping----------------

//Reference https ://www.physicsforums.com/threads/measuring-time-elapsed-in-c.224989/
double diffclock(clock_t clock1, clock_t clock2)
{
	double diffticks = clock1 - clock2;
	double diffms = (diffticks*1000) / CLOCKS_PER_SEC;
	return diffms;
}

int main(int argc)
{
	std::cout << "Artificial Intelligence Prac 1\nMatthew de Neef\n212503024\n" << std::endl;
	while (newImage == true)	//if user wants to keep adding images
	{
		fflush(stdin);
		filePath == "\0";
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


		//time taken to load & transform the image START
		clock_t begin_load_and_img_transform = clock();

		//create an output file
		std::ofstream output("ConnectedCompoData.txt");

		//----------------image processing to matrix with OPENCV-----------------
		/*instantiate class object
		CC connectedCompo;*/

		//else we assume file opening OK...

		//convert the image matrix to greyscale (basically resize it)
		Mat inputImgGreyScale(inputImgMat.size(), CV_8UC1);

		//convert the image matrix to a type of matrix defined by the greyscale matrix
		cvtColor(inputImgMat, inputImgGreyScale, CV_BGR2GRAY);

		//binary matrix of values from image
		Mat binaryMat(inputImgGreyScale.size(), inputImgGreyScale.type());

		//apply threshold values (everything either goes to 0 or 255)
		threshold(inputImgGreyScale, binaryMat, 50, 255, cv::THRESH_BINARY);

		if (1)
		{
			//display the results using OCV's window render
			namedWindow("Binary matrix", WINDOW_NORMAL);
			imshow("Binary Matrix", binaryMat);
		}
		//end timing for loading image!
		clock_t end_load_and_img_transform = clock();
		double load_time = double(diffclock(end_load_and_img_transform, begin_load_and_img_transform));


		//create a copy of the image matrix to mark completed pixels
		//the dimensions are copied but its set to FALSE
		tempProcMat = (Mat_<uchar>(binaryMat.rows, binaryMat.cols));
		tempProcMat = Mat::zeros(binaryMat.rows, binaryMat.cols, CV_32F);
		//DEBUG!

		int pixArray[250][250];

		//time taken to find Connected components START
		clock_t begin_find_cc_and_params = clock();

		//resize it to the same size as the image matrix
		procMat.resize(binaryMat.cols + 2, std::vector<bool>(binaryMat.rows + 2, false));

		//-------------------------start processing connected components----------
		//DEBUG
		if (DEBUG) {
			for (int i = 0; i < binaryMat.rows; i++)
			{
				//row_ptr = binaryMat.ptr<uchar>(i);		//get the current row
				for (int j = 0; j < binaryMat.cols; j++)
				{
					pixArray[i][j] = binaryMat.at<uchar>(i, j);
					std::cout << pixArray[i][j];
				}
			}
		}

		//this list is dynamically allocated; it holds all the connected component objects found
		std::list<CC> listOfConnectedCompos;

		uchar *row_ptr = NULL;
		for (int i = 1; i < binaryMat.rows; i++)
		{
			//row_ptr = binaryMat.ptr<uchar>(i);		//get the current row
			for (int j = 1; j < binaryMat.cols; j++)
			{
				//look for an initial unprocessed pixel?
				if (binaryMat.at<uchar>(i, j) == 0)
				{
					if (procMat[i][j] == false)
					{
						//instantiated object of CC class
						CC connectedCompo;
						//run DFS to find the connected components
						findCC(&binaryMat, &tempProcMat, i, binaryMat.rows, j, &connectedCompo);
						//if found, add to the list of CCs at the front
						listOfConnectedCompos.push_front(connectedCompo);
						if (DEBUG) {
							std::cout << "\n Connected Component Found!" << std::endl;
						}
					}

				}
			}
		}
		//DEBUG!
		if (1)
		{
			output << binaryMat;
		}

		//--------------perform the calculation for invariat moments----


		//--------------output result to file------------------
		std::cout << "\n\nDone. Outputting to file....." << std::endl;
		std::cout << "\nA total of " << listOfConnectedCompos.size() << " connected components found." << std::endl;

		//initialise a totalsize counter since we are popping off a list and the size will reduce
		int total_size = 0;
		total_size = listOfConnectedCompos.size();
		for (int i = 0; i < total_size; i++)
		{
			output << "----------Component #" << i + 1 << "-------------" << std::endl;
			//using overloaded stream operator to print all details out
			(listOfConnectedCompos.front()).getInvariantMoments();
			output << listOfConnectedCompos.front() << std::endl;
			listOfConnectedCompos.pop_front();
		}

		clock_t end_find_cc_and_params = clock();
		//find time taken to perform calculations
		double proc_time = double(diffclock(end_find_cc_and_params, begin_find_cc_and_params));

		//Write timing results to file and screen
		std::cout << "\n----------Timing analysis-------------" << std::endl;
		output << "\n----------Timing analysis-------------" << std::endl;
		std::cout << "\n Time to load & parse image with OpenCV: " << load_time << " ms." << std::endl;
		output << "\n Time to load & parse image with OpenCV: " << load_time << " ms." << std::endl;

		std::cout << "\n Time to find parameters and CC: " << proc_time << " ms." << std::endl;
		output << "\n Time to find parameters and CC: " << proc_time << " ms." << std::endl;

		//system("pause");
		waitKey(1000);
		char response;
		std::cout << "\n Process another image? (Y/N)";
		std::cin >> response;
		if (response == 'Y' || response == 'y')
		{
			output.close();
			
			newImage = true;
			fileSuccess = false;
		}
		else {
			//close file
			output.close();
			return 0;
		}
	}


}

//method to execute nested DFS on the matrix
void findCC(Mat *pixelMat,Mat *progressMat, int row, int rowSize, int col, CC *currCompo)
{
	//the index of the current pixel in the array, treats the matrix as "wraparound"
	bool onPerim = false;

	//mark the current pixel as visited (true)
	//(*progressMat).at<uchar>(row, col) = 1;
	procMat[row][col] = true;
	//check all 8 adjacent pixels! e.g. finding Xs about A
	/*	row-1,col-1	->	X	X	X
						X	A	X
						X	X	X <- row+1,col+1
	*/
	for (int adjX = row -1; adjX <= row +1 ; adjX++)
	{
		for (int adjY = col - 1; adjY <= col + 1; adjY++)
		{
			if (DEBUG) {
			std::cout << "\n Pixel:[";
			std::cout << (*pixelMat).at<uchar>(adjX, adjY);
			std::cout << "]";
			}
			//if we have found a background pixel (0=foreground, 255=background)
			if ((*pixelMat).at<uchar>(adjX,adjY) == 255)
			{
				//if this pixel X is b/g and A is f/g it means X is on perimeter!
				onPerim = true;
			}
	//		//else run a recursive DFS on the next adjacent pixel that HAS NOT been recorded!
			if (((*pixelMat).at<uchar>(adjX, adjY) == 0))
			{
				if (procMat[adjX][adjY]==false)
				{
					if (DEBUG) {
						std::cout << "doing DFS" << std::endl;
					}
					findCC(pixelMat, progressMat, adjX, rowSize, adjY, currCompo);
				}
				
			}
			
		}

	}



	//once DFS is done, we can add the pixel to the list of connected components
	(*currCompo).addPixToCC(row, col, onPerim);
}

std::ostream &operator<< (std::ostream &outFile, const CC c)
{
	outFile << "\nThe Centroid is: [" << c.centroid[x_bar] << "," << c.centroid[y_bar] << "]" << std::endl;

	outFile << "\nThe invariant Moments are:\n ";
	for (int i = 0; i < 7; i++)
	{
		outFile << "<phi_"<<i<<">: ";
		outFile << c.invarMoment[i] << ", " << std::endl;
	}
		outFile << "\nThe Surface Area is: " << c.ccPixels.size() << " pixels."<<std::endl;
		outFile << "\nThe Perimeter is: " << c.perimeter<<" pixels."<< std::endl;

		return outFile;
	
}
