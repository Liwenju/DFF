
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

	Mat src1, src2, src3, src4, src5;//ͼƬ����ϵ��

	vector<Mat> src_img;//ͼƬ����ϵ��
	src_img.clear();
	string path = "G:\\AtestImage\\";
	char xx[10];

	for (int i = 1; i <= 5; i++)
	{
		sprintf(xx, "%d", i);
		string s1(xx);
		string s2(".bmp");
		string s3 = path + s1 + s2;
		Mat src_temp = imread(s3, 0);
		src_img.push_back(src_temp.clone());
	}


	Mat src1 = imread("G:\\AtestImage\\1.bmp");//��ͼƬ��ȡΪ����
	Mat src2 = imread("G:\\AtestImage\\2.bmp");
	Mat src3 = imread("G:\\AtestImage\\3.bmp");
	Mat src4 = imread("G:\\AtestImage\\4.bmp");
	Mat src5 = imread("G:\\AtestImage\\5.bmp");
	Mat _src1, _src2, _src3, _src4, _src5;
	cvtColor(src1, _src1, CV_RGB2GRAY);//����ͨ��ת��Ϊ��ͨ���Ҷ�
	cvtColor(src2, _src2, CV_RGB2GRAY);
	cvtColor(src3, _src3, CV_RGB2GRAY);
	cvtColor(src4, _src4, CV_RGB2GRAY);
	cvtColor(src5, _src5, CV_RGB2GRAY);
	
	Mat imgWave1 = m_waveTransform.WDT(_src1, "haar", 3);//�Ҷ�ͼHarrС�����߶ȷֽ�
	Mat imgWave2 = m_waveTransform.WDT(_src2, "haar", 3);
	Mat imgWave3 = m_waveTransform.WDT(_src3, "haar", 3);
	Mat imgWave4 = m_waveTransform.WDT(_src4, "haar", 3);
	Mat imgWave5 = m_waveTransform.WDT(_src5, "haar", 3);

	Mat srcArray[5] = { _src1, _src2, _src3, _src4, _src5 };//С���ֽ�ͼ�ϳ�����

	Mat imgFusion = m_waveTransform.WFR(srcArray);


	//Mat imgWave = m_waveTransform.WDT(_src, "haar", 3);
	//Mat imgSor = m_waveTransform.IWDT(imgWave, "haar", 3);
	//imshow("img", Mat_<uchar>(imgWave));
	//Mat _imgSor = Mat::zeros(imgSor.rows, imgSor.cols, CV_8UC1);
	//normalize(imgSor, imgSor, 1.0, 0.0, NORM_MINMAX);//��һ��0~1֮��
	//imgSor.convertTo(_imgSor, CV_8UC1, 255, 0); //ת��Ϊ0~255֮�������
	//imshow("sor", _imgSor);
	//imwrite("rewrite.png", _imgSor);
	waitKey();
}