#pragma once
#include <opencv2/core.hpp>
#include <vector>
#include "XImagePro.h"
enum xTaskType
{
	XTASK_NONE,
	XTASK_GAIN,
	XTASK_ROTATE90,
	XTASK_ROTATE180,
	XTASK_ROTATE270,
	XTASK_FLIPX,
	XTASK_FLIPY,
	XTASK_FLIPXY,
	XTASK_RESIZE,
	XTASK_PYDOWN,
	XTASK_PYUP,
	XTASK_CLIP,
	XTASK_GRAY,
	XTASK_MARK,
	XTASK_BLEND,
	XTASK_MERGE,
};
struct xTask
{
	xTaskType type;
	std::vector<double> para;
};
class XFilter
{
public:
	virtual cv::Mat filter(cv::Mat mat1, cv::Mat mat2) = 0;
	virtual void add(xTask task) = 0;
	virtual void clear() = 0;
	static XFilter& instance();
	virtual ~XFilter();
protected:
	XFilter();
private:
	
};

