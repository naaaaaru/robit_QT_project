#include "chess.h"
#include "./ui_chess.h"
#include <QBrush>
#include <QPen>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>

chess::chess(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::chess), isWhiteTurn(true)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    QGraphicsScene *whiteGotScene = new QGraphicsScene(this);
    ui->white_got->setScene(whiteGotScene);

    QGraphicsScene *blackGotScene = new QGraphicsScene(this);
    ui->black_got->setScene(blackGotScene);

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

void chess::drawChessBoard()
{
    int tileSize = 80;
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            QRectF tile(col * tileSize, row * tileSize, tileSize, tileSize);
            QBrush brush;
            if ((row + col) % 2 == 0)
            {
                brush = Qt::white;
            }
            else
            {
                brush = Qt::black;
            }
            scene->addRect(tile, QPen(Qt::black), brush);
        }
    }
}

QGraphicsPixmapItem* chess::addPiece(const QString& imagePath, int row, int col)
{
    QPixmap piece(imagePath);
    piece = piece.scaled(80, 80);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(piece);
    item->setPos(col * 80, row * 80);
    item->setData(0, imagePath);
    scene->addItem(item);

    return item;
}

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

bool chess::isSameColor(QGraphicsPixmapItem *piece1, QGraphicsPixmapItem *piece2)
{
    QString path1 = piece1->data(0).toString();
    QString path2 = piece2->data(0).toString();
    return path1.contains("white") == path2.contains("white");
}

void chess::mousePressEvent(QMouseEvent *event)
{
    QPointF clickPos = ui->graphicsView->mapToScene(event->pos());
    QGraphicsItem *item = scene->itemAt(clickPos, QTransform());

    if (item && event->button() == Qt::LeftButton)
    {
        QGraphicsPixmapItem *piece = dynamic_cast<QGraphicsPixmapItem *>(item);
        if (piece)
        {
            if ((isWhiteTurn && piece->data(0).toString().contains("white")) ||
                (!isWhiteTurn && piece->data(0).toString().contains("black")))
            {
                selectedPiece = piece;
                originalPos = piece->pos();
                qDebug() << "기물이 선택되었습니다.";
            }
            else
            {
                qDebug() << "에러: 잘못된 색의 기물이 선택되었습니다.";
            }
        }
    }
}


void chess::mouseReleaseEvent(QMouseEvent *event)
{
    if (!selectedPiece)
    {
        qDebug() << "에러: 기물이 선택되지 않았습니다.";
        return;
    }

    QPointF releasePos = ui->graphicsView->mapToScene(event->pos());
    int tileSize = 80;
    int col = static_cast<int>(releasePos.x()) / tileSize;
    int row = static_cast<int>(releasePos.y()) / tileSize;

    if (row < 0 || row > 7 || col < 0 || col > 7)
    {
        qDebug() << "에러: 체스판 외부로 이동할 수 없습니다.";
        selectedPiece->setPos(originalPos);
        selectedPiece = nullptr;
        return;
    }

    if (!debugMode)
    {
        if (pieceMovedInTurn)
        {
            qDebug() << "에러: 이번 턴에서 이미 기물이 움직였습니다.";
            selectedPiece->setPos(originalPos);
            selectedPiece = nullptr;
            return;
        }

        if ((isWhiteTurn && !selectedPiece->data(0).toString().contains("white")) ||
            (!isWhiteTurn && selectedPiece->data(0).toString().contains("white")))
        {
            qDebug() << "에러: 현재 턴의 기물이 아닙니다.";
            selectedPiece->setPos(originalPos);
            selectedPiece = nullptr;
            return;
        }

        QString pieceType = getPieceType(selectedPiece);
        int startRow = static_cast<int>(originalPos.y()) / tileSize;
        int startCol = static_cast<int>(originalPos.x()) / tileSize;

        if (!isValidMove(pieceType, startRow, startCol, row, col))
        {
            qDebug() << "에러: 유효하지 않은 움직임입니다. 해당 기물 규칙을 따르지 않았습니다.";
            selectedPiece->setPos(originalPos);
            selectedPiece = nullptr;
            return;
        }
    }

    QGraphicsPixmapItem* capturedPiece = nullptr;
    QList<QGraphicsItem*> items = scene->items(QPointF(col * 80 + 40, row * 80 + 40));

    for (int i = 0; i < items.size(); ++i)
    {
        if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(items[i]))
        {
            if (pixmapItem != selectedPiece)
            {
                capturedPiece = pixmapItem;
                break;
            }
        }
    }

    if (capturedPiece)
    {
        if (!debugMode && isSameColor(selectedPiece, capturedPiece))
        {
            qDebug() << "에러: 같은 색의 기물은 잡을 수 없습니다.";
            selectedPiece->setPos(originalPos);
            selectedPiece = nullptr;
            return;
        }

        QString capturedPieceType = getPieceType(capturedPiece);
        QGraphicsView *storageView;

        if (isWhiteTurn)
        {
            storageView = ui->white_got;
        }
        else
        {
            storageView = ui->black_got;
        }
        addPieceToStorage(capturedPiece, storageView);

        if (capturedPieceType == "king")
        {
            if (isWhiteTurn)
            {
                declareWinner("흰색");
            }
            else
            {
                declareWinner("검은색");
            }

            return;  // 게임 종료
        }
        scene->removeItem(capturedPiece);
        delete capturedPiece;
        qDebug() << "알림: 기물을 잡았습니다!";
    }

    selectedPiece->setPos(col * 80, row * 80);

    QString pieceType = getPieceType(selectedPiece);
    if (!debugMode && pieceType == "pawn" && (row == 0 || row == 7))
    {
        promotePawn(selectedPiece, row, col);
    }

    if (!debugMode)
    {
        selectedPiece = nullptr;
        pieceMovedInTurn = true;
        isWhiteTurn = !isWhiteTurn;
        qDebug() << "알림: 기물이 성공적으로 이동했습니다.";
    }
    else
    {
        selectedPiece = nullptr;
    }
}

