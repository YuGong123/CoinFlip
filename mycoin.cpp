#include "mycoin.h"
#include <QDebug>

//MyCoin::MyCoin(QWidget *parent) : QPushButton(parent)
//{

//}


MyCoin::MyCoin(QString btnImg){
    QPixmap pix;
    bool ret = pix.load(btnImg);

    if(!ret){
        qDebug() << "MyCoin图片加载失败";
        return;
    }

    //设置图片固定大小 -- 个人觉得是根据图片大小设置按钮大小
    this->setFixedSize(pix.width(), pix.height());

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(), pix.height()));






    //---------------初始化定时器对象-------------------
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);



    //监听正面翻反面的信号，并且翻转金币，8张图，翻转8次。
    connect(timer1, &QTimer::timeout, [=](){
        QPixmap pix;
        QString coinPath = QString(":/res/Coin000%1.png").arg(this->picMin++);
        pix.load(coinPath);

        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));

        //金币翻成银币，已经翻完了，将picMin重置为1.
        if(this->picMin > this->picMax){
            this->picMin = 1;
            isAnimation = false;     //动画做完了
            timer1->stop();
        }
    });




    //监听反面翻正面的信号，并且翻转银币，8张图，翻转8次。
    connect(timer2, &QTimer::timeout, [=](){
        QPixmap pix;
        QString coinPath = QString(":/res/Coin000%8.png").arg(this->picMax--);
        pix.load(coinPath);

        this->setFixedSize(pix.width(), pix.height());
        this->setStyleSheet("QPushButton{border:0px}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(), pix.height()));

        //金币翻成银币，已经翻完了，将picMin重置为1.
        if(this->picMax < this->picMin){
            this->picMax = 8;
            isAnimation = false;     //动画做完了
            timer2->stop();
        }
    });

}



//改变正反面标志的方法
void MyCoin::changeFlag()
{
    //如果是正面  翻成反面
    if(this->flag){
        //开启正面翻反面的定时器
        timer1->start(30);      //每隔30毫秒都会发送"timeout"信号
        isAnimation = true;     //开始做动画了
        this->flag = false;
    }
    //反面翻正面
    else{
        timer2->start(30);
        isAnimation = true;     //开始做动画了
        this->flag = true;
    }
}



//重写鼠标按下的事件
//当金币在翻转状态时，此时父类就啥也不做，也就无法触发翻转
//如果金币不在翻转状态，就把消息传递下去。
void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation == true || this->isWin == true){
        return;
    }
    else{
        QPushButton::mousePressEvent(e);
    }

}
















