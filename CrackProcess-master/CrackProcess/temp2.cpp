#include "stdafx.h"
#include "MathFunction.h"
#include "Utilities.h"


using namespace cv;
using namespace std;
using namespace Custom;

void getPoints1(cv::Mat &dstImg, int i, int j, std::vector<Point> &points);

void thinImage(const Mat & src, Mat & dst, const int iterations);

bool findNextPoint(vector<Point>& _neighbor_points, Mat & _image, Point _inpoint, int flag, Point & _outpoint, int & _outflag);

bool findFirstPoint(Mat & _inputimg, Point & _outputpoint);

void findLines(Mat & _inputimg, vector<deque<Point>>& _outputlines);

Scalar random_color(RNG & _rng);

int main3(int argc, char** argv) {
	Mat srcImg = imread("C:\\Users\\23925\\Desktop\\dstImg.png");//C:\\Users\\23925\\Desktop\\image1\\自适应阈值处理.png
	Mat dstImg, dstImg2;

	cvtColor(srcImg, dstImg, COLOR_RGB2GRAY);//COLOR_RGBA2GRAY
	cout << "灰度化完成" << endl;

	/*vector<Point> points;
	int flag = 10;
	Mat_<uchar> tempImg = dstImg;
	getPoints1(dstImg, 0, 0, points);
	double a0 = 0;
	double a1 = 0;
	Custom::MathFunction::leastSquareProcedure(points, a0, a1);
	line(srcImg, Point(0, (int)a0), Point(srcImg.cols, (int)(a0 + a1 * srcImg.cols)), Scalar(255, 255, 0));
	double derivation = MathFunction::calDeviation(points, a0, a1);
	cout << derivation;*/

	//imwrite("C:\\Users\\23925\\Desktop\\image1\\line.png", srcImg);

	/*swapMat(srcImg, dstImg);
	adaptiveThreshold(srcImg, dstImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 35);*/
	//thinImage(dstImg, dstImg2, 4);

	/*namedWindow("srcImg", 1);
	imshow("srcImg", dstImg);*/

	//Utilities::save2PNG(dstImg, 255, 0, 0, "C:\\Users\\23925\\Desktop\\Process2.png");

	/*vector<myPoint> myPoints{
		myPoint(1,0.5),
		myPoint(2,2.5),
		myPoint(3,2),
		myPoint(4,4),
		myPoint(5,3.5),
		myPoint(6,6),
		myPoint(7,6.5)
	};
	double a0 = 0.071;
	double a1 = 0.839;
	double deviation = MathFunction::calDeviation(myPoints, a0, a1);
	cout << deviation;*/

	/*vector<int> nums{ 1,2,3 };
	int sum = std::accumulate(nums.begin(), nums.end(), 0, [](int a, int b) {return a  + b * b ; });
	cout << sum;
*/

	Utilities::thinImage(dstImg);

	vector<deque<Point>> lines;
	findLines(dstImg, lines);
	cout << lines.size() << endl;
	//draw lines
	Mat draw_img(srcImg.rows, srcImg.cols, CV_8UC3, Scalar(0, 0, 0));
	RNG rng(123);
	Scalar color;
	for (int i = 0; i < lines.size(); i++) {
		color = random_color(rng);
		for (int j = 0; j < lines[i].size(); j++) {
			draw_img.at<Vec3b>(lines[i][j]) = Vec3b((uchar)color[0], (uchar)color[1], (uchar)color[2]);
		}
	}
	imwrite("C:\\Users\\23925\\Desktop\\lines.png", draw_img);

	return 0;
}


void getPoints1(cv::Mat &dstImg, int i, int j, std::vector<Point> &points) {
	Mat_<uchar> tmpImg = dstImg;
	for (int rowNum = i; rowNum < dstImg.rows; ++rowNum) {
		uchar* row = tmpImg.ptr(rowNum);
		for (int colNum = j; colNum < dstImg.cols; ++colNum) {
			if (255 == row[colNum]) {
				points.push_back(Point(colNum, rowNum));
			}
		}
	}
}

