#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QLCDNumber>
#include <QMouseEvent>  // 마우스 이벤트 처리
#include <QTimer>
#include <qdebug.h>

QT_BEGIN_NAMESPACE
namespace Ui { class chess; }
QT_END_NAMESPACE

class chess : public QMainWindow
{
    Q_OBJECT

public:
    chess(QWidget *parent = nullptr);
    ~chess();

protected:
    void mousePressEvent(QMouseEvent *event) override;   // 마우스 클릭 처리
    void mouseMoveEvent(QMouseEvent *event) override;    // 마우스 이동 처리
    void mouseReleaseEvent(QMouseEvent *event) override; // 마우스 버튼 해제 처리

private slots:
    void on_white_done_clicked();
    void on_black_done_clicked();
    void updateWhiteTimer();
    void updateBlackTimer();

    void on_black_giveup_clicked();

    void on_white_giveup_clicked();

private:
    Ui::chess *ui;
    QGraphicsScene *scene;

    QGraphicsPixmapItem *selectedPiece = nullptr;
    QPointF originalPos;
    QPointF pieceOffset;

    bool isDragging = false;
    bool isSelectedWhite; // 선택된 기물의 색상 저장


    // 추가된 상태 변수
    bool isWhiteTurn = true;          // 백의 턴인지 여부
    bool pieceMovedInTurn = false;     // 한 턴에 기물이 이미 움직였는지 여부

    QTimer whiteTimer, blackTimer;
    int whiteTimeRemaining = 300000;  // 5분
    int blackTimeRemaining = 300000;  // 5분

    void drawChessBoard();
    void placePieces();
    QGraphicsPixmapItem* addPiece(const QString& imagePath, int row, int col);
    void updateLCD(int timeMs, QLCDNumber *lcd);
    bool isValidMove(QGraphicsPixmapItem *piece);
};

#endif // CHESS_H
