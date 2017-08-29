#include "WaveTransform.h"
#include <math.h>
#include <imgproc/imgproc.hpp>


Mat WaveTransform::WDT(const Mat &_src, const string _wname, const int _level)
{
	Mat src = Mat_<float>(_src);
	Mat dst = Mat::zeros(src.rows, src.cols, src.type());
	int N = src.rows;
	int D = src.cols;
	//��ͨ��ͨ�˲���
	Mat lowFilter;
	Mat highFilter;
	wavelet(_wname, lowFilter, highFilter);
	//С���任
	int t = 1;
	int row = N;
	int col = D;
	while (t <= _level)
	{
		//�Ƚ��� ��С���任
		for (int i = 0; i<row; i++)
		{
			//ȡ��src��Ҫ��������ݵ�һ��
			Mat oneRow = Mat::zeros(1, col, src.type());
			for (int j = 0; j<col; j++)
			{
				oneRow.at<float>(0, j) = src.at<float>(i, j);
			}
			oneRow = waveletDecompose(oneRow, lowFilter, highFilter);
			for (int j = 0; j<col; j++)
			{
				dst.at<float>(i, j) = oneRow.at<float>(0, j);
			}
		}
		//char s[10];
		//itoa(t, s, 10);
		//imshow(s, dst);
		//waitKey();
#if 0
		//    normalize(dst,dst,0,255,NORM_MINMAX);
		IplImage dstImg1 = IplImage(dst);
		cvSaveImage("dst1.jpg", &dstImg1);
#endif

		//С���б任
		for (int j = 0; j<col; j++)
		{
			Mat oneCol = Mat::zeros(row, 1, src.type());
			for (int i = 0; i<row; i++)
			{
				oneCol.at<float>(i, 0) = dst.at<float>(i, j);//dst,not src
			}
			oneCol = (waveletDecompose(oneCol.t(), lowFilter, highFilter)).t();
			for (int i = 0; i<row; i++)
			{
				dst.at<float>(i, j) = oneCol.at<float>(i, 0);
			}
		}
#if 0
		//    normalize(dst,dst,0,255,NORM_MINMAX);
		IplImage dstImg2 = IplImage(dst);
		cvSaveImage("dst2.jpg", &dstImg2);
#endif
		//���� 
		row /= 2;
		col /= 2;
		t++;
		src = dst;

	}
	return dst;
}


Mat WaveTransform::IWDT(const Mat &_src, const string _wname, const int _level)
{
	//int reValue = THID_ERR_NONE;
	Mat src = Mat_<float>(_src);
	Mat dst = Mat::zeros(src.rows, src.cols, src.type());
	int N = src.rows;
	int D = src.cols;

	/// ��ͨ��ͨ�˲���

	Mat lowFilter;
	Mat highFilter;
	wavelet(_wname, lowFilter, highFilter);

	/// С���任
	int t = 1;
	int row = N / std::pow(2., _level - 1);
	int col = D / std::pow(2., _level - 1);

	while (row <= N && col <= D)
	{
		/// С������任
		for (int j = 0; j<col; j++)
		{
			/// ȡ��src���ݵ�һ������
			Mat oneCol = Mat::zeros(row, 1, src.type());
			for (int i = 0; i<row; i++)
			{
				oneCol.at<float>(i, 0) = src.at<float>(i, j);
			}
			oneCol = (waveletReconstruct(oneCol.t(), lowFilter, highFilter)).t();
			for (int i = 0; i<row; i++)
			{
				dst.at<float>(i, j) = oneCol.at<float>(i, 0);
			}
		}
#if 0

		//normalize( dst, dst, 0, 255, NORM_MINMAX );
		IplImage dstImg2 = IplImage(dst);
		cvSaveImage("dst.jpg", &dstImg2);
#endif

		///��С����任
		for (int i = 0; i<row; i++)
		{
			/// ȡ��src��Ҫ��������ݵ�һ��
			Mat oneRow = Mat::zeros(1, col, src.type());
			for (int j = 0; j<col; j++)
			{
				oneRow.at<float>(0, j) = dst.at<float>(i, j);
			}
			oneRow = waveletReconstruct(oneRow, lowFilter, highFilter);
			/// ��src��һ����ΪoneRow�е�����
			for (int j = 0; j<col; j++)
			{
				dst.at<float>(i, j) = oneRow.at<float>(0, j);
			}
		}
#if 0

		//normalize( dst, dst, 0, 255, NORM_MINMAX );
		IplImage dstImg1 = IplImage(dst);
		cvSaveImage("dst.jpg", &dstImg1);
#endif
		row *= 2;
		col *= 2;
		src = dst;
	}
	return dst;
}