void thinImage(const Mat &src, Mat &dst, const int iterations) {
	const int height = src.rows - 1;
	const int width = src.cols - 1;

	//拷贝一个数组给另一个数组
	if (src.data != dst.data) {
		src.copyTo(dst);
	}


	int n = 0, i = 0, j = 0;
	Mat tmpImg;
	uchar *pU, *pC, *pD;
	bool isFinished = false;

	for (n = 0; n < iterations; n++) {
		dst.copyTo(tmpImg);
		isFinished = false;   //一次 先行后列扫描 开始
							  //扫描过程一 开始
		for (i = 1; i < height; i++) {
			pU = tmpImg.ptr<uchar>(i - 1);
			pC = tmpImg.ptr<uchar>(i);
			pD = tmpImg.ptr<uchar>(i + 1);
			for (int j = 1; j < width; j++) {
				if (pC[j] > 0) {
					int ap = 0;
					int p2 = (pU[j] > 0);
					int p3 = (pU[j + 1] > 0);
					if (p2 == 0 && p3 == 1) {
						ap++;
					}
					int p4 = (pC[j + 1] > 0);
					if (p3 == 0 && p4 == 1) {
						ap++;
					}
					int p5 = (pD[j + 1] > 0);
					if (p4 == 0 && p5 == 1) {
						ap++;
					}
					int p6 = (pD[j] > 0);
					if (p5 == 0 && p6 == 1) {
						ap++;
					}
					int p7 = (pD[j - 1] > 0);
					if (p6 == 0 && p7 == 1) {
						ap++;
					}
					int p8 = (pC[j - 1] > 0);
					if (p7 == 0 && p8 == 1) {
						ap++;
					}
					int p9 = (pU[j - 1] > 0);
					if (p8 == 0 && p9 == 1) {
						ap++;
					}
					if (p9 == 0 && p2 == 1) {
						ap++;
					}
					if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) > 1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) < 7) {
						if (ap == 1) {
							if ((p2*p4*p6 == 0) && (p4*p6*p8 == 0)) {
								dst.ptr<uchar>(i)[j] = 0;
								isFinished = true;
							}

							//   if((p2*p4*p8==0)&&(p2*p6*p8==0))
							//    {                           
							//         dst.ptr<uchar>(i)[j]=0;
							//         isFinished =TRUE;                            
							//    }

						}
					}
				}

			} //扫描过程一 结束


			dst.copyTo(tmpImg);
			//扫描过程二 开始
			for (i = 1; i < height; i++)  //一次 先行后列扫描 开始
			{
				pU = tmpImg.ptr<uchar>(i - 1);
				pC = tmpImg.ptr<uchar>(i);
				pD = tmpImg.ptr<uchar>(i + 1);
				for (int j = 1; j < width; j++) {
					if (pC[j] > 0) {
						int ap = 0;
						int p2 = (pU[j] > 0);
						int p3 = (pU[j + 1] > 0);
						if (p2 == 0 && p3 == 1) {
							ap++;
						}
						int p4 = (pC[j + 1] > 0);
						if (p3 == 0 && p4 == 1) {
							ap++;
						}
						int p5 = (pD[j + 1] > 0);
						if (p4 == 0 && p5 == 1) {
							ap++;
						}
						int p6 = (pD[j] > 0);
						if (p5 == 0 && p6 == 1) {
							ap++;
						}
						int p7 = (pD[j - 1] > 0);
						if (p6 == 0 && p7 == 1) {
							ap++;
						}
						int p8 = (pC[j - 1] > 0);
						if (p7 == 0 && p8 == 1) {
							ap++;
						}
						int p9 = (pU[j - 1] > 0);
						if (p8 == 0 && p9 == 1) {
							ap++;
						}
						if (p9 == 0 && p2 == 1) {
							ap++;
						}
						if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) > 1 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) < 7) {
							if (ap == 1) {
								//   if((p2*p4*p6==0)&&(p4*p6*p8==0))
								//   {                           
								//         dst.ptr<uchar>(i)[j]=0;
								//         isFinished =TRUE;                            
								//    }

								if ((p2*p4*p8 == 0) && (p2*p6*p8 == 0)) {
									dst.ptr<uchar>(i)[j] = 0;
									isFinished = true;
								}

							}
						}
					}

				}

			} //一次 先行后列扫描完成          
			  //如果在扫描过程中没有删除点，则提前退出
			if (isFinished == false) {
				break;
			}
		}

	}
}


