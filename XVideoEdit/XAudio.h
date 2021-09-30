#pragma once
#include <string>
#include <QMutex>
class XAudio
{
public:
	static XAudio& instance();
	virtual bool exportA(std::string src,std::string out,int beginms =0,int endms=0) = 0;
	
	virtual bool merge(std::string v, std::string a, std::string out) = 0;

	virtual ~XAudio();
protected:
	XAudio();
private:
	
};

