#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include <QSound>
#include <QTimer>
#include "mypushbutton.h"

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);


    //------------------翻金币主场景------------------

    //设置固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻硬币");

    //退出游戏
    connect(ui->actionQuit, &QAction::triggered, this,&MainScene::close);





    //------------------实例化选择关卡场景------------------
    chooseScene = new ChooseLevelScene;




    //------------------主场景监听子场景的返回信号------------------
    connect(chooseScene, &ChooseLevelScene::chooseSceneBack, [this](){
        this->setGeometry( chooseScene->geometry() );   //防止窗口乱跑
        this->chooseScene->hide();
        this->show();
    });




    //------------------设置主场景里的开始按钮的点击音效------------------
    QSound *startSound = new QSound(":/res/TapButtonSound.wav", this);





    //------------------设置主场景里的开始按钮------------------
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width() * 0.5 - startBtn->width() * 0.5 , this->height() * 0.7);

    //弹跳特效
    connect(startBtn, &MyPushButton::clicked, [=]()
    {
        //播放开始音效
        startSound->play();
        startBtn->zoom1();
        startBtn->zoom2();



        //------------------进入到选择关卡场景中------------------
        //延时0.5秒后，this触发Lambda表达式的内容。
        QTimer::singleShot(500,this,[this]()
        {
            //设置chooseScene场景的位置
            chooseScene->setGeometry(this->geometry());     //防止窗口乱跑
            //隐藏主场景
            this->hide();
            //显示选择关卡
            chooseScene->show();
        });


    });



}


void MainScene::paintEvent(QPaintEvent * ){
    //创建画家painter， 在this画板上画
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0, this->width(), this->height(), pix);

    //給背景上图标
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5 , pix.height() * 0.5);
    painter.drawPixmap(10,30,  pix);
}





MainScene::~MainScene()
{
    delete ui;
}

