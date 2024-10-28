#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLCDNumber>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>

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
    bool debugMode = false;
    bool pieceMovedInTurn = false;

    int whiteTime= 600000;
    int blackTime = 600000;

    bool isValidMove(const QString& pieceType, int startRow, int startCol, int endRow, int endCol);
    bool isPathClear(int startRow, int startCol, int endRow, int endCol);
    QString getPieceType(QGraphicsPixmapItem* piece);

    void updateTurn();
    void resetGame();
    void checkTimeOver();
    void finishGame(const QString& winner);
    void drawChessBoard();
    void capturedShow(QGraphicsPixmapItem* piece, QGraphicsView* storageView);

    void promotePawn(QGraphicsPixmapItem* pawn, int row, int col);
    void changePiece(QGraphicsPixmapItem* oldPiece, const QString& newImagePath, int row, int col);
    QGraphicsPixmapItem* addPiece(const QString& imagePath, int row, int col);
    void placePieces();
    void updateLCD(int timeMs, QLCDNumber *lcd);
    bool isSameColor(QGraphicsPixmapItem *piece1, QGraphicsPixmapItem *piece2);

    QTimer whiteTimer;
    QTimer blackTimer;

private slots:
    void updateWhiteTimer();
    void updateBlackTimer();
    void on_white_done_clicked();
    void on_black_done_clicked();
    void on_white_giveup_clicked();
    void on_black_giveup_clicked();
    void on_help_button_clicked();
    void on_debug_button_clicked();
};

#endif // CHESS_H
