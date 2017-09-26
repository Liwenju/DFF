
#include "WaveTransform.h"
#include <cv.h>  
#include <cxcore.h>
#include <highgui.h>
#include <cxcore.hpp>

using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
	WaveTransform m_waveTransform;
	/*
	Mat src1, src2, src3, src4, src5;//源图矩阵系列
	vector<Mat> srcImg;//源图向量
	srcImg.clear();//矢量初始化
	string imgPath = "G:\\AtestImage\\";//源图目录
	char xx[10];//临时字符数组

	for (int i = 1; i <= 5; i++)
	{
		sprintf(xx, "%d", i);
		string s1(xx);
		string s2(".bmp");
		string s3 = imgPath + s1 + s2;
		Mat src_temp = imread(s3, 0);
		srcImg.push_back(src_temp.clone());
	}

	Mat imgFusion = m_waveTransform.WFR(srcImg,3);
	Mat _imgSor = Mat::zeros(imgFusion.rows, imgFusion.cols, CV_8UC1);
	normalize(imgFusion, imgFusion, 1.0, 0.0, NORM_MINMAX);//归一到0~1之间
	imgFusion.convertTo(_imgSor, CV_8UC1, 255, 0); //转换为0~255之间的整数
	imshow("sor", _imgSor);
	
	*/
	/*
	Mat a = (Mat_<float>(1, 8) << 6, 4, 8, 7, 5, 9, 3, 2);
	Mat low1 = (Mat_<float>(1, 2) << 1 / sqrtf(2), 1 / sqrtf(2));
	Mat high1 = (Mat_<float>(1, 2) << 1 / sqrtf(2), -1 / sqrtf(2));
	Mat dst1 = Mat::zeros(1, 8, CV_32FC1);
	Mat dst2 = Mat::zeros(1, 8, CV_32FC1);
	filter2D(a, dst1, -1, low1);
	filter2D(a, dst2, -1, high1,Point(-1,-1));


	Mat downDst1 = Mat::zeros(1, 8, CV_32FC1);
	Mat downDst2 = Mat::zeros(1, 8, CV_32FC1);

	for (int i = 0, cnt = 1; i<4; i++, cnt += 2)
	{
		downDst1.at<float>(0, cnt) = dst1.at<float>(0, cnt);
		downDst2.at<float>(0, cnt) = dst2.at<float>(0, cnt);
	}


	Mat low2 = (Mat_<float>(1, 2) << 1, 1);
	Mat high2 = (Mat_<float>(1, 2) << 1, -1);

	Mat downDst3 = Mat::zeros(1, 8, CV_32FC1);
	Mat downDst4 = Mat::zeros(1, 8, CV_32FC1);

	for (int i = 0; i <8; i++)
	{
		downDst3.at<float>(0, i) = downDst1.at<float>(0, 7 - i);
		downDst4.at<float>(0, i) = downDst2.at<float>(0, 7 - i);
	}

	Mat a1 = Mat::zeros(1, 8, CV_32FC1);
	Mat a2 = Mat::zeros(1, 8, CV_32FC1);
	filter2D(downDst3, a1, -1, low1);
	filter2D(downDst4, a2, -1, high1);

	for (int i = 0; i <8; i++)
	{
		downDst1.at<float>(0, i) = a1.at<float>(0, 7 - i);
		downDst2.at<float>(0, i) = a2.at<float>(0, 7 - i);

	}

	Mat a3 = downDst1 + downDst2;
	
	*/
	
	string path = "G:\\AtestImage\\1.bmp";
	Mat src_temp = imread(path, 0);
	//Mat _src;
	//cvtColor(src_temp, _src, CV_RGB2GRAY);
	Mat imgWave = m_waveTransform.WDT(src_temp, "haar", 1);
	Mat imgSor = m_waveTransform.IWDT(imgWave, "haar", 1);
	//namedWindow("img", CV_WINDOW_NORMAL);
    //imshow("img", Mat_<uchar>(imgWave));


	//Mat _imgSor = Mat::zeros(imgSor.rows, imgSor.cols, CV_8UC1);
	//normalize(imgSor, imgSor, 1.0, 0.0, NORM_MINMAX);//归一到0~1之间
	//imgSor.convertTo(_imgSor, CV_8UC1, 255, 0); //转换为0~255之间的整数
	Mat _imgSor = Mat_<uchar>(imgSor);
	Mat compare = src_temp - _imgSor;
	imshow("compare", compare);

	//namedWindow("sor", CV_WINDOW_NORMAL);
	//imshow("sor", _imgSor);

	imwrite("rewrite.bmp", _imgSor);
	waitKey();
}