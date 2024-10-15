#include "chess.h"
#include "./ui_chess.h"

chess::chess(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::chess)
{
    ui->setupUi(this);

    // QGraphicsScene 초기화 및 체스판 그리기
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    drawChessBoard();

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

// 체스판 그리기
void chess::drawChessBoard()
{
    int tileSize = 50;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QRectF tile(col * tileSize, row * tileSize, tileSize, tileSize);
            QBrush brush((row + col) % 2 == 0 ? Qt::white : Qt::black);
            scene->addRect(tile, QPen(Qt::black), brush);
        }
    }
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
