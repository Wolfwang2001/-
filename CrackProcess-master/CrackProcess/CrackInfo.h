#pragma once
namespace Custom {
	class CrackInfo
	{
		friend std::ostream& operator << (std::ostream&, const CrackInfo&);
	public:
		CrackInfo();
		~CrackInfo();
		CrackInfo(cv::Point& position, long length, float width);
		cv::Point Position;
	private:
		long Length;
		float Width;
		int X;
		int Y;
	};
}