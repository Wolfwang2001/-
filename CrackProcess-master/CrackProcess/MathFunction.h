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

		/* ����ģ�ͺ͹۲�ֵ�����ķ��� */
		static double calDeviation(std::vector<cv::Point> const &points, double a0, double a1);

		/* ������С���˷��Ĳ��� */
		static void calParameters(std::vector<cv::Point> const &points, double& xySum, double& xxSum, double& xSum, double& ySum);

		/* ���ݸ����ĵ�ʹ����С���˷���ϳ�һ��ֱ�ߣ����ֱ�ߵ�б�ʺͽؾ� */
		static void leastSquareProcedure(std::vector<cv::Point> const &points, double &a0, double &a1);
	};

}