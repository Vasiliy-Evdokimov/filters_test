//============================================================================
// Name        : filters_test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "examples.hpp"

using namespace std;
using namespace cv;

#define USE_CAM 1
#define CAM_ADDR "rtsp://admin:1234qwer@192.168.1.63:554/streaming/channels/2"
#define IMG_ADDR "test100.png"

void add_img(std::vector<cv::Mat>& aImages, cv::Mat aImg, string aText) {
	cv::Mat aImgCpy = aImg.clone();
	cv::putText(aImgCpy, aText, cv::Point(10, 30),
		cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0));
	aImages.push_back(aImgCpy);
}

void process_img(Mat imgColor) {


	if (USE_CAM)
		imshow("source", imgColor);

	if (!USE_CAM) {
		//blur_example::run(imgColor);
		//erosion_dilation_example::run(imgColor);
		//moprphology_example::run(imgColor);
		//hitmiss_example::run(imgColor); // << for binary only
		//threshold_example::run(imgColor);
	}

	std::vector<cv::Mat> images;

	Mat im0;
	cvtColor(imgColor, im0, cv::COLOR_BGR2GRAY);
	add_img(images, im0, "cvtColor");

//	Mat im01;
//	int ers = 2;
//	Mat erel = getStructuringElement( MORPH_RECT, Size(2 * ers + 1, 2 * ers + 1), Point(ers, ers));
//	cv::erode(im0, im01, erel);
//	add_img(images, im01, "erode");

	Mat im1;
	int gbk = 25;
	cv::GaussianBlur(im0, im1, Size(gbk, gbk), 0);
	add_img(images, im1, "GaussianBlur");

	//moprphology_example::run(im1);

	Mat im2;
	int mok = 25;
	cv::morphologyEx(im1, im2, MORPH_OPEN, getStructuringElement(MORPH_RECT, Size(mok, mok)));
	add_img(images, im2, "MORPH_OPEN");

	//moprphology_example::run(im2);

	Mat im3;
	int mck = 9;
	cv::morphologyEx(im2, im3, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(mck, mck)));
	add_img(images, im3, "MORPH_CLOSE");

	//threshold_example::run(im3);

	cv::Mat im5;
	cv::threshold(im3, im5, 160, 255, cv::THRESH_BINARY_INV );
	add_img(images, im5, "thresholdClose");

	//

	const int ROW = 3;

	while (images.size() % ROW != 0)
		images.push_back(images[images.size() - 1].clone());

	int images_sz = images.size();
	std::vector<cv::Mat> rowsMerged;
	for (int i = 0; i < (images_sz / ROW); i++) {
		std::vector<cv::Mat> rowImages;
		for (int j = 0; j < ROW; j++)
			rowImages.push_back(images[i * ROW + j]);
		cv::Mat rowMerged;
		cv::hconcat(rowImages, rowMerged);
		rowsMerged.push_back(rowMerged);
	}

	cv::Mat mergedImage;
	cv::vconcat(rowsMerged, mergedImage);
	cv::imshow("Merged Image", mergedImage);

}

int main() {

	Mat imgColor;

	if (USE_CAM) {

		VideoCapture cap;

		cap.open(CAM_ADDR);

		for (int i = 0; i < 20; i++)
			cap >> imgColor;

		while (1) {

			cap >> imgColor;

			process_img(imgColor);

			cv::waitKey(1);
		}

	} else {

		imgColor = imread(IMG_ADDR, IMREAD_COLOR);

		process_img(imgColor);

		cv::waitKey(0);

	}

	return 0;
}
