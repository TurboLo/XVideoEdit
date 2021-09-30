#pragma once
#include <QThread>
#include <QMutex>
#include <opencv2/core.hpp>
#include "XFilter.h"
class XVideoThread:public QThread
{
	Q_OBJECT
public:
	int fps = 0;
	int width = 0;
	int height = 0;
	int width2 = 0;
	int height2 = 0;

	int begin = 0;
	int end = 0;

	static XVideoThread& instance();
	
	~XVideoThread();
	bool openFileOne(const std::string& str);
	bool openFileTwo(const std::string& str);
	bool startSave(const std::string &fileName,int width =0,int height =0, bool color = true);

	void stopSave();

	void run();
	double getPos();
	bool seek(int frame);
	bool seek(double pos);
	void setMark(cv::Mat m) { mutex.lock(); mark = m; mutex.unlock(); };
	void Play() { mutex.lock(); isPause = false; mutex.unlock(); };
	void Pause() { mutex.lock(); isPause = true ; mutex.unlock(); };

	void setBegin(double p);
	void setEnd(double p);

signals:
	void sig_viewImage1(cv::Mat mat);
	
	void sig_viewImage2(cv::Mat mat);
	void sig_viewImage3(cv::Mat mat);
	void saveEnd();
protected:
	
private:
	QMutex mutex;
	bool isWrite = false;
	cv::Mat mark;
	XVideoThread();
	bool isPause = true;
};