void chess::addPieceToStorage(QGraphicsPixmapItem* piece, QGraphicsView* storageView)
{
    QGraphicsScene *storageScene = storageView->scene();
    QPixmap pieceImage(piece->data(0).toString());

    pieceImage = pieceImage.scaled(40, 40);
    QGraphicsPixmapItem *storedItem = new QGraphicsPixmapItem(pieceImage);
    int itemCount = storageScene->items().size();
    int row = itemCount / 4;
    int col = itemCount % 4;
    storedItem->setPos(col * 40, row * 40);
    storageScene->addItem(storedItem);
}


void chess::updateLCD(int timeMs, QLCDNumber *lcd)
{
    int minutes = (timeMs / 60000) % 60;
    int seconds = (timeMs / 1000) % 60;
    lcd->display(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
}

void chess::updateWhiteTimer()
{
    whiteTimeRemaining -= 10;
    updateLCD(whiteTimeRemaining, ui->white_timer);

    if (whiteTimeRemaining <= 0)
    {
        whiteTimer.stop();
    }
}

void chess::updateBlackTimer()
{
    blackTimeRemaining -= 10;
    updateLCD(blackTimeRemaining, ui->black_timer);

    if (blackTimeRemaining <= 0)
    {
        blackTimer.stop();
    }
}

void chess::on_white_done_clicked()
{
    if (!pieceMovedInTurn) return;

    whiteTimer.stop();
    blackTimer.start(10);
    isWhiteTurn = false;
    pieceMovedInTurn = false;
    updateTurnDisplay();
    qDebug() << "백의 턴이 끝났습니다. 흑의 차례입니다.";
}

void chess::on_black_done_clicked()
{
    if (!pieceMovedInTurn) return;

    blackTimer.stop();
    whiteTimer.start(10);
    isWhiteTurn = true;
    pieceMovedInTurn = false;
    updateTurnDisplay();
    qDebug() << "흑의 턴이 끝났습니다. 백의 차례입니다.";
}

QString chess::getPieceType(QGraphicsPixmapItem* piece)
{
    QString path = piece->data(0).toString();
    if (path.contains("pawn")) return "pawn";
    if (path.contains("rook")) return "rook";
    if (path.contains("knight")) return "knight";
    if (path.contains("bishop")) return "bishop";
    if (path.contains("queen")) return "queen";
    if (path.contains("king")) return "king";
    return "";
}

bool chess::isValidMove(const QString& pieceType, int startRow, int startCol, int endRow, int endCol)
{
    int rowDiff = endRow - startRow;
    int colDiff = endCol - startCol;

    if (endRow < 0 || endRow > 7 || endCol < 0 || endCol > 7)
    {
        return false;
    }

    QGraphicsPixmapItem* targetPiece = nullptr;
    QList<QGraphicsItem*> items = scene->items(QPointF(endCol * 80 + 40, endRow * 80 + 40));
    for (QGraphicsItem* item : items)
    {
        if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item))
        {
            targetPiece = pixmapItem;
            break;
        }
    }

    if (isWhiteTurn)
    {
        qDebug() << "기물색: 흰색";
    }
    else
    {
        qDebug() << "기물색: 검은색";
    }
    qDebug() << "가로 칸수:" << rowDiff;
    qDebug() << "세로 칸수:" << colDiff;
    if (targetPiece != nullptr)
    {
        qDebug() << "상대 기물이 존재함";
    }
    else
    {
        qDebug() << "상대 기물이 존재하지 않음";
    }


    if (pieceType == "pawn")
    {
        if (colDiff == 0)
        {
            if (targetPiece)
            {
                qDebug() << "경로에 기물이 있습니다";
                return false;
            }

            if (isWhiteTurn)
            {
                if (rowDiff == -1) return true;
                if (startRow == 6 && rowDiff == -2)
                {
                    QList<QGraphicsItem*> midItems = scene->items(QPointF(startCol * 80 + 40, (startRow - 1) * 80 + 40));
                    bool midPathClear = true;
                    for (QGraphicsItem* item : midItems)
                    {
                        if (dynamic_cast<QGraphicsPixmapItem*>(item))
                        {
                            midPathClear = false;
                            break;
                        }
                    }
                    return midPathClear;
                }
            }
            else
            {
                if (rowDiff == 1) return true;
                if (startRow == 1 && rowDiff == 2)
                {
                    QList<QGraphicsItem*> midItems = scene->items(QPointF(startCol * 80 + 40, (startRow + 1) * 80 + 40));
                    bool midPathClear = true;
                    for (QGraphicsItem* item : midItems)
                    {
                        if (dynamic_cast<QGraphicsPixmapItem*>(item))
                        {
                            midPathClear = false;
                            break;
                        }
                    }
                    return midPathClear;
                }
            }
        }

        if (abs(colDiff) == 1)
        {
            if (isWhiteTurn && rowDiff == -1)
            {
                return targetPiece && !isSameColor(selectedPiece, targetPiece);
            }
            if (!isWhiteTurn && rowDiff == 1)
            {
                return targetPiece && !isSameColor(selectedPiece, targetPiece);
            }
        }

        return false;
    }

    if (pieceType == "rook")
    {
        return (startRow == endRow || startCol == endCol) && isPathClear(startRow, startCol, endRow, endCol);
    }

    if (pieceType == "bishop")
    {
        return abs(endRow - startRow) == abs(endCol - startCol) && isPathClear(startRow, startCol, endRow, endCol);
    }

    if (pieceType == "queen")
    {
        return (startRow == endRow || startCol == endCol || abs(endRow - startRow) == abs(endCol - startCol)) &&
               isPathClear(startRow, startCol, endRow, endCol);
    }

    if (pieceType == "knight")
    {
        return (abs(endRow - startRow) == 2 && abs(endCol - startCol) == 1) ||
               (abs(endRow - startRow) == 1 && abs(endCol - startCol) == 2);
    }

    if (pieceType == "king")
    {
        return abs(endRow - startRow) <= 1 && abs(endCol - startCol) <= 1;
    }

    return false;
}