bool findNextPoint(vector<Point> &_neighbor_points, Mat &_image, Point _inpoint, int flag, Point& _outpoint, int &_outflag) {
	int i = flag;
	int count = 1;
	bool success = false;

	while (count <= 7) {
		Point tmppoint = _inpoint + _neighbor_points[i];
		if (tmppoint.x > 0 && tmppoint.y > 0 && tmppoint.x < _image.cols&&tmppoint.y < _image.rows) {
			if (_image.at<uchar>(tmppoint) == 255) {
				_outpoint = tmppoint;
				_outflag = i;
				success = true;
				_image.at<uchar>(tmppoint) = 0;
				break;
			}
		}
		if (count % 2) {
			i += count;
			if (i > 7) {
				i -= 8;
			}
		}
		else {
			i += -count;
			if (i < 0) {
				i += 8;
			}
		}
		count++;
	}
	return success;
}
//寻找图像上的第一个点
bool findFirstPoint(Mat &_inputimg, Point &_outputpoint) {
	bool success = false;
	for (int i = 0; i < _inputimg.rows; i++) {
		uchar* data = _inputimg.ptr<uchar>(i);
		for (int j = 0; j < _inputimg.cols; j++) {
			if (data[j] == 255) {
				success = true;
				_outputpoint.x = j;
				_outputpoint.y = i;
				data[j] = 0;
				break;
			}
		}
		if (success)
			break;
	}
	return success;
}
//寻找曲线 
void findLines(Mat &_inputimg, vector<deque<Point>> &_outputlines) {
	vector<Point> neighbor_points = { Point(-1,-1),Point(0,-1),Point(1,-1),Point(1,0),Point(1,1),Point(0,1),Point(-1,1),Point(-1,0) };
	Point first_point;
	while (findFirstPoint(_inputimg, first_point)) {
		deque<Point> line;
		line.push_back(first_point);
		//由于第一个点不一定是线段的起始位置，双向找
		Point this_point = first_point;
		int this_flag = 0;
		Point next_point;
		int next_flag;
		while (findNextPoint(neighbor_points, _inputimg, this_point, this_flag, next_point, next_flag)) {
			line.push_back(next_point);
			this_point = next_point;
			this_flag = next_flag;
		}
		//找另一边
		this_point = first_point;
		this_flag = 0;
		//cout << "flag:" << this_flag << endl;
		while (findNextPoint(neighbor_points, _inputimg, this_point, this_flag, next_point, next_flag)) {
			line.push_front(next_point);
			this_point = next_point;
			this_flag = next_flag;
		}
		if (line.size() > 20) {// 10
			_outputlines.push_back(line);
		}
	}
}
//随机取色 用于画线的时候
Scalar random_color(RNG& _rng) {
	int icolor = (unsigned)_rng;
	return Scalar(icolor & 0xFF, (icolor >> 8) & 0xFF, (icolor >> 16) & 0xFF);
}
int main1() {
	Mat image = imread("images\\2.bmp");
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	vector<deque<Point>> lines;
	findLines(gray, lines);
	cout << lines.size() << endl;
	//draw lines
	Mat draw_img = image.clone();
	RNG rng(123);
	Scalar color;
	for (int i = 0; i < lines.size(); i++) {
		color = random_color(rng);
		for (int j = 0; j < lines[i].size(); j++) {
			draw_img.at<Vec3b>(lines[i][j]) = Vec3b(color[0], color[1], color[2]);
		}
	}
	//imshow("draw_img", draw_img);
	imwrite("images\\draw_img.bmp", draw_img);
	waitKey(0);
	system("pause");
	return 0;
}