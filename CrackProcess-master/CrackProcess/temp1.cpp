// OpenCVTest.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MathFunction.h"
#include "Utilities.h"
#include "CrackInfo.h"

using namespace std;
using namespace cv;
using namespace Custom;

int squareWidth = 320;
int threshhold = 150000;

const string winName1 = "srcImg";
const string winName2 = "dstImg";

int main4(int argc, char** argv) {
	Mat srcImg = imread("F:\\image\\20161208_125325.jpg");//C:\\Users\\23925\\Pictures\\Saved Pictures\\picture1.jpg
	Mat dstImg, dstImg2;

	cvtColor(srcImg, dstImg, CV_BGR2GRAY, 1);
	cout << "灰度化完成" << endl;

	/*Utilities::addContrast(dstImg);
	cout << "增加对比度完成" << endl;

	Utilities::swapMat(srcImg, dstImg);
	Canny(srcImg, dstImg, 50, 150);
	cout << "边缘检测完成" << endl;

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(dstImg, dstImg, kernel);
	morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel, Point(-1, -1), 3);
	morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel);

	vector<vector<Point>> connectedDomains;
	Utilities::findConnectedDomain(dstImg, connectedDomains, 20);
	kernel = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
	morphologyEx(dstImg, dstImg, CV_MOP_CLOSE, kernel, Point(-1, -1), 5);

	connectedDomains.clear();
	Utilities::findConnectedDomain(dstImg, connectedDomains, 20);
	kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	morphologyEx(dstImg, dstImg, CV_MOP_OPEN, kernel);

	kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	erode(dstImg, dstImg, kernel);


	connectedDomains.clear();
	Utilities::findConnectedDomain(dstImg, connectedDomains, 20);

	cout << "开始测量" << endl;
	cout << "连通域数量：" << connectedDomains.size() << endl;
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
		Point position = Utilities::calInfoPosition(dstImg.rows, dstImg.cols, *domain_it);
		crackInfos.push_back(CrackInfo(position, length, (float)(area / length)));
	}

	cout << "开始绘制信息" << endl;
	cout << "信息数量：" << crackInfos.size() << endl;

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
		putText(dstImg, info.str(), it->Position, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255));
	}
	//Utilities::save2PNG(dstImg, "C:\\Users\\23925\\Desktop\\re.png", 0, 255, 0);
	*/

	imwrite("C:\\Users\\23925\\Desktop\\result1.png", dstImg);
	cout << "保存图像完成" << endl;

	/*vector<vector<Point>> contours;
	findContours(dstImg, contours, RETR_LIST, CV_CHAIN_APPROX_NONE);

	for (auto begin = contours.begin(); begin != contours.end();) {
		if (begin->size() < 20)
			begin = contours.erase(begin);
		else
			++begin;
	}

	srcImg = Scalar::all(0);
	drawContours(srcImg, contours, -1, Scalar::all(255));
	Utilities::swapMat(srcImg, dstImg);
	vector<cv::Point> points;

	Mat_<uchar> tempImg = dstImg;
	for (int i = 0; i < tempImg.rows - squareWidth; i += squareWidth) {
		uchar *row = tempImg.ptr(i);
		for (int j = 0; j < tempImg.cols - squareWidth; ++j) {
			points.clear();
			Utilities::getPoints(dstImg, i, j, points, squareWidth);
			if (points.size() < 100) {
				Utilities::clearArea(dstImg, i, j, squareWidth);
				j += squareWidth - 1;
			}
			else {
				Utilities::recalculatePoints(i, j, points);
				double a0 = 0;
				double a1 = 0;
				MathFunction::leastSquareProcedure(points, a0, a1);
				double derivation = MathFunction::calDeviation(points, a0, a1);
				if (derivation > threshhold) {
					Utilities::clearArea(dstImg, i, j, squareWidth);
				}
				else {
					cout << derivation << endl;
				}
				j += squareWidth - 1;
			}
		}
	}


	fillCrack(dstImg, 9);
	cout << "填充裂缝" << endl;

	swapMat(srcImg, dstImg);
	medianBlur(srcImg, dstImg, 7);
	cout << "中值滤波" << endl;
	*/


	return 0;
}
