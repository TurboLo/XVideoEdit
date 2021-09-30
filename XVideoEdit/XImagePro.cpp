#include "XImagePro.h"

void XImagePro::set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty()) return;
	src1 = mat1;
	src2 = mat2;
	src1.copyTo(des);
}

void XImagePro::gain(double bright, double contrast)
{
	if (des.empty()) return;
	des.convertTo(des, -1, contrast, bright);
}

void XImagePro::rotate90()
{
	if (des.empty()) return;
	cv::rotate(des, des, cv::ROTATE_90_CLOCKWISE);
}

void XImagePro::rotate180()
{
	if (des.empty()) return;
	cv::rotate(des, des, cv::ROTATE_180);
}

void XImagePro::rotate270()
{
	if (des.empty()) return;
	cv::rotate(des, des, cv::ROTATE_90_COUNTERCLOCKWISE);
}

void XImagePro::flipX()
{
	if (des.empty()) return;
	cv::flip(des, des, 1);
}

void XImagePro::flipY()
{
	if (des.empty()) return;
	cv::flip(des, des, 0);
}

void XImagePro::flipXY()
{
	if (des.empty()) return;
	cv::flip(des, des, -1);
}

void XImagePro::myResize(double width, double height)
{
	if (des.empty()) return;
	cv::resize(des, des, cv::Size(width,height));
}

void XImagePro::pyDown(int times)
{
	if (des.empty()) return;
	for (int i = 0; i < times; i++)
	{
		cv::pyrDown(des, des);
	}
}

void XImagePro::pyUp(int times)
{
	if (des.empty()) return;
	for (int i = 0; i < times; i++)
	{
		cv::pyrUp(des, des);
	}
}

void XImagePro::clip(int x, int y, int w, int h)
{
	if (des.empty()) return;
	if (x < 0 || y < 0 || w <= 0 || h <= 0) return;
	if (x > des.cols || y > des.rows)return;
	des = des(cv::Rect(x, y, w, h));
}

void XImagePro::gray()
{
	if (des.empty()) return;
	cv::cvtColor(des, des, cv::COLOR_BGR2GRAY);
}

void XImagePro::mark(int x, int y, double a)
{
	if (des.empty()) return;
	if (src2.empty()) return;
	cv::Mat rol = des(cv::Rect(x, y, src2.cols, src2.rows));
	cv::addWeighted(src2, a, rol, 1 - a, 0, rol);
}

void XImagePro::blend(double a)
{
	if (des.empty()) return;
	if (src2.empty()) return;
	if (src2.size() != des.size())
	{
		cv::resize(src2, src2, des.size());
	}
	cv::addWeighted(src2, a, des, 1 - a, 0, des);
}

void XImagePro::merge()
{
	if (des.empty()) return;
	if (src2.empty()) return;
	if (src2.size() != des.size())
	{
		int w = src2.cols * ((double)src2.rows / double(des.rows));
		cv::resize(src2, src2, cv::Size(w,des.rows));
	}
	int dw = src1.cols + src2.cols;
	int dh = src1.rows;
	des = cv::Mat(cv::Size(dw, dh), src1.type());

	cv::Mat r1 = des(cv::Rect(0, 0, src1.cols, dh));
	cv::Mat r2 = des(cv::Rect(src1.cols, 0, src2.cols, dh));
	src1.copyTo(r1);
	src2.copyTo(r2);
}

XImagePro::XImagePro()
{

}

XImagePro::~XImagePro()
{

}
