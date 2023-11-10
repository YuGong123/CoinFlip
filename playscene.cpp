#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QSound>
#include <QPropertyAnimation>
#include "mypushbutton.h"
#include "dataconfig.h"
#include "mycoin.h"

//PlayScene::PlayScene(QWidget *parent) : QMainWindow(parent)
//{

//}


PlayScene::PlayScene(int levelNum)
{
    QString str = QString("你选择的是第 %1 关").arg(levelNum);
    qDebug() << str;

    this->LevelIndex = levelNum;





    //------------------初始化游戏场景------------------
    this->setFixedSize(320, 588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("翻硬币场景");

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 退出游戏
    connect(quitAction, &QAction::triggered, this, &PlayScene::close);





    //----------------------音效-------------------
    //返回按钮音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);
    //翻金币音效
    QSound *flipSound = new QSound(":/res/ConFlipSound.wav",this);
    //胜利按钮音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);





    //------------------显示胜利图片------------------
    QLabel* winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - tmpPix.width())*0.5 , -tmpPix.height());






    //------------------返回按钮------------------
    MyPushButton *backBtn = new MyPushButton(":/res/BackButton.png", ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move( this->width() - backBtn->width(), this->height() - backBtn->height() );

    connect(backBtn, &MyPushButton::clicked, [=](){
//        qDebug() << "翻硬币场景:点击了返回按钮，准备返回选择场景";
        backSound->play();

        //告诉选择场景，我返回了，选择场景监听“游戏场景”的返回按钮
        QTimer::singleShot(500, this, [this](){
            emit this->chooseSceneBack();
        });
    });





    //------------------显示第几关------------------
    QLabel *label = new QLabel(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    label->setFont(font);
    QString fontstr = QString("Level: %1").arg(this->LevelIndex);
    label->setText(fontstr);
    label->setGeometry(QRect(30, this->height() - 50,120, 50) );






    //------------------初始化每个关卡的二维数组------------------
    dataConfig  config;
    for(int i = 0 ; i < 4;i++)
    {
        for(int j = 0 ; j < 4; j++){
            this->gameArray[i][j] = config.mData[this->LevelIndex][i][j];
        }
    }






    //------------------创建每一个金币的背景图片------------------
       for(int i = 0 ; i < 4;i++)
       {
           for(int j = 0 ; j < 4; j++)
           {
              //绘制背景图片
               QLabel* label = new QLabel;
               label->setGeometry(0,0,50,50);
               label->setPixmap(QPixmap(":/res/BoardNode.png"));
               label->setParent(this);
               label->move(57 + i*50,200+j*50);


               //创建每一个金币和它的图片
               QString coinPath;
               if(this->gameArray[i][j] == 1){
                   coinPath = ":/res/Coin0001.png";
               }
               else{
                   coinPath = ":/res/Coin0008.png";
               }

               MyCoin *coin = new MyCoin(coinPath);
               coin->setParent(this);
               coin->move(59 + i*50,204+j*50);


               //给每一个金币的添加属性
               coin->posX = i;
               coin->posY = j;
               coin->flag = gameArray[i][j];



               //将金币放入到 金币的二维数组里 以便后期的维护
               coinBtnArray[i][j] = coin;




               //点击金币 进行翻转
               connect(coin, &MyCoin::clicked, [=](){
                   //播放音效
                   flipSound->play();


                   //有的人手快，预防BUG，点击按钮的瞬间，屏蔽所有的按钮
                   for(int i = 0 ; i < 4;i++)
                   {
                       for(int j = 0 ; j < 4; j++)
                       {
                           coinBtnArray[i][j]->isWin = true;
                       }
                   }


                   coin->changeFlag();

                   //该位置的金币被点过了，二维数组该更新了
                   this->gameArray[i][j] = this->gameArray[i][j] == 0 ? 1 : 0;


                   //---------------翻转该硬币周围的硬币---------------
                   QTimer::singleShot(300, this,[=](){
                       //周围的右侧硬币翻转的条件
                       if(coin->posX+1 <=3)
                       {
                           coinBtnArray[coin->posX+1][coin->posY]->changeFlag();
                           gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY]== 0 ? 1 : 0;
                       }
                       //周围的左侧硬币翻转的条件
                       if(coin->posX-1>=0)
                       {
                           coinBtnArray[coin->posX-1][coin->posY]->changeFlag();
                           gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY]== 0 ? 1 : 0;
                       }
                       //周围的下侧硬币翻转的条件
                       if(coin->posY+1<=3)
                       {
                           coinBtnArray[coin->posX][coin->posY+1]->changeFlag();
                           gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX][coin->posY+1]== 0 ? 1 : 0;
                       }
                       //周围的上侧硬币翻转的条件
                       if(coin->posY-1>=0)
                       {
                           coinBtnArray[coin->posX][coin->posY-1]->changeFlag();
                           gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX][coin->posY-1]== 0 ? 1 : 0;
                       }



                       //有的人手快，预防BUG，周围按钮翻转完毕后，解除屏蔽所有的按钮
                       for(int i = 0 ; i < 4;i++)
                       {
                           for(int j = 0 ; j < 4; j++)
                           {
                               coinBtnArray[i][j]->isWin = false;
                           }
                       }




                       //---------------判断游戏是否胜利---------------
                       this->isWin = true;
                       for(int i = 0 ; i < 4;i++)
                       {
                           for(int j = 0 ; j < 4; j++)
                           {
                               //qDebug() << coinBtn[i][j]->flag ;
                               if( coinBtnArray[i][j]->flag == false)
                               {
                                   this->isWin = false;
                                   break;
                               }
                           }
                       }


                       if(this->isWin == true)
                       {
                           //qDebug() << "胜利";
                           winSound->play();

                           //胜利，就将所有按钮的胜利标志改为true,在mousePressEvent事件中屏蔽所有的按钮事件。
                           for(int i = 0 ; i < 4;i++)
                           {
                               for(int j = 0 ; j < 4; j++)
                               {
                                   coinBtnArray[i][j]->isWin = true;
                               }
                           }


                           //------------------将胜利的图片移动下来--------------------
                           QPropertyAnimation * winAnimation = new QPropertyAnimation(winLabel, "geometry");

                           winAnimation->setDuration(1000);
                           winAnimation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                           winAnimation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                           winAnimation->setEasingCurve(QEasingCurve::OutBounce);
                           winAnimation->start();
                       }



                   });

               });
           }
       }


}



void PlayScene::paintEvent(QPaintEvent *){

    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0, this->width(), this->height(), pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width() * 0.5 , pix.height() * 0.5);
    painter.drawPixmap(10,30,  pix);

}

