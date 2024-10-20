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
void chess::mouseReleaseEvent(QMouseEvent *event) {
    if (!selectedPiece || pieceMovedInTurn) {
        qDebug() << "Move rejected - No piece selected or piece already moved this turn";
        return;
    }

    QPointF releasePos = ui->graphicsView->mapToScene(event->pos());
    int tileSize = 80;
    int col = static_cast<int>(releasePos.x()) / tileSize;
    int row = static_cast<int>(releasePos.y()) / tileSize;

    // 체스판 범위 체크
    if (row < 0 || row > 7 || col < 0 || col > 7) {
        qDebug() << "Move rejected - Outside chess board";
        selectedPiece->setPos(originalPos);
        selectedPiece = nullptr;
        return;
    }

    QString pieceType = getPieceType(selectedPiece);
    int startRow = static_cast<int>(originalPos.y()) / tileSize;
    int startCol = static_cast<int>(originalPos.x()) / tileSize;

    if (!isValidMove(pieceType, startRow, startCol, row, col)) {
        qDebug() << "Move rejected - Invalid move according to chess rules";
        selectedPiece->setPos(originalPos);
        selectedPiece = nullptr;
        return;
    }

    // 타겟 위치의 기물 확인 (QGraphicsPixmapItem만 고려)
    QGraphicsPixmapItem* capturedPiece = nullptr;
    QList<QGraphicsItem*> items = scene->items(QPointF(col * 80 + 40, row * 80 + 40));
    for (QGraphicsItem* item : items) {
        if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
            if (pixmapItem != selectedPiece) {
                capturedPiece = pixmapItem;
                break;
            }
        }
    }

    // 기물 잡기
    if (capturedPiece) {
        scene->removeItem(capturedPiece);
        delete capturedPiece;
        qDebug() << "Piece captured!";
    }

    // 기물 이동
    selectedPiece->setPos(col * 80, row * 80);
    selectedPiece = nullptr;
    pieceMovedInTurn = true;
    qDebug() << "Piece moved successfully";
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

QString chess::getPieceType(QGraphicsPixmapItem* piece)
{
    QString path = piece->data(0).toString();  // 이미지 경로 가져오기
    if (path.contains("pawn")) return "pawn";
    if (path.contains("rook")) return "rook";
    if (path.contains("knight")) return "knight";
    if (path.contains("bishop")) return "bishop";
    if (path.contains("queen")) return "queen";
    if (path.contains("king")) return "king";
    return "";
}

