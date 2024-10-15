#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QLCDNumber>  // QLCDNumber 헤더 추가
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class chess; }
QT_END_NAMESPACE

class chess : public QMainWindow
{
    Q_OBJECT

public:
    chess(QWidget *parent = nullptr);
    ~chess();

private slots:
    void on_white_done_clicked();
    void on_black_done_clicked();
    void updateWhiteTimer();
    void updateBlackTimer();

private:
    Ui::chess *ui;
    QGraphicsScene *scene;
    QTimer whiteTimer;
    QTimer blackTimer;
    int whiteTimeRemaining = 300000;  // 300초 = 5분 (밀리초 단위)
    int blackTimeRemaining = 300000;

    void drawChessBoard();
    void updateLCD(int timeMs, QLCDNumber *lcd);
};

#endif // CHESS_H
