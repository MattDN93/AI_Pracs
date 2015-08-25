#pragma once

#pragma region Includes
#include <stdio.h>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <iostream>

//for centroid storage
#define x_bar 0
#define y_bar 1

using namespace cv;
#pragma endregion

#pragma region Variable Defs

//For file management
std::string filePath;		//name of file path
std::string quitStr = "quit";
bool fileSuccess = false;			//whether file opened OK

//OpenCV vars
Mat inputImgMat;			//input matrix from image
Mat& ScanImageAndReduce(Mat& I);
#pragma endregion

#pragma region CC Class & Function Defs
//This class holds connected component methods and variables
class CC
{
public:
	//default constructor/destructor
	CC();
	~CC();
	//-------------pixel edit tool-----------
	//matrix of pixel points
	Mat pixPoints;
	//adds a given scanned pixel to the CC
	void addPixToCC(int x, int y, bool onPerim);

	//returns surface area /perimeter of CC
	int findSurfArea();
	int findPerimeter();
	//-------------get accessors-----------
	//returns pointer to centroids array
	float *getCentroid();
	//returns pointer to invMoments array
	float *getInvariantMoments();
	//returns 2D points from Mat object
	Mat getPoints();

	
private:
	//-------------pixel structure---------------------------
	//The pixel structure allows for marking perimeter pixels on-the-fly
	struct pix
	{
		int x;			//x-coord
		int y;			//y-coord
		bool perim;		//part of perimeter?
	};
	//-------------vector of structs-------------------------
	//vector allows for structs of each pixel+co-ord to be stores
	//memory allocation is managed automatically
	std::vector<pix> ccPixels;
	std::vector<Point2f> points;
	//-------------centroids and invariant moments-----------
	//invariant moments of connected components (0-6 = 7 moments)
	float invarMoment[7];
	//x and y centroid of each component [0] = x; [1]=y;
	float centroid[2];
	//-------------calculation methods-----------------------
	//<M>_pq; is the two-dimensional moment of the function f(x,y
	float calc2dMoments(int p, int q);
	
	//Calculate connected compo centroid
	void findCentroid();
	
	//<mu>_pq; Having found centroids, calc central moment
	float findCentralMoments(int p, int q);
	
	//<neta>_pq; The moments are further normalised for the effects of change of scale using the following formula
	float findNormalisedCentroidMoments(int p, int q);
	
	//find 7 invariant moments to write to invarMoment[] returns TRUE/FALSE based on result
	bool findInvarMoments();
	//-------------parameters and flags-----------------------
	//normalised central moments uses this variable each time
	float mu_00 = 0;
	//Are centroids found?
	bool centroids_found = false;
	//Is <mu>_00 found?
	bool mu_00_found = false;
};

CC::CC()
{
}

CC::~CC()
{
}

inline void CC::addPixToCC(int x, int y, bool onPerim)
{
}

inline int CC::findSurfArea()
{
	return 0;
}

inline int CC::findPerimeter()
{
	return 0;
}

inline float * CC::getCentroid()
{
	return NULL;
}

inline float * CC::getInvariantMoments()
{
	//to find the 7 invariant moments, we must have centroids
	if (centroids_found == false) {
		findCentroid();
	}

	//mu_00 is used in the neta_pq calculation to the power gamma
	//we may find this value once and re-use it each calculation
	mu_00 = findCentralMoments(0, 0);
	mu_00_found = true;

	//we execute finding invariant moments
	if (findInvarMoments() == false) {
		if (ccPixels.empty())
		{
			//we have an empty image so throw an error
			std::cerr << "ERROR: The component image is empty!" << std::endl;
		}
		else {
			std::cerr << "ERROR: Can't call invariant moment calculation function." << std::endl;
		}
	}
	return invarMoment;
}

inline Mat CC::getPoints()
{
	pixPoints = Mat(points);
	return pixPoints;
}

inline float CC::calc2dMoments(int p, int q)
{
	/*summation given by:
	(SIGMA_X)(SIGMA_Y)x^p * y^q */

	float summation;
	for (int i = 0; i < ccPixels.size(); i++)
	{
		summation += pow(ccPixels[i].x, p) * pow(ccPixels[i].y, q);
	}
	return summation; //return M_pq, 2D moment
}

inline void CC::findCentroid()
{
	//find the x and y centroids of the image
	centroid[x_bar] = calc2dMoments(1, 0) / calc2dMoments(0, 0);
	centroid[y_bar] = calc2dMoments(0, 1) / calc2dMoments(0, 0);

	centroids_found = true;	//we hae found the centroids
}

inline float CC::findCentralMoments(int p, int q)
{
	float summation;
	for (int i = 0; i < ccPixels.size(); i++)
	{
		summation += pow(ccPixels[i].x - centroid[x_bar], p) * pow(ccPixels[i].y - centroid[y_bar], q);
	}
	return summation; //return <mu>_pq, central moments
}

inline float CC::findNormalisedCentroidMoments(int p, int q)
{
	/*The moments are further normalised for the effects of 
	change of scale using the following formula:

	neta_pq = (mu_pq / (mu_00)^gamma)
	gamma = normalization factor
	
 */

	float gamma = (p + q / 2) + 1;
	float neta_pq;
	neta_pq = findCentralMoments(p, q) / pow(mu_00, gamma);
	return neta_pq;
}

inline bool CC::findInvarMoments()
{
	//using Hu's formula for invariant moments
	//if any of the variables haven't been set, or image is empty, throw an error.
	if (ccPixels.empty() || !mu_00_found || !centroids_found)
	{
		return false;
	}
	else {
		//first fine neta values with normalized invariant moment formula
		float neta_02 = findNormalisedCentroidMoments(0, 2);
		float neta_20 = findNormalisedCentroidMoments(2, 0);
		float neta_03 = findNormalisedCentroidMoments(0, 3);
		float neta_30 = findNormalisedCentroidMoments(3, 0);
		float neta_12 = findNormalisedCentroidMoments(1, 2);
		float neta_21 = findNormalisedCentroidMoments(2, 1);
		float neta_11 = findNormalisedCentroidMoments(1, 1);

		//the 7 invariant moments as defined by Hu
		invarMoment[0] = neta_20 + neta_02;
		invarMoment[1] = pow((neta_20 - neta_02), 2) + 4*pow(neta_11, 2);
		invarMoment[2] = pow((neta_30 + (3*neta_12)), 2) + pow((neta_03 + (3*neta_21)), 2);
		invarMoment[3] = pow((neta_30 + neta_12), 2) + pow((neta_03 + neta_21), 2);
		
		invarMoment[4] = (neta_30 - 3 * neta_12) * (neta_30 + neta_12) *
			(pow(neta_30 + neta_12, 2) - 3 * pow(neta_21 - neta_03, 2)) +
			(3 * neta_21 - neta_03) * (neta_21 + neta_03) *
			(3 * pow(neta_30 + neta_12, 2) - pow(neta_21 - neta_03, 2));

		invarMoment[5] = (neta_20 - neta_02) * (pow(neta_30 + neta_12, 2) -
			pow(neta_21 + neta_03, 2)) + 4 * neta_11 *
			(neta_30 + neta_12) * (neta_21 + neta_03);
		
		invarMoment[6] = (3 * neta_21 - neta_03) * (neta_30 + neta_12) *
			(pow(neta_30 + neta_12, 2) - 3 * pow(neta_21 + neta_03, 2)) -
			(neta_30 + 3 * neta_12) * (neta_21 + neta_03) *
			(3 * pow(neta_30 + neta_12, 2) - pow(neta_21 + neta_03, 2));

		return true;
	}
	
}



#pragma endregion