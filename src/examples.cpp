/*
 * examples.cpp
 *
 *  Created on: 4 окт. 2023 г.
 *      Author: vevdokimov
 */

#include "opencv2/opencv.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

#include "examples.hpp"

using namespace std;
using namespace cv;

namespace blur_example
{

	int DELAY_CAPTION = 1500;
	int DELAY_BLUR = 100;
	int MAX_KERNEL_LENGTH = 31;

	Mat src; Mat dst;

	char window_name[] = "Smoothing Demo";

	int display_caption( const char* caption );

	int display_dst( int delay );

	int run(cv::Mat aSrc) {

		src = aSrc.clone();

		namedWindow( window_name, WINDOW_AUTOSIZE );

		dst = src.clone();
		if( display_dst( DELAY_CAPTION ) != 0 )
		{
			return 0;
		}
		if( display_caption( "Homogeneous Blur" ) != 0 )
		{
			return 0;
		}
		for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
		{
			blur( src, dst, Size( i, i ), Point(-1,-1) );
			if( display_dst( DELAY_BLUR ) != 0 )
			{
				return 0;
			}
		}
		if( display_caption( "Gaussian Blur" ) != 0 )
		{
			return 0;
		}
		for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
		{
			GaussianBlur( src, dst, Size( i, i ), 0, 0 );
			if( display_dst( DELAY_BLUR ) != 0 )
			{
				return 0;
			}
		}
		if( display_caption( "Median Blur" ) != 0 )
		{
			return 0;
		}
		for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
		{
			medianBlur ( src, dst, i );
			if( display_dst( DELAY_BLUR ) != 0 )
			{
				return 0;
			}
		}
		if( display_caption( "Bilateral Blur" ) != 0 )
		{
			return 0;
		}
		for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
		{
			bilateralFilter ( src, dst, i, i*2, i/2 );
			if( display_dst( DELAY_BLUR ) != 0 )
			{
				return 0;
			}
		}
		display_caption( "Done!" );
		return 0;
	}

	int display_caption( const char* caption )
	{
		dst = Mat::zeros( src.size(), src.type() );
		putText( dst, caption,
				 Point( src.cols/4, src.rows/2),
				 FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255) );
		return display_dst(DELAY_CAPTION);
	}
	int display_dst( int delay )
	{
		imshow( window_name, dst );
		int c = waitKey ( delay );
		if( c >= 0 ) { return -1; }
		return 0;
	}

}

namespace erosion_dilation_example
{

	Mat src, erosion_dst, dilation_dst;
	int erosion_elem = 0;
	int erosion_size = 0;
	int dilation_elem = 0;
	int dilation_size = 0;
	int const max_elem = 2;
	int const max_kernel_size = 21;
	void Erosion( int, void* );
	void Dilation( int, void* );

	int run(cv::Mat aSrc)
	{
	  src = aSrc.clone();

	  namedWindow( "Erosion Demo", WINDOW_AUTOSIZE );
	  namedWindow( "Dilation Demo", WINDOW_AUTOSIZE );
	  moveWindow( "Dilation Demo", src.cols, 0 );
	  createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
			  &erosion_elem, max_elem,
			  Erosion );
	  createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo",
			  &erosion_size, max_kernel_size,
			  Erosion );
	  createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
			  &dilation_elem, max_elem,
			  Dilation );
	  createTrackbar( "Kernel size:\n 2n +1", "Dilation Demo",
			  &dilation_size, max_kernel_size,
			  Dilation );
	  Erosion( 0, 0 );
	  Dilation( 0, 0 );
	  waitKey(0);
	  return 0;
	}
	void Erosion( int, void* )
	{
	  int erosion_type = 0;
	  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
	  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
	  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
	  Mat element = getStructuringElement( erosion_type,
						   Size( 2*erosion_size + 1, 2*erosion_size+1 ),
						   Point( erosion_size, erosion_size ) );
	  erode( src, erosion_dst, element );
	  imshow( "Erosion Demo", erosion_dst );
	}
	void Dilation( int, void* )
	{
	  int dilation_type = 0;
	  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
	  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
	  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
	  Mat element = getStructuringElement( dilation_type,
						   Size( 2*dilation_size + 1, 2*dilation_size+1 ),
						   Point( dilation_size, dilation_size ) );
	  dilate( src, dilation_dst, element );
	  imshow( "Dilation Demo", dilation_dst );
	}

}

