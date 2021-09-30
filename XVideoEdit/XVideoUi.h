#pragma once

#include <QtWidgets/QWidget>
#include "ui_XVideoUi.h"
#include "XVideoThread.h"
#include "XFilter.h"
class XVideoUi : public QWidget
{
    Q_OBJECT

public:
    XVideoUi(QWidget *parent = Q_NULLPTR);
protected:
    void timerEvent(QTimerEvent *e) override;
private slots:
    void on_pushButtonClose_clicked();
    void on_pushButtonOpenFile_clicked();
    void SliderPress();
    void SliderRelease();
    void SliderMove(int);
    void Set();
    void Export();
    void ExportEnd();
    void Play();
    void Pause();
    void Mark();
    void Blend();
    void Merge();
    void Left(int a);
    void Right(int a);
private:
    void initUi();
private:
    Ui::XVideoUiClass ui;
};