//���ɲ�ͬ���͵�С��
void WaveTransform::wavelet(const string _wname, Mat &_lowFilter, Mat &_highFilter)
{

	if (_wname == "haar" || _wname == "db1")
	{
		int N = 2;
		_lowFilter = Mat::zeros(1, N, CV_32F);
		_highFilter = Mat::zeros(1, N, CV_32F);

		_lowFilter.at<float>(0, 0) = 1 / sqrtf(N);
		_lowFilter.at<float>(0, 1) = 1 / sqrtf(N);

		_highFilter.at<float>(0, 0) = -1 / sqrtf(N);
		_highFilter.at<float>(0, 1) = 1 / sqrtf(N);
	}
	if (_wname == "sym2")
	{
		int N = 4;
		float h[] = { -0.483, 0.836, -0.224, -0.129 };
		float l[] = { -0.129, 0.224, 0.837, 0.483 };

		_lowFilter = Mat::zeros(1, N, CV_32F);
		_highFilter = Mat::zeros(1, N, CV_32F);

		for (int i = 0; i<N; i++)
		{
			_lowFilter.at<float>(0, i) = l[i];
			_highFilter.at<float>(0, i) = h[i];
		}
	}
}

//С���ֽ�
Mat WaveTransform::waveletDecompose(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter)
{
	assert(_src.rows == 1 && _lowFilter.rows == 1 && _highFilter.rows == 1);
	assert(_src.cols >= _lowFilter.cols && _src.cols >= _highFilter.cols);
	Mat &src = Mat_<float>(_src);

	int D = src.cols;

	Mat &lowFilter = Mat_<float>(_lowFilter);
	Mat &highFilter = Mat_<float>(_highFilter);

	//Ƶ���˲���ʱ������ifft( fft(x) * fft(filter)) = cov(x,filter) 
	Mat dst1 = Mat::zeros(1, D, src.type());
	Mat dst2 = Mat::zeros(1, D, src.type());

	filter2D(src, dst1, -1, lowFilter);
	filter2D(src, dst2, -1, highFilter);

	//�²���
	Mat downDst1 = Mat::zeros(1, D / 2, src.type());
	Mat downDst2 = Mat::zeros(1, D / 2, src.type());

	resize(dst1, downDst1, downDst1.size());
	resize(dst2, downDst2, downDst2.size());

	//����ƴ��
	for (int i = 0; i<D / 2; i++)
	{
		src.at<float>(0, i) = downDst1.at<float>(0, i);
		src.at<float>(0, i + D / 2) = downDst2.at<float>(0, i);

	}
	return src;
}

// С���ؽ�
Mat WaveTransform::waveletReconstruct(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter)
{
	assert(_src.rows == 1 && _lowFilter.rows == 1 && _highFilter.rows == 1);
	assert(_src.cols >= _lowFilter.cols && _src.cols >= _highFilter.cols);
	Mat &src = Mat_<float>(_src);
	int D = src.cols;
	Mat &lowFilter = Mat_<float>(_lowFilter);
	Mat &highFilter = Mat_<float>(_highFilter);

	/// ��ֵ;
	Mat Up1 = Mat::zeros(1, D, src.type());
	Mat Up2 = Mat::zeros(1, D, src.type());

	/// ��ֵΪ0

	//for ( int i=0, cnt=1; i<D/2; i++,cnt+=2 )

	//{

	//    Up1.at<float>( 0, cnt ) = src.at<float>( 0, i );     ///< ǰһ��

	//    Up2.at<float>( 0, cnt ) = src.at<float>( 0, i+D/2 ); ///< ��һ��

	//}

	/// ���Բ�ֵ
	Mat roi1(src, Rect(0, 0, D / 2, 1));//Rect(x,y,width,height) xy���Ͻ����� ��������
	Mat roi2(src, Rect(D / 2, 0, D / 2, 1));
	resize(roi1, Up1, Up1.size(), 0, 0, INTER_CUBIC);
	resize(roi2, Up2, Up2.size(), 0, 0, INTER_CUBIC);

	/// ǰһ���ͨ����һ���ͨ
	Mat dst1 = Mat::zeros(1, D, src.type());
	Mat dst2 = Mat::zeros(1, D, src.type());
	filter2D(Up1, dst1, -1, lowFilter);
	filter2D(Up2, dst2, -1, highFilter);

	/// ������
	dst1 = dst1 + dst2;
	return dst1;
}