bool chess::isPathClear(int startRow, int startCol, int endRow, int endCol)
{
    int rowStep;
    int colStep;
    if ((endRow - startRow) == 0)
    {
        rowStep = 0;
    }
    else
    {
        rowStep = (endRow - startRow) / abs(endRow - startRow);
    }

    if ((endCol - startCol) == 0)
    {
        colStep = 0;
    }
    else
    {
        colStep = (endCol - startCol) / abs(endCol - startCol);
    }

    int currentRow = startRow + rowStep;
    int currentCol = startCol + colStep;

    while (currentRow != endRow || currentCol != endCol)
    {
        QList<QGraphicsItem*> items = scene->items(QPointF(currentCol * 80 + 40, currentRow * 80 + 40));

        for (QGraphicsItem* item : items)
        {
            if (dynamic_cast<QGraphicsPixmapItem*>(item))
            {
                return false;
            }
        }

        currentRow += rowStep;
        currentCol += colStep;
    }
    return true;
}

void chess::declareWinner(const QString& winner)
{
    QMessageBox::information(this, "게임 종료", winner + " 승리!");
    resetGame();
}

void chess::checkTimeOver()
{
    if (whiteTimeRemaining <= 0)
    {
        declareWinner("검은색");
    }
    else if (blackTimeRemaining <= 0)
    {
        declareWinner("흰색");
    }
}