namespace moprphology_example
{

	Mat src, dst;
	int morph_elem = 0;
	int morph_size = 0;
	int morph_operator = 0;
	int const max_operator = 4;
	int const max_elem = 2;
	int const max_kernel_size = 21;
	const char* window_name = "Morphology Transformations Demo";
	void Morphology_Operations( int, void* );

	int run(cv::Mat aSrc)
	{

	  src = aSrc.clone();

	  namedWindow( window_name, WINDOW_AUTOSIZE ); // Create window
	  createTrackbar("Operator:\n 0: Opening - 1: Closing  \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator, Morphology_Operations );
	  createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,
					  &morph_elem, max_elem,
					  Morphology_Operations );
	  createTrackbar( "Kernel size:\n 2n +1", window_name,
					  &morph_size, max_kernel_size,
					  Morphology_Operations );
	  Morphology_Operations( 0, 0 );
	  waitKey(0);
	  return 0;
	}
	void Morphology_Operations( int, void* )
	{
	  // Since MORPH_X : 2,3,4,5 and 6
	  int operation = morph_operator + 2;
	  Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
	  morphologyEx( src, dst, operation, element );
	  imshow( window_name, dst );
	}

}

namespace hitmiss_example
{

	int run(cv::Mat aSrc) {

		//Mat input_image = aSrc.clone();
		Mat input_image = (Mat_<uchar>(8, 8) <<
			0, 0, 0, 0, 0, 0, 0, 0,
			0, 255, 255, 255, 0, 0, 0, 255,
			0, 255, 255, 255, 0, 0, 0, 0,
			0, 255, 255, 255, 0, 255, 0, 0,
			0, 0, 255, 0, 0, 0, 0, 0,
			0, 0, 255, 0, 0, 255, 255, 0,
			0, 255, 0, 255, 0, 0, 255, 0,
			0, 255, 255, 255, 0, 0, 0, 0);
		Mat kernel = (Mat_<int>(3, 3) <<
			0, 1, 0,
			1, -1, 1,
			0, 1, 0);
		Mat output_image;
		morphologyEx(input_image, output_image, MORPH_HITMISS, kernel);
		const int rate = 50;
	 kernel = (kernel + 1) * 127;
	 kernel.convertTo(kernel, CV_8U);
		resize(kernel, kernel, Size(), rate, rate, INTER_NEAREST);
		imshow("kernel", kernel);
		moveWindow("kernel", 0, 0);
		resize(input_image, input_image, Size(), rate, rate, INTER_NEAREST);
		imshow("Original", input_image);
		moveWindow("Original", 0, 200);
		resize(output_image, output_image, Size(), rate, rate, INTER_NEAREST);
		imshow("Hit or Miss", output_image);
		moveWindow("Hit or Miss", 500, 200);
		waitKey(0);
		return 0;
	}

}

namespace threshold_example
{

	int threshold_value = 0;
	int threshold_type = 3;
	int const max_value = 255;
	int const max_type = 4;
	int const max_binary_value = 255;
	Mat src, src_gray, dst;
	const char* window_name = "Threshold Demo";
	const char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
	const char* trackbar_value = "Value";
	static void Threshold_Demo( int, void* )
	{
		/* 0: Binary
		 1: Binary Inverted
		 2: Threshold Truncated
		 3: Threshold to Zero
		 4: Threshold to Zero Inverted
		*/
		threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type );
		imshow( window_name, dst );
	}

	int run(cv::Mat aSrc)
	{
		src = aSrc.clone();

		cvtColor( src, src_gray, COLOR_BGR2GRAY ); // Convert the image to Gray
		namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
		createTrackbar( trackbar_type,
						window_name, &threshold_type,
						max_type, Threshold_Demo ); // Create a Trackbar to choose type of Threshold
		createTrackbar( trackbar_value,
						window_name, &threshold_value,
						max_value, Threshold_Demo ); // Create a Trackbar to choose Threshold value
		Threshold_Demo( 0, 0 ); // Call the function to initialize
		waitKey();
		return 0;
	}

}
