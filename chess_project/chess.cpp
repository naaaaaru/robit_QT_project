#include "chess.h"
#include "./ui_chess.h"
#include <QBrush>
#include <QPen>

chess::chess(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::chess)
{
    ui->setupUi(this);

    // QGraphicsScene 초기화 및 설정
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    drawChessBoard();   // 체스판 그리기
    placePieces();      // 체스말 배치

    // 초기 시간 표시 (5:00.00)
    updateLCD(whiteTimeRemaining, ui->white_timer);
    updateLCD(blackTimeRemaining, ui->black_timer);

    // 타이머 설정 (10ms마다 업데이트)
    connect(&whiteTimer, &QTimer::timeout, this, &chess::updateWhiteTimer);
    connect(&blackTimer, &QTimer::timeout, this, &chess::updateBlackTimer);
}

chess::~chess()
{
    delete ui;
}

// 체스판 그리기 (8x8 칸)
void chess::drawChessBoard()
{
    int tileSize = 80;  // 각 칸의 크기를 80x80으로 설정
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QRectF tile(col * tileSize, row * tileSize, tileSize, tileSize);
            QBrush brush((row + col) % 2 == 0 ? Qt::white : Qt::black);  // 번갈아 색 설정
            scene->addRect(tile, QPen(Qt::black), brush);
        }
    }
}

// 체스말을 주어진 위치에 배치하는 함수
QGraphicsPixmapItem* chess::addPiece(const QString& imagePath, int row, int col)
{
    qDebug() << "Trying to load image from path:" << imagePath; // 추가 디버깅
    QPixmap piece(imagePath);
    if (piece.isNull()) {
        qDebug() << "Failed to load image:" << imagePath;  // 여기에서 경로를 출력
        return nullptr; // 혹은 적절한 오류 처리
    }
    piece = piece.scaled(80, 80);  // 말의 크기를 80x80으로 맞춤
    QGraphicsPixmapItem* item = scene->addPixmap(piece);
    item->setPos(col * 80, row * 80);  // 체스판 위치에 맞게 배치
    return item;
}

// LCD 업데이트: 남은 시간을 분:초.밀리초 형식으로 표시
void chess::updateLCD(int timeMs, QLCDNumber *lcd)
{
    int minutes = (timeMs / 60000) % 60;  // 분 계산
    int seconds = (timeMs / 1000) % 60;   // 초 계산
    int milliseconds = (timeMs % 1000) / 10;  // 밀리초 (두 자리)

    QString timeText = QString("%1:%2.%3")
                           .arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'))
                           .arg(milliseconds, 2, 10, QChar('0'));

    lcd->display(timeText);
}

// 흰색 시간 감소
void chess::updateWhiteTimer()
{
    whiteTimeRemaining -= 10;  // 10ms 감소
    updateLCD(whiteTimeRemaining, ui->white_timer);

    if (whiteTimeRemaining <= 0) {
        whiteTimer.stop();
    }
}

// 검은색 시간 감소
void chess::updateBlackTimer()
{
    blackTimeRemaining -= 10;  // 10ms 감소
    updateLCD(blackTimeRemaining, ui->black_timer);

    if (blackTimeRemaining <= 0) {
        blackTimer.stop();
    }
}

// 흰색 턴 종료: 검은색 타이머 시작
void chess::on_white_done_clicked()
{
    whiteTimer.stop();
    blackTimer.start(10);  // 10ms마다 업데이트
}

// 검은색 턴 종료: 흰색 타이머 시작
void chess::on_black_done_clicked()
{
    blackTimer.stop();
    whiteTimer.start(10);  // 10ms마다 업데이트
}

// 체스판에 말 배치
void chess::placePieces()
{
    // 흰색 말 배치
    addPiece(":/images/white_pawn.png", 6, 0);
    addPiece(":/images/white_pawn.png", 6, 1);
    addPiece(":/images/white_pawn.png", 6, 2);
    addPiece(":/images/white_pawn.png", 6, 3);
    addPiece(":/images/white_pawn.png", 6, 4);
    addPiece(":/images/white_pawn.png", 6, 5);
    addPiece(":/images/white_pawn.png", 6, 6);
    addPiece(":/images/white_pawn.png", 6, 7);

    addPiece(":/images/white_rook.png", 7, 0);
    addPiece(":/images/white_knight.png", 7, 1);
    addPiece(":/images/white_bishop.png", 7, 2);
    addPiece(":/images/white_queen.png", 7, 3);
    addPiece(":/images/white_king.png", 7, 4);
    addPiece(":/images/white_bishop.png", 7, 5);
    addPiece(":/images/white_knight.png", 7, 6);
    addPiece(":/images/white_rook.png", 7, 7);

    // 검은색 말 배치
    addPiece(":/images/black_pawn.png", 1, 0);
    addPiece(":/images/black_pawn.png", 1, 1);
    addPiece(":/images/black_pawn.png", 1, 2);
    addPiece(":/images/black_pawn.png", 1, 3);
    addPiece(":/images/black_pawn.png", 1, 4);
    addPiece(":/images/black_pawn.png", 1, 5);
    addPiece(":/images/black_pawn.png", 1, 6);
    addPiece(":/images/black_pawn.png", 1, 7);

    addPiece(":/images/black_rook.png", 0, 0);
    addPiece(":/images/black_knight.png", 0, 1);
    addPiece(":/images/black_bishop.png", 0, 2);
    addPiece(":/images/black_queen.png", 0, 3);
    addPiece(":/images/black_king.png", 0, 4);
    addPiece(":/images/black_bishop.png", 0, 5);
    addPiece(":/images/black_knight.png", 0, 6);
    addPiece(":/images/black_rook.png", 0, 7);
}
