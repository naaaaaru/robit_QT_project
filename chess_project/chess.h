#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>  // 체스말 이미지 추가를 위한 헤더
#include <QLCDNumber>
#include <QDebug>
#include <QTimer>
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

    void drawChessBoard();               // 체스판 그리기
    void placePieces();  // 체스말 배치
    QTimer whiteTimer;
    QTimer blackTimer;
    QGraphicsPixmapItem* addPiece(const QString& imagePath, int row, int col); // 체스말 추가
    int whiteTimeRemaining = 300000;  // 300초 = 5분 (밀리초 단위)
    int blackTimeRemaining = 300000;
    void updateLCD(int timeMs, QLCDNumber *lcd);
};

#endif // CHESS_H
