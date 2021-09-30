#pragma once
#include <QOpenGLWidget>
#include <opencv2/core.hpp>
#include <QImage>
#include <opencv2/imgproc.hpp>
class XVideoWidget :public QOpenGLWidget
{
	Q_OBJECT
public:
	XVideoWidget(QWidget* parent = nullptr);
	~XVideoWidget();
private slots:
	void dealImage1(cv::Mat mat);
protected:
	void paintEvent(QPaintEvent* e) override;
private:
	QImage img;
};

