#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyCoin(QWidget *parent = nullptr);
    MyCoin(QString btnImg);

    //金币的属性
    int posX;   //金币的坐标
    int posY;
    bool flag;  //金币的正反


    //改变标志的方法
    void changeFlag();
    QTimer *timer1;  //正面翻反面的定时器
    QTimer *timer2;  //反面翻正面的定时器
    int picMin = 1;
    int picMax = 8;

    //执行动画标志
    bool isAnimation = false;


    //重写鼠标按下的事件
    void mousePressEvent(QMouseEvent *e);


    //是否胜利的标志,不能设置为随机值，不然可能导致这个硬币上来就被屏蔽。
    bool isWin = false;


signals:

};

#endif // MYCOIN_H