void chess::resetGame()
{
    scene->clear();

    pieceMovedInTurn = false;
    isWhiteTurn = true;
    updateTurnDisplay();

    if (ui->white_got->scene())
    {
        ui->white_got->scene()->clear();
    }
    if (ui->black_got->scene())
    {
        ui->black_got->scene()->clear();
    }

    whiteTimer.stop();
    blackTimer.stop();
    whiteTimeRemaining = 600000;
    blackTimeRemaining = 600000;
    updateLCD(whiteTimeRemaining, ui->white_timer);
    updateLCD(blackTimeRemaining, ui->black_timer);

    drawChessBoard();
    placePieces();

    qDebug() << "게임이 초기화되었습니다. 백의 턴입니다.";
}


void chess::on_white_giveup_clicked()
{
    declareWinner("검은색");
}

void chess::on_black_giveup_clicked()
{
    declareWinner("흰색");
}

void chess::on_help_button_clicked()
{
    QWidget *helpWindow = new QWidget();
    helpWindow->setWindowTitle("Chess Rules");

    QLabel *imageLabel = new QLabel(helpWindow);
    QPixmap chessRulesPixmap(":/images/chess_rule.png");

    imageLabel->setPixmap(chessRulesPixmap);
    imageLabel->setScaledContents(true);

    helpWindow->resize(chessRulesPixmap.size());

    // 창을 표시
    helpWindow->show();
}

void chess::updateTurnDisplay()
{
    if (isWhiteTurn)
    {
        ui->textBrowser->setText("백의 턴입니다");
    }
    else
    {
        ui->textBrowser->setText("흑의 턴입니다");
    }
}


void chess::promotePawn(QGraphicsPixmapItem* pawn, int row, int col)
{
    QDialog promotionDialog(this);
    promotionDialog.setWindowTitle("프로모션 선택");

    QVBoxLayout* layout = new QVBoxLayout(&promotionDialog);
    QPushButton* queenButton = new QPushButton("퀸", &promotionDialog);
    QPushButton* rookButton = new QPushButton("룩", &promotionDialog);
    QPushButton* bishopButton = new QPushButton("비숍", &promotionDialog);
    QPushButton* knightButton = new QPushButton("나이트", &promotionDialog);

    layout->addWidget(queenButton);
    layout->addWidget(rookButton);
    layout->addWidget(bishopButton);
    layout->addWidget(knightButton);

    QString color;
    if (pawn->data(0).toString().contains("white"))
    {
        color = "white";
    }
    else
    {
        color = "black";
    }
    // queenButton 클릭 이벤트 처리
    connect(queenButton, &QPushButton::clicked, [&]()
            {
                changePiece(pawn, QString(":/images/%1_queen.png").arg(color), row, col);
                promotionDialog.accept();
            });
    connect(rookButton, &QPushButton::clicked, [&]()
            {
                changePiece(pawn, QString(":/images/%1_rook.png").arg(color), row, col);
                promotionDialog.accept();
            });
    connect(bishopButton, &QPushButton::clicked, [&]()
            {
                changePiece(pawn, QString(":/images/%1_bishop.png").arg(color), row, col);
                promotionDialog.accept();
            });
    connect(knightButton, &QPushButton::clicked, [&]()
            {
                changePiece(pawn, QString(":/images/%1_knight.png").arg(color), row, col);
                promotionDialog.accept();
            });
}

void chess::changePiece(QGraphicsPixmapItem* oldPiece, const QString& newImagePath, int row, int col)
{
    scene->removeItem(oldPiece);
    delete oldPiece;
    QGraphicsPixmapItem* newPiece = addPiece(newImagePath, row, col);
    newPiece->setData(0, newImagePath);
}

void chess::on_debug_button_clicked()
{
    debugMode = !debugMode;

    if (debugMode)
    {
        qDebug() << "디버그 모드가 활성화되었습니다. 규칙이 무시됩니다.";
    }
    else
    {
        qDebug() << "디버그 모드가 비활성화되었습니다. 게임이 초기화됩니다.";
        resetGame();
    }
}

