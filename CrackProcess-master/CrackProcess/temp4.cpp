#include "stdafx.h"
#include "Utilities.h"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
using namespace Custom;

int main5() {
	Mat image01 = imread("C:\\Users\\23925\\Desktop\\image2\\1.jpg", 1);
	Mat image02 = imread("C:\\Users\\23925\\Desktop\\image2\\2.jpg", 1);
	Mat image03 = imread("C:\\Users\\23925\\Desktop\\image2\\3.jpg", 1);
	Mat image04 = imread("C:\\Users\\23925\\Desktop\\image2\\4.jpg", 1);
	Mat image05 = imread("C:\\Users\\23925\\Desktop\\image2\\5.jpg", 1);

	vector<Mat> images;
	images.push_back(image01);
	images.push_back(image02);
	images.push_back(image03);
	images.push_back(image04);
	images.push_back(image05);

	Mat result;
	Stitcher stitcher = Stitcher::createDefault(true);
	Stitcher::Status status = stitcher.stitch(images, result);

	if (status != Stitcher::OK) {
		cout << "Can't stitch images, error code = " << int(status) << endl;
	}

	imwrite("C:\\Users\\23925\\Desktop\\image2\\result.jpg", result);

	return 0;
}
