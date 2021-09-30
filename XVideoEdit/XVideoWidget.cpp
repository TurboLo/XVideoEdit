#include "XVideoWidget.h"
#include <QPainter>
XVideoWidget::XVideoWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{

}

XVideoWidget::~XVideoWidget()
{

}

void XVideoWidget::paintEvent(QPaintEvent* e)
{
	QPainter p;
	p.begin(this);
	p.drawImage(0, 0, img);
	p.end();
}

void XVideoWidget::dealImage1(cv::Mat mat)
{
	if (mat.empty())
	{
		return;
	}

	if (img.isNull())
	{
		uchar* buf = new uchar[width() * height() * 3];
		img = QImage(buf, width(), height(), QImage::Format_RGB888);
	}
	cv::Mat des;
	
	cv::resize(mat, des, cv::Size(img.size().width(), img.size().height()));
	cv::cvtColor(des, des, cv::COLOR_BGR2RGB);
	memcpy(img.bits(), des.data, des.cols * des.rows * des.elemSize());
	update();
}
