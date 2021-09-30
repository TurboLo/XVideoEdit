#include "XAudio.h"
#include <iostream>

struct XTime {
	XTime(int hms) 
	{
		h = (hms / 1000) / 3600;
		m = ((hms / 1000) % 3600)/60;
		s = (hms / 1000) % (3600 * 60);
		ms = hms % 1000;
	}
	std::string toStr()
	{
		char buf[16] = {0};
		sprintf(buf,"%d:%d:%d:%d",h,m,s,ms);
		return buf;
	}
	int h;
	int m;
	int s;
	int ms;


};

class CXAudio :public XAudio
{
public:
	QMutex mutex;
	bool exportA(std::string src, std::string out, int beginms, int endms)
	{
		std::string cmd = "ffmpeg";
		cmd += " -i ";
		cmd += src;
		cmd += " ";
		if (beginms != 0)
		{
			cmd += " -ss ";
			XTime xt(beginms);
			cmd += xt.toStr();
		}

		if (endms != 0)
		{
			cmd += " -t ";
			XTime xt(endms);
			cmd += xt.toStr();
		}
		cmd += " -vn -y ";
		cmd += out;
		std::cout << cmd << std::endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();
		return true;
	}
	bool merge(std::string v, std::string a, std::string out)
	{

		std::string cmd = "ffmpeg";
		cmd += " -i ";
		cmd += v;
		cmd += " -i ";
		cmd += a;
		cmd += " -c copy ";
		cmd += out;
		std::cout << cmd << std::endl;
		mutex.lock();
		system(cmd.c_str());
		mutex.unlock();

		return true;
	}
};


XAudio::XAudio()
{

}

XAudio::~XAudio()
{

}

XAudio& XAudio::instance()
{
	static CXAudio instance;
	return instance;
}