//С���ںϹ���
Mat WaveTransform::WFR(const Vector<Mat> srcVector, const int _level)
{
	/*if (srcVector.size==0)
	{
		return;
	}*/
	if (srcVector.size() >= 2)
	{
		int imgNum = srcVector.size();
		int R = srcVector[0].rows;//ͼ�����������
		int C = srcVector[0].cols;
		int minRowArea = R / std::pow(2., _level);//С���任���������
		int minColArea = C / std::pow(2., _level);

		Mat Z = Mat::zeros(R, C, CV_8UC1); //ȫ�����

		Mat src1 = srcVector[0];
		Mat src2 = srcVector[1];
		Mat src3 = srcVector[2];
		Mat src4 = srcVector[3];
		Mat src5 = srcVector[4];
		//cvtColor(src1, src1, CV_RGB2GRAY);//����ͨ��ת��Ϊ��ͨ���Ҷ�
		//cvtColor(src2, src2, CV_RGB2GRAY);
		//cvtColor(src3, src3, CV_RGB2GRAY);
		//cvtColor(src4, src4, CV_RGB2GRAY);
		//cvtColor(src5, src5, CV_RGB2GRAY);

		Mat imgWave1 = WDT(src1, "haar", 3);//�Ҷ�ͼHarrС���߶ȷֽ�
		Mat imgWave2 = WDT(src2, "haar", 3);
		Mat imgWave3 = WDT(src3, "haar", 3);
		Mat imgWave4 = WDT(src4, "haar", 3);
		Mat imgWave5 = WDT(src5, "haar", 3);

		int row = 0, col = 0;//��������ֵ��ʼ��
		//�ںϴ���

		//��Ƶ�ں�
		for (int i = row; i <= minRowArea; i++)
		{
			for (int j = col; j <= minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}

		}
		//�ԽǷ����ں�
		row = minRowArea, col = minColArea;
		for (int i = row; i <= 2 * minRowArea; i++)
		{
			for (int j = col; j <= 2 * minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}

		}
		row = 2*minRowArea, col = 2*minColArea;
		for (int i = row; i <= 4 * minRowArea; i++)
		{
			for (int j = col; j <= 4 * minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}

		}
		row = 4 * minRowArea, col = 4 * minColArea;
		for (int i = row; i <= R; i++)
		{
			for (int j = col; j <= C; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}
		}


		//�з����ں�
		row = 0, col = minColArea;
		for (int i = row; i <= minRowArea; i++)
		{
			for (int j = col; j <= 2 * minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}

		}
		col = 2*minColArea;
		for (int i = row; i <=2*minRowArea; i++)
		{
			for (int j = col; j <= 4* minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}

		}
		col = 4 * minColArea;
		for (int i = row; i <= 4 * minRowArea; i++)
		{
			for (int j = col; j <= C; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}
		}


		//�з����ں�
		row = minRowArea, col = 0;
		for (int i = row; i <= 2 * minRowArea; i++)
		{
			for (int j = col; j <= minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}
		}
		row = 2*minRowArea;
		for(int i = row; i <= 4 * minRowArea; i++)
		{
			for (int j = col; j <= 2*minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}
		}
		row = 4 * minRowArea;
		for (int i = row; i <= R; i++)
		{
			for (int j = col; j <= 4*minColArea; j++)
			{
				Z.at<uchar>(i, j) = maxFun(imgWave1, imgWave2, imgWave3, imgWave4, imgWave5, i, j);
			}
		}
		return Z;
	}
	else
	{
		return srcVector[0];
	}
}

//�������ֵ
float WaveTransform::maxFun(const Mat src1, const Mat src2, const Mat src3, const Mat src4, const Mat src5, int row, int col)
{
	float tempValue = 0;
	if (src1.at<uchar>(row,col) > tempValue)
	{
		tempValue = src1.at<uchar>(row, col);
	}
	if (src2.at<uchar>(row, col) > tempValue)
	{
		tempValue = src2.at<uchar>(row, col);
	}
	if (src3.at<uchar>(row, col) > tempValue)
	{
		tempValue = src3.at<uchar>(row, col);
	}
	if (src4.at<uchar>(row, col) > tempValue)
	{
		tempValue = src4.at<uchar>(row, col);
	}
	if (src5.at<uchar>(row, col) > tempValue)
	{
		tempValue = src5.at<uchar>(row, col);
	}

	return tempValue;
}
