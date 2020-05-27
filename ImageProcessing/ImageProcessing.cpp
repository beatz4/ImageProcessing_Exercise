// ImageProcessing.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/types_c.h>

using namespace cv;
using namespace std;

#define SCALE 2.0

// 히스토그래 평활화
void ShowEqualizeHist(string imgPath);

// 히스토그램 역투영
void ShowBackProject(string imgPath);

int main()
{
	string strFile = "C:/Users/user/Documents/imageProcessing/exercise/20200324_140354_TestImage.png";
	ShowBackProject(strFile);
	
	return 0;

	//std::cout << "Hello World!\n"; 
}

void ShowEqualizeHist(string imgPath) 
{
	Mat inputImage;
	Mat greyImage;

	inputImage = imread(imgPath);

	resize(inputImage, inputImage, Size(), SCALE, SCALE, INTER_AREA);

	// input image를 gray로 변환
	cvtColor(inputImage, greyImage, cv::COLOR_BGR2GRAY);

	MatND histogram;
	const int* channel_numbers = { 0 };
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges = { channel_range };
	int number_bins = 255;

	calcHist(&greyImage, 1, channel_numbers, Mat(), histogram, 1, &number_bins, &channel_ranges);

	// Plot the histogram
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / number_bins);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(histogram, histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < number_bins; i++) {
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	// 평활화
	Mat dst;
	equalizeHist(greyImage, dst);

	namedWindow("Original", WINDOW_AUTOSIZE);
	namedWindow("Histogram", WINDOW_AUTOSIZE);
	namedWindow("EqualizeHist", WINDOW_AUTOSIZE);

	moveWindow("Original", 100, 100);
	moveWindow("Histogram", 120, 120);
	moveWindow("EqualizeHist", 140, 140);

	imshow("Original", greyImage);
	imshow("Histogram", histImage);
	imshow("EqualizeHist", dst);

	waitKey(0);
}

void ShowBackProject(string imgPath)
{
	Mat ref, ref_ycrcb;
	ref = imread(imgPath);
	cvtColor(ref, ref_ycrcb, cv::COLOR_BGR2YCrCb);

	// 영상 히스토그램
	Mat hist;
	int channels[] = { 1, 2 };
	int cr_bins = 128;
	int cb_bins = 128;
	int histSize[] = { cr_bins, cb_bins };
	float cr_range[] = { 0, 256 };
	float cb_range[] = { 0, 256 };
	const float* ranges[] = { cr_range, cb_range };

	// 영상 히스토그램
	calcHist(&ref_ycrcb, 1, channels, Mat(), hist, 2, histSize, ranges);

	Mat src, src_ycrcb;
	src = imread(imgPath);
	cvtColor(src, src_ycrcb, cv::COLOR_BGR2YCrCb);

	Mat backproj;
	// 역투영
	calcBackProject(&src_ycrcb, 1, channels, hist, backproj, ranges, 1, true);

	imshow("src", src);
	imshow("backproj", backproj);
	waitKey(0);
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