bool chess::isValidMove(const QString& pieceType, int startRow, int startCol, int endRow, int endCol) {
    qDebug() << "Moving piece:" << pieceType;
    qDebug() << "From:" << startRow << startCol;
    qDebug() << "To:" << endRow << endCol;

    int rowDiff = endRow - startRow;
    int colDiff = endCol - startCol;

    // 체스판 범위 체크
    if (endRow < 0 || endRow > 7 || endCol < 0 || endCol > 7) {
        return false;
    }

    // 타겟 위치의 기물 확인 (QGraphicsPixmapItem만 고려)
    QGraphicsPixmapItem* targetPiece = nullptr;
    QList<QGraphicsItem*> items = scene->items(QPointF(endCol * 80 + 40, endRow * 80 + 40));
    for (QGraphicsItem* item : items) {
        if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item)) {
            targetPiece = pixmapItem;
            break;
        }
    }

    bool isWhite = selectedPiece->data(0).toString().contains("white");
    qDebug() << "Is white piece:" << isWhite;
    qDebug() << "Row difference:" << rowDiff;
    qDebug() << "Column difference:" << colDiff;
    qDebug() << "Target piece exists:" << (targetPiece != nullptr);

    if (pieceType == "pawn") {
        // 전진 이동 (수직 이동)
        if (colDiff == 0) {
            if (targetPiece) {
                qDebug() << "Cannot move forward - piece in the way";
                return false; // 전진 경로에 기물이 있으면 이동 불가
            }

            if (isWhite) {
                // 백색 폰
                if (rowDiff == -1) return true; // 한 칸 전진
                if (startRow == 6 && rowDiff == -2) { // 첫 이동시 두 칸 전진
                    // 중간 경로 체크
                    QList<QGraphicsItem*> midItems = scene->items(QPointF(startCol * 80 + 40, (startRow - 1) * 80 + 40));
                    bool midPathClear = true;
                    for (QGraphicsItem* item : midItems) {
                        if (dynamic_cast<QGraphicsPixmapItem*>(item)) {
                            midPathClear = false;
                            break;
                        }
                    }
                    return midPathClear;
                }
            } else {
                // 흑색 폰
                if (rowDiff == 1) return true; // 한 칸 전진
                if (startRow == 1 && rowDiff == 2) { // 첫 이동시 두 칸 전진
                    // 중간 경로 체크
                    QList<QGraphicsItem*> midItems = scene->items(QPointF(startCol * 80 + 40, (startRow + 1) * 80 + 40));
                    bool midPathClear = true;
                    for (QGraphicsItem* item : midItems) {
                        if (dynamic_cast<QGraphicsPixmapItem*>(item)) {
                            midPathClear = false;
                            break;
                        }
                    }
                    return midPathClear;
                }
            }
        }

        // 대각선 공격
        if (abs(colDiff) == 1) {
            if (isWhite && rowDiff == -1) {
                return targetPiece && !isSameColor(selectedPiece, targetPiece);
            }
            if (!isWhite && rowDiff == 1) {
                return targetPiece && !isSameColor(selectedPiece, targetPiece);
            }
        }

        return false;
    }


    // 룩: 같은 행 또는 열로 이동하며 경로에 장애물이 없어야 함
    if (pieceType == "rook") {
        return (startRow == endRow || startCol == endCol) && isPathClear(startRow, startCol, endRow, endCol);
    }

    // 비숍: 대각선 이동하며 경로에 장애물이 없어야 함
    if (pieceType == "bishop") {
        return abs(endRow - startRow) == abs(endCol - startCol) && isPathClear(startRow, startCol, endRow, endCol);
    }

    // 퀸: 행, 열, 대각선으로 이동 가능하며 경로에 장애물이 없어야 함
    if (pieceType == "queen") {
        return (startRow == endRow || startCol == endCol || abs(endRow - startRow) == abs(endCol - startCol)) &&
               isPathClear(startRow, startCol, endRow, endCol);
    }

    // 나이트: 경로와 관계없이 L자 형태로 이동
    if (pieceType == "knight") {
        return (abs(endRow - startRow) == 2 && abs(endCol - startCol) == 1) ||
               (abs(endRow - startRow) == 1 && abs(endCol - startCol) == 2);
    }

    // 킹: 한 칸씩 이동
    if (pieceType == "king") {
        return abs(endRow - startRow) <= 1 && abs(endCol - startCol) <= 1;
    }

    return false;  // 유효하지 않은 이동
}



bool chess::isPathClear(int startRow, int startCol, int endRow, int endCol) {
    int rowStep = (endRow - startRow) == 0 ? 0 : (endRow - startRow) / abs(endRow - startRow);
    int colStep = (endCol - startCol) == 0 ? 0 : (endCol - startCol) / abs(endCol - startCol);

    int currentRow = startRow + rowStep;
    int currentCol = startCol + colStep;

    // 목표 지점 전까지 경로를 검사
    while (currentRow != endRow || currentCol != endCol) {
        // 해당 좌표에 있는 아이템들 확인
        QList<QGraphicsItem*> items = scene->items(QPointF(currentCol * 80 + 40, currentRow * 80 + 40));

        // 기물(Pixmap Item)만 체크
        for (QGraphicsItem* item : items) {
            if (dynamic_cast<QGraphicsPixmapItem*>(item)) {
                return false;  // 경로에 기물이 있으면 false 반환
            }
        }

        // 다음 위치로 이동
        currentRow += rowStep;
        currentCol += colStep;
    }
    return true;  // 경로에 기물이 없음
}





void chess::on_black_giveup_clicked()
{

}


void chess::on_white_giveup_clicked()
{

}
