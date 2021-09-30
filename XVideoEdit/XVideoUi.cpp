#include "XVideoUi.h"
#include <QFileDialog>
#include <iostream>

static bool pressSlider;
static bool isExport = false;
static bool isColor = true;
static bool isMark = false;
static bool isBlend = false;
static bool isMerge = false;
XVideoUi::XVideoUi(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    initUi();
    qRegisterMetaType<cv::Mat>("cv::Mat");
    connect(&XVideoThread::instance(), SIGNAL(sig_viewImage1(cv::Mat)), ui.src1Video, SLOT(dealImage1(cv::Mat)));
    connect(&XVideoThread::instance(), SIGNAL(sig_viewImage2(cv::Mat)), ui.src2Video, SLOT(dealImage1(cv::Mat)));
    connect(&XVideoThread::instance(), SIGNAL(sig_viewImage3(cv::Mat)), ui.src3Video, SLOT(dealImage1(cv::Mat)));
    connect(&XVideoThread::instance(), SIGNAL(ExportEnd()), ui.src2Video, SLOT(ExportEnd()));
    startTimer(25);
}

void XVideoUi::SliderRelease()
{
    pressSlider = false;
}

void XVideoUi::SliderMove(int pos)
{
    XVideoThread::instance().seek((double)pos / 1000);
}

void XVideoUi::SliderPress()
{
	pressSlider = true;
}

void XVideoUi::initUi()
{
    setWindowFlag(Qt::FramelessWindowHint);
}

void XVideoUi::on_pushButtonClose_clicked()
{
    close();
}

void XVideoUi::timerEvent(QTimerEvent* e)
{
    if (pressSlider)return;
    double pos = XVideoThread::instance().getPos();
    ui.SliderMovie->setValue(pos * 1000);
}

void XVideoUi::on_pushButtonOpenFile_clicked()
{
    QString name = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("ÇëÑ¡ÔñÎÄ¼þ¼Ð"),"./");
    if (name.isEmpty()) return;
    std::string file = name.toLocal8Bit().data();
    if (XVideoThread::instance().openFileOne(file))
    {
        XVideoThread::instance().start();
    }
}

void XVideoUi::Set()
{
    XFilter::instance().clear();
    bool isclip = false;
    isColor = true;
    double cmx = ui.cX->value();
    double cmy = ui.cY->value();
    double cmw = ui.cW->value();
    double cmh = ui.cH->value();

    if (!isMerge&&cmx + cmy + cmw + cmh > 0.0001)
    {
        isclip = true;
        double w = XVideoThread::instance().width;
        double h = XVideoThread::instance().height;
		XFilter::instance().add(xTask{ XTASK_CLIP,
			{ cmx , cmy , cmw , cmh
			} });
		XFilter::instance().add(xTask{ XTASK_RESIZE,
			{ w,h
			} });
    }

    if (isclip)
    {
        return;
    }

    bool isPy = false;
	if (ui.pyDown->value() > 0)
	{
        isPy = true;
		XFilter::instance().add(xTask{ XTASK_PYDOWN,
			{ ui.pyDown->value()
			} });
	}
    else if (ui.pyUp->value() > 0)
    {
		isPy = true;
		XFilter::instance().add(xTask{ XTASK_PYUP,
			{ ui.pyUp->value()
			} });
    }
    if (!isPy)
    {
		if (ui.doubleSpinBoxX->value() > 0 && ui.doubleSpinBoxX->value() > 0)
		{
			XFilter::instance().add(xTask{ XTASK_GAIN,
				{ ui.doubleSpinBoxX->value(),ui.doubleSpinBoxX->value()
				} });
		}
    }

    if (ui.spinBoxBright->value() > 0 || ui.SpinBoxContrast->value() > 1)
    {

        XFilter::instance().add(xTask{ XTASK_GAIN,
            {(double)ui.spinBoxBright->value(),ui.SpinBoxContrast->value()
            } });
    }

	if (ui.comboBoxColor->currentIndex() == 1)
	{
		XFilter::instance().add(xTask{ XTASK_GRAY });
        isColor = false;
	}

    if (ui.comboBoxRotate->currentIndex() == 1)
    {
        XFilter::instance().add(xTask{ XTASK_ROTATE90 });
    }
    else if(ui.comboBoxRotate->currentIndex() == 2)
    {
        XFilter::instance().add(xTask{ XTASK_ROTATE180 });
    }
    else if (ui.comboBoxRotate->currentIndex() == 3)
    {
        XFilter::instance().add(xTask{ XTASK_ROTATE270 });
	}
    
    if (ui.comboBoxFlip->currentIndex() == 1)
	{
		XFilter::instance().add(xTask{ XTASK_FLIPX });
	}
	else if (ui.comboBoxFlip->currentIndex() == 2)
	{
		XFilter::instance().add(xTask{ XTASK_FLIPY });
	}
	else if (ui.comboBoxFlip->currentIndex() == 3)
	{
		XFilter::instance().add(xTask{ XTASK_FLIPXY });
	}

    if (isMark)
    {
        double x = ui.MarkX->value();
        double y = ui.MarkY->value();
        double a = ui.MarkA->value();


        XFilter::instance().add(xTask{ XTASK_MARK,
            {x,y,a} });
    }

	if (isBlend)
	{
		double a = ui.BlendA->value();
		XFilter::instance().add(xTask{ XTASK_BLEND,
			{a} });
	}
	if (isMerge)
	{
        double h = XVideoThread::instance().height;
        double w = XVideoThread::instance().width;
        double w2 = XVideoThread::instance().width2;
		XFilter::instance().add(xTask{ XTASK_MERGE });
        ui.doubleSpinBoxX->setValue(w + w2);
        ui.doubleSpinBoxY->setValue(h);
	}
}

