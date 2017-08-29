//#pragma once
#include <cv.h>  
#include <cxcore.h>
#include <highgui.h>
#include <cxcore.hpp>

using namespace cv;
using namespace std;

class WaveTransform
{
public:
	WaveTransform(){};
	~WaveTransform(){};

	Mat WDT(const Mat &_src, const string _wname, const int _level);

	Mat IWDT(const Mat &_src, const string _wname, const int _level);

	void wavelet(const string _wname, Mat &_lowFilter, Mat &_highFilter);

	Mat waveletDecompose(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter);

	Mat waveletReconstruct(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter);

	Mat WFR(const Mat srcArray[], const int _level);



private:

};
