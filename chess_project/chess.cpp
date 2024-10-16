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

// 마우스 클릭 이벤트 처리: 말을 선택합니다.
void chess::mousePressEvent(QMouseEvent *event)
{
    if (pieceMovedInTurn) return;  // 이미 기물을 움직였다면 더 이상 이동 불가

    QPointF clickPos = ui->graphicsView->mapToScene(event->pos());
    QGraphicsItem *item = scene->itemAt(clickPos, QTransform());

    if (item && event->button() == Qt::LeftButton) {
        QGraphicsPixmapItem *piece = dynamic_cast<QGraphicsPixmapItem *>(item);
        if (piece && isValidMove(piece)) {  // 현재 턴에 맞는 기물인지 확인
            selectedPiece = piece;
            originalPos = selectedPiece->pos();  // 원래 위치 저장
            pieceOffset = clickPos - selectedPiece->pos();  // 클릭 지점과 말의 좌표 차이

            // 선택된 기물의 색상 저장
            QString piecePath = selectedPiece->data(0).toString();
            isSelectedWhite = piecePath.contains("white");

            isDragging = true;
        }
    }
}


// 마우스 이동 이벤트 처리: 선택된 말을 드래그합니다.
void chess::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging && selectedPiece) {
        QPointF movePos = ui->graphicsView->mapToScene(event->pos());
        selectedPiece->setPos(movePos - pieceOffset);  // 중심 보정
    }
}

// 마우스 해제 이벤트 처리: 드래그를 멈추고 말 위치를 확정합니다.
void chess::mouseReleaseEvent(QMouseEvent *event)
{
    if (isDragging && selectedPiece) {
        QPointF releasePos = ui->graphicsView->mapToScene(event->pos());

        // 체스판 범위 확인
        if (releasePos.x() < 0 || releasePos.x() >= 640 ||
            releasePos.y() < 0 || releasePos.y() >= 640) {
            selectedPiece->setPos(originalPos);  // 범위 벗어나면 원래 위치로 복귀
        } else {
            int tileSize = 80;

            // 놓은 칸 계산
            int col = static_cast<int>(releasePos.x()) / tileSize;
            int row = static_cast<int>(releasePos.y()) / tileSize;

            // 해당 칸의 중앙 좌표 계산
            qreal centerX = col * tileSize + tileSize / 2;
            qreal centerY = row * tileSize + tileSize / 2;

            // 중앙에 있는 기물 탐색
            QGraphicsItem* existingItem = scene->itemAt(QPointF(centerX, centerY), QTransform());
            QGraphicsPixmapItem* capturedPiece = dynamic_cast<QGraphicsPixmapItem*>(existingItem);

            // 상대방 기물만 제거
            if (capturedPiece && capturedPiece != selectedPiece) {
                QString capturedPath = capturedPiece->data(0).toString();
                bool isCapturedWhite = capturedPath.contains("white");

                // 현재 턴에 맞는 상대방 기물일 때만 제거
                if ((isWhiteTurn && !isCapturedWhite) || (!isWhiteTurn && isCapturedWhite)) {
                    qDebug() << "Captured piece at:" << capturedPiece->pos();
                    scene->removeItem(capturedPiece);  // 기물 삭제
                    delete capturedPiece;  // 메모리 해제
                }
            }

            // 선택된 기물을 새로운 칸의 중앙에 배치
            selectedPiece->setPos(centerX - selectedPiece->boundingRect().width() / 2,
                                  centerY - selectedPiece->boundingRect().height() / 2);

            pieceMovedInTurn = true;  // 기물이 움직였음을 표시
        }

        isDragging = false;
        selectedPiece = nullptr;
    }
}




bool chess::isValidMove(QGraphicsPixmapItem* piece)
{
    // QGraphicsPixmapItem에서 이미지 경로 가져오기
    QString piecePath = piece->data(0).toString();
    if (piecePath.isEmpty()) return false;

    // 이미지 경로에서 기물의 색상 확인
    bool isWhitePiece = piecePath.contains("white");

    // 현재 턴에 맞는 기물인지 확인
    return (isWhiteTurn && isWhitePiece) || (!isWhiteTurn && !isWhitePiece);
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
    qDebug() << "Trying to load image from path:" << imagePath;
    QPixmap piece(imagePath);
    if (piece.isNull()) {
        qDebug() << "Failed to load image:" << imagePath;
        return nullptr;
    }

    piece = piece.scaled(80, 80);  // 말의 크기를 80x80으로 맞춤
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(piece);
    item->setPos(col * 80, row * 80);

    // 이미지 경로를 QGraphicsPixmapItem의 사용자 데이터로 저장
    item->setData(0, imagePath);  // 키 0번에 이미지 경로 저장

    scene->addItem(item);
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
    if (!pieceMovedInTurn) return;  // 기물이 움직이지 않았다면 턴 종료 불가

    whiteTimer.stop();
    blackTimer.start(10);  // 흑의 타이머 시작
    isWhiteTurn = false;   // 흑의 턴으로 변경
    pieceMovedInTurn = false;  // 다음 턴을 위해 초기화
}

void chess::on_black_done_clicked()
{
    if (!pieceMovedInTurn) return;  // 기물이 움직이지 않았다면 턴 종료 불가

    blackTimer.stop();
    whiteTimer.start(10);  // 백의 타이머 시작
    isWhiteTurn = true;    // 백의 턴으로 변경
    pieceMovedInTurn = false;  // 다음 턴을 위해 초기화
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

void chess::on_black_giveup_clicked()
{

}


void chess::on_white_giveup_clicked()
{

}