void XVideoUi::Export()
{
    if (isExport)
    {
        XVideoThread::instance().stopSave();
        isExport = false;
        ui.pushButtonExport->setText("Export");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(this, "save", "out1.avi");
    if (fileName.isEmpty()) return;
    std::string name = fileName.toStdString();
    int w = ui.doubleSpinBoxX->value();
    int h = ui.doubleSpinBoxY->value();
    if (XVideoThread::instance().startSave(name,w,h, isColor))
    {
        ui.pushButtonExport->setText("Stop");
        isExport = true;
    }
}

void XVideoUi::ExportEnd()
{
    isExport = false;
    ui.pushButtonExport->setText("Export");
}

void XVideoUi::Play()
{
    XVideoThread::instance().Play();
}

void XVideoUi::Pause()
{
    XVideoThread::instance().Pause();
}

void XVideoUi::Mark()
{
	isMark = false;
	isBlend = false;
	isMerge = false;
    QString name = QFileDialog::getOpenFileName(this, "select file", "./");
    if (name.isEmpty())
    {
        return;
    }

    std::string fileName = name.toStdString();
    cv::Mat m = cv::imread(fileName);
    if (m.empty())
    {
        return;
    }
    XVideoThread::instance().setMark(m);
    isMark = true;
}   

void XVideoUi::Blend()
{
    isMark = false;
    isBlend = false;
    isMerge = false;
	QString name = QFileDialog::getOpenFileName(this, "select video", "./");
	if (name.isEmpty())
	{
		return;
	}
    std::string fileName = name.toStdString();
    isBlend = XVideoThread::instance().openFileTwo(fileName);
}
void XVideoUi::Merge()
{
	isMark = false;
	isBlend = false;
	isMerge = false;
	QString name = QFileDialog::getOpenFileName(this, "select video", "./");
	if (name.isEmpty())
	{
		return;
	}
	std::string fileName = name.toStdString();
    isMerge = XVideoThread::instance().openFileTwo(fileName);
}

void XVideoUi::Left(int a)
{
    XVideoThread::instance().setBegin((double)a/1000);
    SliderMove(a);
}

void XVideoUi::Right(int a)
{
    XVideoThread::instance().setEnd((double)a / 1000);
}
