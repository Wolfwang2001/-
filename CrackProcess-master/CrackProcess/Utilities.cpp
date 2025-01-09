#include "stdafx.h"
#include "Utilities.h"

namespace Custom {
	using cv::Mat;
	using cv::Mat_;
	using cv::MatIterator_;
	using cv::Vec4b;
	using cv::Point;
	using cv::Scalar;
	using cv::String;
	using cv::LUT;
	using cv::RotatedRect;
	using cv::minAreaRect;
	using std::vector;
	using std::deque;
	using std::stack;
	using cv::saturate_cast;

	Utilities::Utilities() {}

	Utilities::~Utilities() {}

	void Utilities::addContrast(Mat & srcImg) {
		Mat lookUpTable(1, 256, CV_8U);
		double temp = pow(1.1, 5);
		uchar* p = lookUpTable.data;
		for (int i = 0; i < 256; ++i)
			p[i] = saturate_cast<uchar>(i * temp);
		LUT(srcImg, lookUpTable, srcImg);
	}

	void Utilities::findConnectedDomain(Mat & srcImg, vector<vector<Point>>& connectedDomains, int area, int WHRatio) {
		Mat_<uchar> tempImg = (Mat_<uchar> &)srcImg;

		for (int i = 0; i < tempImg.rows; ++i) {
			uchar* row = tempImg.ptr(i);
			for (int j = 0; j < tempImg.cols; ++j) {
				if (row[j] == 255) {
					stack<Point> connectedPoints;
					vector<Point> domain;
					connectedPoints.push(Point(j, i));
					while (!connectedPoints.empty()) {
						Point currentPoint = connectedPoints.top();
						domain.push_back(currentPoint);

						int colNum = currentPoint.x;
						int rowNum = currentPoint.y;

						tempImg.ptr(rowNum)[colNum] = 0;
						connectedPoints.pop();

						if (rowNum - 1 >= 0 && colNum - 1 >= 0 && tempImg.ptr(rowNum - 1)[colNum - 1] == 255) {
							tempImg.ptr(rowNum - 1)[colNum - 1] = 0;
							connectedPoints.push(Point(colNum - 1, rowNum - 1));
						}
						if (rowNum - 1 >= 0 && tempImg.ptr(rowNum - 1)[colNum] == 255) {
							tempImg.ptr(rowNum - 1)[colNum] = 0;
							connectedPoints.push(Point(colNum, rowNum - 1));
						}
						if (rowNum - 1 >= 0 && colNum + 1 < tempImg.cols && tempImg.ptr(rowNum - 1)[colNum + 1] == 255) {
							tempImg.ptr(rowNum - 1)[colNum + 1] = 0;
							connectedPoints.push(Point(colNum + 1, rowNum - 1));
						}
						if (colNum - 1 >= 0 && tempImg.ptr(rowNum)[colNum - 1] == 255) {
							tempImg.ptr(rowNum)[colNum - 1] = 0;
							connectedPoints.push(Point(colNum - 1, rowNum));
						}
						if (colNum + 1 < tempImg.cols && tempImg.ptr(rowNum)[colNum + 1] == 255) {
							tempImg.ptr(rowNum)[colNum + 1] = 0;
							connectedPoints.push(Point(colNum + 1, rowNum));
						}
						if (rowNum + 1 < tempImg.rows && colNum - 1 > 0 && tempImg.ptr(rowNum + 1)[colNum - 1] == 255) {
							tempImg.ptr(rowNum + 1)[colNum - 1] = 0;
							connectedPoints.push(Point(colNum - 1, rowNum + 1));
						}
						if (rowNum + 1 < tempImg.rows && tempImg.ptr(rowNum + 1)[colNum] == 255) {
							tempImg.ptr(rowNum + 1)[colNum] = 0;
							connectedPoints.push(Point(colNum, rowNum + 1));
						}
						if (rowNum + 1 < tempImg.rows && colNum + 1 < tempImg.cols && tempImg.ptr(rowNum + 1)[colNum + 1] == 255) {
							tempImg.ptr(rowNum + 1)[colNum + 1] = 0;
							connectedPoints.push(Point(colNum + 1, rowNum + 1));
						}
					}
					if (domain.size() > area) {
						RotatedRect rect = minAreaRect(domain);
						float width = rect.size.width;
						float height = rect.size.height;
						if (width < height) {
							float temp = width;
							width = height;
							height = temp;
						}
						if (width > height * WHRatio && width > 50) {
							for (auto cit = domain.begin(); cit != domain.end(); ++cit) {
								tempImg.ptr(cit->y)[cit->x] = 250;
							}
							connectedDomains.push_back(domain);
						}
					}
				}
			}
		}

		Utilities::binaryzation(srcImg);
	}

