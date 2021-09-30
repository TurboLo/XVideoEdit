#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

static cv::VideoCapture cap1;
static cv::VideoCapture cap2;
static bool isExit = false;
static cv::VideoWriter vw;
XVideoThread& XVideoThread::instance()
{
	static XVideoThread instance;
	return instance;
}
XVideoThread::XVideoThread()
{

}

XVideoThread::~XVideoThread()
{
	mutex.lock();
	isExit = true;
	mutex.unlock();
	wait();
}

void XVideoThread::stopSave()
{
	mutex.lock();
	vw.release();
	isWrite = false;
	mutex.unlock();
}

bool XVideoThread::startSave(const std::string& fileName, int width, int height, bool color)
{
	seek(begin);
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	if (width <= 0)
	{
		width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
	}
	if (height <= 0)
	{
		height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);
	}
	vw.open(fileName, cv::VideoWriter::fourcc('X', '2', '6', '4'),
		fps,
		cv::Size(width, height),
		color
	);
	if (!vw.isOpened())
	{
		mutex.unlock();
		return false;
	}
	isWrite = true;
	mutex.unlock();
	return isWrite;
}

void XVideoThread::run()
{
	cv::Mat mat1;
	for (;;)
	{
		mutex.lock();
		if (isExit)
		{
			mutex.unlock();
			break;
		}

		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		if (isPause)
		{
			mutex.unlock();
			msleep(5);
			continue;
		}

		int cur = cap1.get(cv::CAP_PROP_POS_FRAMES);

		if ((end>0&&cur>=end) || !cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			if (isWrite)
			{
				stopSave();
				emit saveEnd();
				isWrite = false;
			}
			msleep(5);
			continue;
		}
		if (!isWrite)
		{
			emit sig_viewImage1(mat1);
		}
		cv::Mat mat2 = mark;
		if (cap2.isOpened())
		{
			cap2.read(mat2);
		}
		if (!isWrite)
		{
			emit sig_viewImage3(mat2);
		}

		cv::Mat des = XFilter::instance().filter(mat1, mat2);
		if (!isWrite)
		{
			emit sig_viewImage2(des);
		}

		int s = 0;
		s = 1000 / fps;
		if (isWrite)
		{
			s = 1;
			vw.write(des);
		}
		if (!isWrite)
		{
			msleep(s);
		}
		mutex.unlock();
	}
}

bool XVideoThread::openFileOne(const std::string& str)
{
	seek(0);
	mutex.lock();
	int ret = cap1.open(str);
	mutex.unlock();
	if (!ret)
	{
		return ret;
	}
	fps = cap1.get(cv::CAP_PROP_FPS);
	width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
	height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0) fps = 25;

	return ret;
}

bool XVideoThread::openFileTwo(const std::string& str)
{
	seek(0);
	mutex.lock();
	int ret = cap2.open(str);
	mutex.unlock();
	if (!ret)
	{
		return ret;
	}
	fps = cap2.get(cv::CAP_PROP_FPS);
	width2 = cap2.get(cv::CAP_PROP_FRAME_WIDTH);
	height2 = cap2.get(cv::CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0) fps = 25;

	return ret;
}

double XVideoThread::getPos()
{
	double pos = 0;
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return pos;
	}

	double count = cap1.get(cv::CAP_PROP_FRAME_COUNT);
	double cur = cap1.get(cv::CAP_PROP_POS_FRAMES);
	width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
	height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);
	if (count > 0.001)
	{
		pos = cur / count;
	}
	mutex.unlock();
	return pos;
}

bool XVideoThread::seek(int frame)
{
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	int re = cap1.set(cv::CAP_PROP_POS_FRAMES, frame);

	if (cap1.isOpened())
	{
		int re = cap2.set(cv::CAP_PROP_POS_FRAMES, frame);
	}
	
	mutex.unlock();
	return re;
}

bool XVideoThread::seek(double pos)
{
	double count = cap1.get(cv::CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	return seek(frame);
}

void XVideoThread::setBegin(double p) 
{ 
	mutex.lock(); 
	double count = cap1.get(cv::CAP_PROP_FRAME_COUNT);
	int frame = p * count;
	begin = frame;
	mutex.unlock(); 
}


void XVideoThread::setEnd(double p)
{
	mutex.lock();
	double count = cap1.get(cv::CAP_PROP_FRAME_COUNT);
	int frame = p * count;
	end = frame;
	mutex.unlock();
}