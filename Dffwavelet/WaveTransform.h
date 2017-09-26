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

	void wavelet(const string _wname,const int flag, Mat &_lowFilter, Mat &_highFilter);

	Mat waveletDecompose(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter);

	Mat waveletReconstruct(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter);

	Mat WFR(const Vector<Mat> srcVector, const int _level);

	float maxFun(const Mat src1, const Mat src2, const Mat src3, const Mat src4, const Mat src5, int row,int col);


private:

};
