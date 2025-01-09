#include "stdafx.h"
#include "MathFunction.h"

namespace Custom {
	MathFunction::MathFunction() {}

	MathFunction::~MathFunction() {}

	double MathFunction::calDeviation(std::vector<cv::Point> const & points, double a0, double a1) {
		std::vector<double> variances;
		for (auto begin = points.cbegin(); begin != points.cend(); ++begin) {
			double variance = begin->y - a0 - a1 * begin->x;
			variances.push_back(variance);
		}
		double varianceAvg = std::accumulate(variances.begin(), variances.end(), double(0)) / variances.size();
		/*double sum = 0;
		for (auto begin = variances.begin(); begin != variances.end(); ++begin)
			sum += pow((*begin - varianceAvg), 2);
		return sum / (points.size() - 1);*/
		return std::accumulate(variances.begin(), variances.end(), double(0), 
			[varianceAvg](double v1, double v2) { return v1 + pow((v2 - varianceAvg), 2); }
		) / (variances.size() - 1);
	}

	void MathFunction::calParameters(std::vector<cv::Point> const & points, double & xySum, double & xxSum, double & xSum, double & ySum) {
		for (auto begin = points.cbegin(); begin != points.cend(); ++begin) {
			xySum += begin->x*begin->y;
			xxSum += begin->x*begin->x;
			xSum += begin->x;
			ySum += begin->y;
		}
	}

	void MathFunction::leastSquareProcedure(std::vector<cv::Point> const & points, double & a0, double & a1) {
		int n = (int)points.size();
		double xySum = 0;
		double xxSum = 0;
		double xSum = 0;
		double ySum = 0;
		double xAvg = 0;
		double yAvg = 0;
		calParameters(points, xySum, xxSum, xSum, ySum);
		xAvg = xSum / n;
		yAvg = ySum / n;
		a1 = (n*xySum - xSum * ySum) / (n*xxSum - xSum * xSum);
		a0 = yAvg - a1 * xAvg;
	}

	myPoint::myPoint() {}

	myPoint::~myPoint() {}
	myPoint::myPoint(double _x, double _y) :x(_x), y(_y) {}
}