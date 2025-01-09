#include "stdafx.h"
#include "CrackProcess.h"
#include "Utilities.h"
#include "CrackInfo.h"

CRACKPROCESSDLL_API int __stdcall FindCracks(char* srcFileName, char* dstFileName, int red, int green, int blue) {
	using cv::imread;
	using cv::cvtColor;
	using cv::getStructuringElement;
	using cv::Mat;
	using cv::Point;
	using cv::Size;
	using cv::ImreadModes;
	using cv::MorphShapes;
	using namespace Custom;

	int flagValue = 0;
	try {
		Mat srcImg, dstImg, tempImg, temp;
		srcImg = imread(srcFileName, ImreadModes::IMREAD_UNCHANGED);
		cvtColor(srcImg, dstImg, CV_BGR2GRAY);

		Utilities::addContrast(dstImg);
		Canny(dstImg, dstImg, 50, 150);

		Mat kernel = getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3, 3));
		dilate(dstImg, dstImg, kernel);
		morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel, Point(-1, -1), 3);
		morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel);

		Utilities::save2PNG(dstImg, dstFileName, red, green, blue);
	}
	catch (const std::exception&) {
		flagValue = -1;
	}
	return flagValue;
}

CRACKPROCESSDLL_API int __stdcall Filter(char* srcFileName, char* dstFileName, int red, int green, int blue) {
	using cv::imread;
	using cv::cvtColor;
	using cv::getStructuringElement;
	using cv::Mat;
	using cv::Point;
	using cv::Size;
	using std::vector;
	using cv::ImreadModes;
	using cv::MorphShapes;
	using namespace Custom;

	int flagValue = 0;
	try {
		Mat srcImg, dstImg, temp;
		srcImg = imread(srcFileName, ImreadModes::IMREAD_UNCHANGED);
		cvtColor(srcImg, temp, CV_BGRA2BGR);
		cvtColor(temp, dstImg, CV_BGR2GRAY);

		Utilities::binaryzation(dstImg);

		vector<vector<Point>> connectedDomains;
		Utilities::findConnectedDomain(dstImg, connectedDomains, 20, 3);
		Mat kernel = getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(7, 7));
		morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel, Point(-1, -1), 5);

		kernel = getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3, 3));
		erode(dstImg, dstImg, kernel);

		Utilities::save2PNG(dstImg, dstFileName, red, green, blue);
	}
	catch (const std::exception&) {
		flagValue = -1;
	}
	return flagValue;
}

CRACKPROCESSDLL_API int __stdcall MeasureCracks(char* srcFileName, char* dstFileName, int red, int green, int blue) {
	using cv::Mat;
	using cv::Point;
	using cv::Scalar;
	using cv::ImreadModes;
	using cv::HersheyFonts;
	using cv::imread;
	using std::vector;
	using std::ostringstream;
	using namespace Custom;

	int flagValue = 0;
	try {
		Mat srcImg, dstImg, temp;
		srcImg = imread(srcFileName, ImreadModes::IMREAD_UNCHANGED);
		cvtColor(srcImg, temp, CV_BGRA2BGR);
		cvtColor(temp, dstImg, CV_BGR2GRAY);

		Utilities::binaryzation(dstImg);

		vector<vector<Point>> connectedDomains;
		Utilities::findConnectedDomain(dstImg, connectedDomains, 20, 3);

		Mat lookUpTable(1, 256, CV_8U, Scalar(0));
		vector<CrackInfo> crackInfos;
		for (auto domain_it = connectedDomains.begin(); domain_it != connectedDomains.end(); ++domain_it) {
			LUT(dstImg, lookUpTable, dstImg);
			for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it) {
				dstImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
			}
			double area = (double)domain_it->size();
			Utilities::thinImage(dstImg);
			Utilities::getWhitePoints(dstImg, *domain_it);
			long length = (long)domain_it->size();
			Point position = Utilities::calInfoPosition(dstImg.rows, dstImg.cols, 50, *domain_it);
			crackInfos.push_back(CrackInfo(position, length, (float)(area / length)));
		}

		LUT(dstImg, lookUpTable, dstImg);
		for (auto domain_it = connectedDomains.cbegin(); domain_it != connectedDomains.cend(); ++domain_it) {
			for (auto point_it = domain_it->cbegin(); point_it != domain_it->cend(); ++point_it) {
				dstImg.ptr<uchar>(point_it->y)[point_it->x] = 255;
			}
		}

		ostringstream info;
		for (auto it = crackInfos.cbegin(); it != crackInfos.cend(); ++it) {
			info.str("");
			info << *it;
			putText(dstImg, info.str(), it->Position, HersheyFonts::FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255));
		}

		Utilities::save2PNG(dstImg, dstFileName, red, green, blue);
	}
	catch (const std::exception&) {
		flagValue = -1;
	}
	return flagValue;
}

CRACKPROCESSDLL_API int __stdcall Stitcher(char * srcFileNames, char * dstFileName) {
	using std::string;
	using std::vector;
	using cv::Mat;
	using cv::Stitcher;
	using cv::imread;
	using cv::imwrite;

	int flagValue = 0;
	try {
		vector<Mat> images;
		string tempStr(srcFileNames);
		if (tempStr[tempStr.size() - 1] != ';')
			tempStr += ";";
		size_t pos = tempStr.find(";");
		size_t size = tempStr.size();

		while (pos != string::npos) {
			string fileName = tempStr.substr(0, pos);
			images.push_back(imread(fileName));
			tempStr = tempStr.substr(pos + 1, size);
			pos = tempStr.find(";");
		}

		Mat result;
		Stitcher stitcher = Stitcher::createDefault(true);
		Stitcher::Status status = stitcher.stitch(images, result);
		if (status == Stitcher::OK)
			imwrite(dstFileName, result);
		else
			flagValue = (int)status;
	}
	catch (const std::exception&) {
		flagValue = -1;
	}
	return flagValue;
}
