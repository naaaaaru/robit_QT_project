#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLCDNumber>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>

namespace Ui
{
class chess;
}

class chess : public QMainWindow
{
    Q_OBJECT

public:
    explicit chess(QWidget *parent = nullptr);
    ~chess();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::chess *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *selectedPiece = nullptr;
    QPointF originalPos;
    bool isWhiteTurn = true;
    bool pieceMovedInTurn = false;
    bool isValidMove(const QString& pieceType, int startRow, int startCol, int endRow, int endCol);
    bool isPathClear(int startRow, int startCol, int endRow, int endCol);  // 경로에 장애물이 있는지 검사
    QString getPieceType(QGraphicsPixmapItem* piece);

    QTimer whiteTimer;
    QTimer blackTimer;
    int whiteTimeRemaining = 300000;
    int blackTimeRemaining = 300000;


    void drawChessBoard();
    QGraphicsPixmapItem* addPiece(const QString& imagePath, int row, int col);
    void placePieces();
    void updateLCD(int timeMs, QLCDNumber *lcd);
    bool isSameColor(QGraphicsPixmapItem *piece1, QGraphicsPixmapItem *piece2);

private slots:
    void updateWhiteTimer();
    void updateBlackTimer();
    void on_white_done_clicked();
    void on_black_done_clicked();
    void on_white_giveup_clicked();
    void on_black_giveup_clicked();
};

#endif // CHESS_H