	void Utilities::thinImage(Mat & srcImg) {
		vector<Point> deleteList;
		int neighbourhood[9];
		int nl = srcImg.rows;
		int nc = srcImg.cols;
		bool inOddIterations = true;
		while (true) {
			for (int j = 1; j < (nl - 1); j++) {
				uchar* data_last = srcImg.ptr<uchar>(j - 1);
				uchar* data = srcImg.ptr<uchar>(j);
				uchar* data_next = srcImg.ptr<uchar>(j + 1);
				for (int i = 1; i < (nc - 1); i++) {
					if (data[i] == 255) {
						int whitePointCount = 0;
						neighbourhood[0] = 1;
						if (data_last[i] == 255) neighbourhood[1] = 1;
						else  neighbourhood[1] = 0;
						if (data_last[i + 1] == 255) neighbourhood[2] = 1;
						else  neighbourhood[2] = 0;
						if (data[i + 1] == 255) neighbourhood[3] = 1;
						else  neighbourhood[3] = 0;
						if (data_next[i + 1] == 255) neighbourhood[4] = 1;
						else  neighbourhood[4] = 0;
						if (data_next[i] == 255) neighbourhood[5] = 1;
						else  neighbourhood[5] = 0;
						if (data_next[i - 1] == 255) neighbourhood[6] = 1;
						else  neighbourhood[6] = 0;
						if (data[i - 1] == 255) neighbourhood[7] = 1;
						else  neighbourhood[7] = 0;
						if (data_last[i - 1] == 255) neighbourhood[8] = 1;
						else  neighbourhood[8] = 0;
						for (int k = 1; k < 9; k++) {
							whitePointCount = whitePointCount + neighbourhood[k];
						}
						if ((whitePointCount >= 2) && (whitePointCount <= 6)) {
							int ap = 0;
							if ((neighbourhood[1] == 0) && (neighbourhood[2] == 1)) ap++;
							if ((neighbourhood[2] == 0) && (neighbourhood[3] == 1)) ap++;
							if ((neighbourhood[3] == 0) && (neighbourhood[4] == 1)) ap++;
							if ((neighbourhood[4] == 0) && (neighbourhood[5] == 1)) ap++;
							if ((neighbourhood[5] == 0) && (neighbourhood[6] == 1)) ap++;
							if ((neighbourhood[6] == 0) && (neighbourhood[7] == 1)) ap++;
							if ((neighbourhood[7] == 0) && (neighbourhood[8] == 1)) ap++;
							if ((neighbourhood[8] == 0) && (neighbourhood[1] == 1)) ap++;
							if (ap == 1) {
								if (inOddIterations && (neighbourhood[3] * neighbourhood[5] * neighbourhood[7] == 0)
									&& (neighbourhood[1] * neighbourhood[3] * neighbourhood[5] == 0)) {
									deleteList.push_back(Point(i, j));
								}
								else if (!inOddIterations && (neighbourhood[1] * neighbourhood[5] * neighbourhood[7] == 0)
									&& (neighbourhood[1] * neighbourhood[3] * neighbourhood[7] == 0)) {
									deleteList.push_back(Point(i, j));
								}
							}
						}
					}
				}
			}
			if (deleteList.size() == 0)
				break;
			for (size_t i = 0; i < deleteList.size(); i++) {
				Point tem;
				tem = deleteList[i];
				uchar* data = srcImg.ptr<uchar>(tem.y);
				data[tem.x] = 0;
			}
			deleteList.clear();

			inOddIterations = !inOddIterations;
		}
	}

	void Utilities::swapMat(Mat & srcImg, Mat & dstImg) {
		Mat tempImg = srcImg;
		srcImg = dstImg;
		dstImg = tempImg;
	}

	void Utilities::save2PNG(Mat & srcImg, const String & fileName, int red, int green, int blue) {
		Mat dstImg(srcImg.rows, srcImg.cols, CV_8UC4, Scalar(0, 0, 0, 0));
		MatIterator_<uchar> srcIt = srcImg.begin<uchar>();
		MatIterator_<uchar> srcEnd = srcImg.end<uchar>();
		MatIterator_<Vec4b> dstIt = dstImg.begin<Vec4b>();
		for (; srcIt != srcEnd; ++srcIt, ++dstIt) {
			if (*srcIt > 0) {
				(*dstIt)[0] = blue;
				(*dstIt)[1] = green;
				(*dstIt)[2] = red;
				(*dstIt)[3] = 255;
			}
		}
		imwrite(fileName, dstImg);
	}

	void Utilities::binaryzation(Mat & srcImg) {
		Mat lookUpTable(1, 256, CV_8U, Scalar(255));
		lookUpTable.data[0] = 0;
		LUT(srcImg, lookUpTable, srcImg);
	}

	void Utilities::getWhitePoints(Mat &srcImg, vector<Point>& domain) {
		domain.clear();
		Mat_<uchar> tempImg = (Mat_<uchar> &)srcImg;
		for (int i = 0; i < tempImg.rows; i++) {
			uchar * row = tempImg.ptr<uchar>(i);
			for (int j = 0; j < tempImg.cols; ++j) {
				if (row[j] != 0)
					domain.push_back(Point(j, i));
			}
		}
	}

