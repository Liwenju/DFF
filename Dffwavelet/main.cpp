
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

	Mat src1, src2, src3, src4, src5;//Դͼ����ϵ��

	vector<Mat> srcImg;//Դͼ����
	srcImg.clear();//ʸ����ʼ��
	string imgPath = "G:\\AtestImage\\";//ԴͼĿ¼
	char xx[10];//��ʱ�ַ�����

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


	//Mat imgWave = m_waveTransform.WDT(_src, "haar", 3);
	//Mat imgSor = m_waveTransform.IWDT(imgWave, "haar", 3);
	//imshow("img", Mat_<uchar>(imgWave));
	Mat _imgSor = Mat::zeros(imgFusion.rows, imgFusion.cols, CV_8UC1);
	normalize(imgFusion, imgFusion, 1.0, 0.0, NORM_MINMAX);//��һ��0~1֮��
	imgFusion.convertTo(_imgSor, CV_8UC1, 255, 0); //ת��Ϊ0~255֮�������
	imshow("sor", _imgSor);
	//imwrite("rewrite.png", _imgSor);
	waitKey();
}