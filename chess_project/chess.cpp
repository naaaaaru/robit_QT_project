#include "chess.h"
#include "./ui_chess.h"
#include <QBrush>
#include <QPen>

chess::chess(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::chess)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    qDebug() << "체스 게임이 시작되었습니다. 흰색부터 시작하세요.";

    drawChessBoard();
    placePieces();

    updateLCD(whiteTimeRemaining, ui->white_timer);
    updateLCD(blackTimeRemaining, ui->black_timer);

    connect(&whiteTimer, &QTimer::timeout, this, &chess::updateWhiteTimer);
    connect(&blackTimer, &QTimer::timeout, this, &chess::updateBlackTimer);
}

chess::~chess()
{
    delete ui;
}

// 체스판 그리기: 흑백 8x8 칸
void chess::drawChessBoard()
{
    int tileSize = 80;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            QRectF tile(col * tileSize, row * tileSize, tileSize, tileSize);
            QBrush brush((row + col) % 2 == 0 ? Qt::white : Qt::black);
            scene->addRect(tile, QPen(Qt::black), brush);
        }
    }
}

// 체스말 추가: 이미지 경로와 위치를 받아 추가
QGraphicsPixmapItem* chess::addPiece(const QString& imagePath, int row, int col)
{
    QPixmap piece(imagePath);
    piece = piece.scaled(80, 80);  // 말 크기 조정
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(piece);
    item->setPos(col * 80, row * 80);

    // 이미지 경로를 사용자 데이터로 저장 (색상 구분에 사용)
    item->setData(0, imagePath);
    scene->addItem(item);
    return item;
}

// 기물 배치: 초기 체스 기물 위치 설정
void chess::placePieces()
{
    // 백 기물 배치
    for (int i = 0; i < 8; ++i) addPiece(":/images/white_pawn.png", 6, i);
    addPiece(":/images/white_rook.png", 7, 0);
    addPiece(":/images/white_knight.png", 7, 1);
    addPiece(":/images/white_bishop.png", 7, 2);
    addPiece(":/images/white_queen.png", 7, 3);
    addPiece(":/images/white_king.png", 7, 4);
    addPiece(":/images/white_bishop.png", 7, 5);
    addPiece(":/images/white_knight.png", 7, 6);
    addPiece(":/images/white_rook.png", 7, 7);

    // 흑 기물 배치
    for (int i = 0; i < 8; ++i) addPiece(":/images/black_pawn.png", 1, i);
    addPiece(":/images/black_rook.png", 0, 0);
    addPiece(":/images/black_knight.png", 0, 1);
    addPiece(":/images/black_bishop.png", 0, 2);
    addPiece(":/images/black_queen.png", 0, 3);
    addPiece(":/images/black_king.png", 0, 4);
    addPiece(":/images/black_bishop.png", 0, 5);
    addPiece(":/images/black_knight.png", 0, 6);
    addPiece(":/images/black_rook.png", 0, 7);
}

// 같은 색 기물인지 확인하는 함수
bool chess::isSameColor(QGraphicsPixmapItem *piece1, QGraphicsPixmapItem *piece2)
{
    QString path1 = piece1->data(0).toString();
    QString path2 = piece2->data(0).toString();
    return path1.contains("white") == path2.contains("white");
}

// 마우스 클릭: 기물 선택
void chess::mousePressEvent(QMouseEvent *event)
{
    QPointF clickPos = ui->graphicsView->mapToScene(event->pos());
    QGraphicsItem *item = scene->itemAt(clickPos, QTransform());

    if (item && event->button() == Qt::LeftButton)
    {
        QGraphicsPixmapItem *piece = dynamic_cast<QGraphicsPixmapItem *>(item);
        if (piece && isSameColor(piece, piece))  // 자신의 턴에 맞는 색만 선택 가능
        {
            selectedPiece = piece;
            originalPos = piece->pos();
            qDebug() << "기물이 선택되었습니다.";
        }
    }
}

// 마우스 릴리스: 기물 이동 및 잡기 처리
void chess::mouseReleaseEvent(QMouseEvent *event)
{
    if (!selectedPiece || pieceMovedInTurn) {
        qDebug() << "이동할 수 없습니다. 이미 기물이 움직였거나 선택된 기물이 없습니다.";
        return;
    }

    QPointF releasePos = ui->graphicsView->mapToScene(event->pos());
    int tileSize = 80;
    int col = static_cast<int>(releasePos.x()) / tileSize;
    int row = static_cast<int>(releasePos.y()) / tileSize;
    qreal centerX = col * tileSize + tileSize / 2;
    qreal centerY = row * tileSize + tileSize / 2;

    QGraphicsItem *existingItem = scene->itemAt(QPointF(centerX, centerY), QTransform());
    QGraphicsPixmapItem *capturedPiece = dynamic_cast<QGraphicsPixmapItem *>(existingItem);

    // 다른 색의 기물을 잡는 경우
    if (capturedPiece && !isSameColor(selectedPiece, capturedPiece))
    {
        scene->removeItem(capturedPiece);
        delete capturedPiece;
        qDebug() << "기물을 잡았습니다!";
    }

    // 기물 이동 처리
    selectedPiece->setPos(centerX - selectedPiece->boundingRect().width() / 2,
                          centerY - selectedPiece->boundingRect().height() / 2);
    selectedPiece = nullptr;
    qDebug() << "기물이" << col << "열," << row << "행으로 이동했습니다.";
    pieceMovedInTurn = true;  // 이 턴에서 기물이 이미 움직임
}


// LCD 업데이트 함수
void chess::updateLCD(int timeMs, QLCDNumber *lcd)
{
    int minutes = (timeMs / 60000) % 60;
    int seconds = (timeMs / 1000) % 60;
    lcd->display(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
}


// 흰색 시간 감소
void chess::updateWhiteTimer()
{
    whiteTimeRemaining -= 10;  // 10ms 감소
    updateLCD(whiteTimeRemaining, ui->white_timer);

    if (whiteTimeRemaining <= 0)
    {
        whiteTimer.stop();
    }
}

// 검은색 시간 감소
void chess::updateBlackTimer()
{
    blackTimeRemaining -= 10;  // 10ms 감소
    updateLCD(blackTimeRemaining, ui->black_timer);

    if (blackTimeRemaining <= 0)
    {
        blackTimer.stop();
    }
}

// 흰색 턴 종료: 검은색 타이머 시작
void chess::on_white_done_clicked()
{
    if (!pieceMovedInTurn) return;  // 기물이 움직이지 않았다면 턴 종료 불가

    whiteTimer.stop();
    blackTimer.start(10);  // 흑의 타이머 시작
    if (isWhiteTurn)  // 흰색 턴 종료
    {
        isWhiteTurn = false;
        pieceMovedInTurn = false;  // 턴 초기화
        qDebug() << "흰색 턴이 끝났습니다. 검은색 차례입니다.";
    }
}

void chess::on_black_done_clicked()
{
    if (!pieceMovedInTurn) return;  // 기물이 움직이지 않았다면 턴 종료 불가

    blackTimer.stop();
    whiteTimer.start(10);  // 백의 타이머 시작
    if (!isWhiteTurn)  // 검은색 턴 종료
    {
        isWhiteTurn = true;
        pieceMovedInTurn = false;  // 턴 초기화
        qDebug() << "검은색 턴이 끝났습니다. 흰색 차례입니다.";
    }
}


void chess::on_black_giveup_clicked()
{

}


void chess::on_white_giveup_clicked()
{

}
