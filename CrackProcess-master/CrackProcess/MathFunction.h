#pragma once

namespace Custom {
	class myPoint
	{
	public:
		myPoint();
		~myPoint();
		myPoint(double _x, double _y);
		double x;
		double y;
	private:

	};

	class MathFunction
	{
	public:
		MathFunction();
		~MathFunction();

		/* 计算模型和观测值间误差的方差 */
		static double calDeviation(std::vector<cv::Point> const &points, double a0, double a1);

		/* 计算最小二乘法的参数 */
		static void calParameters(std::vector<cv::Point> const &points, double& xySum, double& xxSum, double& xSum, double& ySum);

		/* 根据给定的点使用最小二乘法拟合出一条直线，获得直线的斜率和截距 */
		static void leastSquareProcedure(std::vector<cv::Point> const &points, double &a0, double &a1);
	};

}