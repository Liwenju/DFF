#include "WaveTransform.h"
#include <math.h>
#include <imgproc/imgproc.hpp>


Mat WaveTransform::WDT(const Mat &_src, const string _wname, const int _level)
{
	Mat src = Mat_<float>(_src);
	Mat dst = Mat::zeros(src.rows, src.cols, src.type());
	int N = src.rows;
	int D = src.cols;
	//高通低通滤波器
	Mat lowFilter;
	Mat highFilter;
	wavelet(_wname, lowFilter, highFilter);
	//小波变换
	int t = 1;
	int row = N;
	int col = D;
	while (t <= _level)
	{
		//先进行 行小波变换
		for (int i = 0; i<row; i++)
		{
			//取出src中要处理的数据的一行
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

		//小波列变换
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
		//更新 
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

	/// 高通低通滤波器

	Mat lowFilter;
	Mat highFilter;
	wavelet(_wname, lowFilter, highFilter);

	/// 小波变换
	int t = 1;
	int row = N / std::pow(2., _level - 1);
	int col = D / std::pow(2., _level - 1);

	while (row <= N && col <= D)
	{
		/// 小波列逆变换
		for (int j = 0; j<col; j++)
		{
			/// 取出src数据的一行输入
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

		///行小波逆变换
		for (int i = 0; i<row; i++)
		{
			/// 取出src中要处理的数据的一行
			Mat oneRow = Mat::zeros(1, col, src.type());
			for (int j = 0; j<col; j++)
			{
				oneRow.at<float>(0, j) = dst.at<float>(i, j);
			}
			oneRow = waveletReconstruct(oneRow, lowFilter, highFilter);
			/// 将src这一行置为oneRow中的数据
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

//生成不同类型的小波
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

//小波分解
Mat WaveTransform::waveletDecompose(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter)
{
	assert(_src.rows == 1 && _lowFilter.rows == 1 && _highFilter.rows == 1);
	assert(_src.cols >= _lowFilter.cols && _src.cols >= _highFilter.cols);
	Mat &src = Mat_<float>(_  src);

	int D = src.cols;

	Mat &lowFilter = Mat_<float>(_lowFilter);
	Mat &highFilter = Mat_<float>(_highFilter);

	//频域滤波或时域卷积；ifft( fft(x) * fft(filter)) = cov(x,filter) 
	Mat dst1 = Mat::zeros(1, D, src.type());
	Mat dst2 = Mat::zeros(1, D, src.type());

	filter2D(src, dst1, -1, lowFilter);
	filter2D(src, dst2, -1, highFilter);

	//下采样
	Mat downDst1 = Mat::zeros(1, D / 2, src.type());
	Mat downDst2 = Mat::zeros(1, D / 2, src.type());

	resize(dst1, downDst1, downDst1.size());
	resize(dst2, downDst2, downDst2.size());

	//数据拼接
	for (int i = 0; i<D / 2; i++)
	{
		src.at<float>(0, i) = downDst1.at<float>(0, i);
		src.at<float>(0, i + D / 2) = downDst2.at<float>(0, i);

	}
	return src;
}

// 小波重建
Mat WaveTransform::waveletReconstruct(const Mat &_src, const Mat &_lowFilter, const Mat &_highFilter)
{
	assert(_src.rows == 1 && _lowFilter.rows == 1 && _highFilter.rows == 1);
	assert(_src.cols >= _lowFilter.cols && _src.cols >= _highFilter.cols);
	Mat &src = Mat_<float>(_src);
	int D = src.cols;
	Mat &lowFilter = Mat_<float>(_lowFilter);
	Mat &highFilter = Mat_<float>(_highFilter);

	/// 插值;
	Mat Up1 = Mat::zeros(1, D, src.type());
	Mat Up2 = Mat::zeros(1, D, src.type());

	/// 插值为0

	//for ( int i=0, cnt=1; i<D/2; i++,cnt+=2 )

	//{

	//    Up1.at<float>( 0, cnt ) = src.at<float>( 0, i );     ///< 前一半

	//    Up2.at<float>( 0, cnt ) = src.at<float>( 0, i+D/2 ); ///< 后一半

	//}

	/// 线性插值
	Mat roi1(src, Rect(0, 0, D / 2, 1));//Rect(x,y,width,height) xy左上角坐标 矩形区域
	Mat roi2(src, Rect(D / 2, 0, D / 2, 1));
	resize(roi1, Up1, Up1.size(), 0, 0, INTER_CUBIC);
	resize(roi2, Up2, Up2.size(), 0, 0, INTER_CUBIC);

	/// 前一半低通，后一半高通
	Mat dst1 = Mat::zeros(1, D, src.type());
	Mat dst2 = Mat::zeros(1, D, src.type());
	filter2D(Up1, dst1, -1, lowFilter);
	filter2D(Up2, dst2, -1, highFilter);

	/// 结果相加
	dst1 = dst1 + dst2;
	return dst1;
}

//小波融合规则
Mat WaveTransform::WFR(const Mat srcArray[], const int _level)
{
	int R = 1200;
	int C = 1600;

	int minRow = R/ std::pow(2., _level - 1);
	int minCol = C / std::pow(2., _level - 1);

	int row = 0, col = 0;

	//低频融合
	while (row<=minRow && col<=minCol)
	{
		for (size_t i = 0; i < length; i++)
		{

		}
	}

	
	//对角分量融合
	row = minRow, col = minCol;
	while (row <= 2*minRow && col <= 2*minCol)
	{

	}
	while (row <= 4 * minRow && col <= 4 * minCol)
	{

	}
	while (row <= R && col <= C)
	{

	}


	//行分量融合
	row = 0, col = minCol;
	while (row <=  minRow && col <= 2*minCol)
	{

	}
	row = 0;
	while (row <= 2 * minRow && col <= 4 * minCol)
	{

	}
	row = 0;
	while (row <= 4 * minRow && col <= C)
	{

	}


	//列分量融合
	row = minRow, col = 0;
	while (row <= 2*minRow && col <= minCol)
	{

	}
	col = 0;
	while (row <= 4 * minRow && col <= 2 * minCol)
	{

	}
	col = 0;
	while (row <= R && col <= 4 * minCol)
	{

	}

}