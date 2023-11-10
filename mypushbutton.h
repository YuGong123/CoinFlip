#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushButton(QWidget *parent = nullptr);

    //自定义构造函数  正常显示的图片路径  按下后显示的图片路径
    MyPushButton(QString normalImg, QString pressImg = "");

    QString normalImgPath;
    QString pressImgPath ;


    //弹跳特效
    void zoom1(); //向下跳
    void zoom2(); //向上跳


    //重写按钮按下 和释放事件
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);





signals:

};

#endif // MYPUSHBUTTON_H
