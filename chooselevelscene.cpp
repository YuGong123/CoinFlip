#include "chooselevelscene.h"
#include <QMenuBar>
#include <QString>
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <QSound>
#include "mypushbutton.h"


ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
    //------------------实例化选择关卡场景------------------
    //配置选择关卡场景
    this->setFixedSize(320, 588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 退出游戏
    connect(quitAction, &QAction::triggered, this, &ChooseLevelScene::close);





    //------------------选择关卡按钮的音效------------------
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);





    //------------------返回按钮------------------
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move( this->width() - backBtn->width(), this->height() - backBtn->height() );

    connect(backBtn, &MyPushButton::clicked, [=](){
//        qDebug() << "点击了返回按钮";
        backSound->play();



        //告诉主场景，我返回了，主场景监听chooseLevelScene的返回按钮
        QTimer::singleShot(500, this, [this](){
            emit this->chooseSceneBack();
        });
    });





    //------------------创建关卡选择按钮------------------
    for(int i = 0; i < 20; i++)
    {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + i%4 * 70, 130 + i/4 * 70);


        QLabel *label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(), menuBtn->height());
        label->setText(QString::number(i+1));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->move(25 + i%4 * 70, 130 + i/4 * 70);
        //设置让鼠标进行穿透 51号属性
        label->setAttribute(Qt::WA_TransparentForMouseEvents);





        //------------------监听关卡选择按钮的选择事件------------------
        connect(menuBtn, &MyPushButton::clicked, [=]()
        {
            //播放选择关卡的音效
            chooseSound->play();


            //-----进入到游戏场景-----
            this->hide();
            play = new PlayScene(i+1);
            //防止窗口乱跑
            play->setGeometry(this->geometry());
            play->show();





            //从游戏场景返回到选择场景
            connect(play, &PlayScene::chooseSceneBack, [=]()
            {
                //防止窗口乱跑
                this->setGeometry( play->geometry() );

                this->show();
                delete play;
                play = NULL;
            });
        });





    }


}



void ChooseLevelScene::paintEvent(QPaintEvent *){

    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0, this->width(), this->height(), pix);

    //加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap( (this->width() - pix.width())*0.5, 30, pix.width(), pix.height(), pix );

}











