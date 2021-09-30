#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
class XImagePro
{
public:
	XImagePro();
	~XImagePro();

	void set(cv::Mat mat1, cv::Mat mat2);

	cv::Mat get() { return des; }

	void gain(double bright, double contrast);

	void rotate90();
	void rotate180();
	void rotate270();

	void flipX();
	void flipY();
	void flipXY();

	void myResize(double width,double height);

	void pyDown(int times);
	void pyUp(int times);

	void clip(int x,int y,int w,int h);
	void gray();

	void mark(int x,int y,double a);

	void blend(double a);

	void merge();

protected:

private:
	cv::Mat src1, src2;

	cv::Mat des;
};

