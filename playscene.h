#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
//    explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int levelNum);

    int LevelIndex; //内部成员属性，记录所选的关卡

    //重写绘图事件，背景绘图函数，系统自动调用
    virtual void paintEvent(QPaintEvent *);

    //二维数组 维护每个关卡的具体数据  gameArray = vector< vector<int> >
    int gameArray[4][4];

    //维护硬币按钮的二维数组
    MyCoin *coinBtnArray[4][4];

    //是否胜利的标志
    bool isWin;


signals:
    void chooseSceneBack();

};

#endif // PLAYSCENE_H