	cv::Point Utilities::calInfoPosition(int imgRows, int imgCols, int padding, const std::vector<cv::Point>& domain) {
		long xSum = 0;
		long ySum = 0;
		for (auto it = domain.cbegin(); it != domain.cend(); ++it) {
			xSum += it->x;
			ySum += it->y;
		}
		int x = 0;
		int y = 0;
		x = (int)(xSum / domain.size());
		y = (int)(ySum / domain.size());
		if (x < padding)
			x = padding;
		if (x > imgCols - padding)
			x = imgCols - padding;
		if (y < padding)
			y = padding;
		if (y > imgRows - padding)
			y = imgRows - padding;

		return cv::Point(x, y);
	}

	void Utilities::fillCrack(Mat & srcImg, int width) {
		Mat_<uchar> addedContrastImg = (Mat_<uchar> &)srcImg;
		for (int i = 0; i < addedContrastImg.rows; ++i) {
			uchar* row = addedContrastImg.ptr(i);
			for (int j = 0; j < addedContrastImg.cols - width; ++j) {
				int start = 0, end = 0;
				for (int delta = j; delta < j + width; ++delta) {
					if (255 == row[delta]) {
						start = delta;
						break;
					}
				}
				if (start == 0 || start == j + width - 1)
					continue;
				for (int delta = start + 1; delta < j + width; ++delta) {
					if (255 == row[delta]) {
						end = delta;
						break;
					}
				}
				if (0 == end)
					continue;
				for (; start <= end; ++start) {
					row[start] = 255;
				}
				j = end - 1;
			}
		}
	}

	void Utilities::getPoints(Mat & srcImg, int i, int j, std::vector<Point>& points, int squareWidth) {
		Mat_<uchar> tmpImg = (Mat_<uchar> &)srcImg;
		int rowLimit = (i + squareWidth) < srcImg.rows ? (i + squareWidth) : srcImg.rows;
		int colLimit = (j + squareWidth) < srcImg.cols ? (j + squareWidth) : srcImg.cols;
		for (int rowNum = i; rowNum < rowLimit; ++rowNum) {
			uchar* row = tmpImg.ptr(rowNum);
			for (int colNum = j; colNum < colLimit; ++colNum) {
				if (255 == row[colNum]) {
					points.push_back(Point(colNum - j, rowNum - i));
				}
			}
		}
	}

	void Utilities::recalculatePoints(int row, int col, std::vector<Point> &points) {
		for (auto begin = points.begin(); begin != points.end(); ++begin) {
			begin->x -= col;
			begin->y -= row;
		}
	}

	void Utilities::printPoints(std::vector<Point> const & points) {
		std::ofstream out("D:\\OctaveData\\points.txt");
		for (auto begin = points.cbegin(); begin != points.cend(); ++begin) {
			out << begin->x << " " << begin->y << " " << std::endl;
		}
	}

	void Utilities::clearArea(Mat & srcImg, int i, int j, int squareWidth) {
		Mat_<uchar> tempImg = (Mat_<uchar> &)srcImg;
		int rowLimit = (i + squareWidth) < srcImg.rows ? (i + squareWidth) : srcImg.rows;
		int colLimit = (j + squareWidth) < srcImg.cols ? (i + squareWidth) : srcImg.cols;
		for (int rowNum = i; rowNum < rowLimit; ++rowNum) {
			uchar* row = tempImg.ptr(rowNum);
			for (int colNum = j; colNum < colLimit; ++colNum) {
				row[colNum] = 0;
			}
		}
	}

	bool Utilities::findNextPoint(vector<Point> &_neighbor_points, Mat &_image, Point _inpoint, int flag, Point& _outpoint, int &_outflag) {
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

	bool Utilities::findFirstPoint(Mat &srcImg, Point &_outputpoint) {
		bool success = false;
		for (int i = 0; i < srcImg.rows; i++) {
			uchar* data = srcImg.ptr<uchar>(i);
			for (int j = 0; j < srcImg.cols; j++) {
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

	void Utilities::findLines(Mat & srcImg, vector<deque<Point>>& _outputlines) {
		vector<Point> neighbor_points = { Point(-1,-1),Point(0,-1),Point(1,-1),Point(1,0),Point(1,1),Point(0,1),Point(-1,1),Point(-1,0) };
		Point first_point;
		while (findFirstPoint(srcImg, first_point)) {
			deque<Point> line;
			line.push_back(first_point);
			//由于第一个点不一定是线段的起始位置，双向找
			Point this_point = first_point;
			int this_flag = 0;
			Point next_point;
			int next_flag;
			while (findNextPoint(neighbor_points, srcImg, this_point, this_flag, next_point, next_flag)) {
				line.push_back(next_point);
				this_point = next_point;
				this_flag = next_flag;
			}
			//找另一边
			this_point = first_point;
			this_flag = 0;
			//cout << "flag:" << this_flag << endl;
			while (findNextPoint(neighbor_points, srcImg, this_point, this_flag, next_point, next_flag)) {
				line.push_front(next_point);
				this_point = next_point;
				this_flag = next_flag;
			}
			if (line.size() > 10) {
				_outputlines.push_back(line);
			}
		}
	}

}