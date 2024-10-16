#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLCDNumber>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class chess;
}

class chess : public QMainWindow
{
    Q_OBJECT

public:
    explicit chess(QWidget *parent = nullptr);
    ~chess();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::chess *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *selectedPiece = nullptr;
    QPointF originalPos;
    QPointF pieceOffset;
    bool isDragging = false;
    bool isWhiteTurn = true;
    bool pieceMovedInTurn = false;
    bool isSelectedWhite = false;

    int whiteTimeRemaining = 60000; // 1분
    int blackTimeRemaining = 60000; // 1분
    QTimer whiteTimer;
    QTimer blackTimer;

    void drawChessBoard();
    QGraphicsPixmapItem* addPiece(const QString& imagePath, int row, int col);
    void updateLCD(int timeMs, QLCDNumber *lcd);
    void placePieces();
    bool isValidMove(QGraphicsPixmapItem* piece);

private slots:
    void updateWhiteTimer();
    void updateBlackTimer();
    void on_white_done_clicked();
    void on_black_done_clicked();
    void on_black_giveup_clicked();
    void on_white_giveup_clicked();
};

#endif // CHESS_H
