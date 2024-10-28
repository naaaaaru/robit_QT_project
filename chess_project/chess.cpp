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
    scene = new QGraphicsScene(this);//체스 메인판 생성자
    ui->graphicsView->setScene(scene);

    QGraphicsScene *whiteGotScene = new QGraphicsScene(this);
    //흰색이 잡은 기물을 보여주는 창 생성자
    ui->white_got->setScene(whiteGotScene);

    QGraphicsScene *blackGotScene = new QGraphicsScene(this);
    //검은색이 잡은 기물을 보여주는 창 생성자
    ui->black_got->setScene(blackGotScene);

    qDebug() << "체스 게임이 시작되었습니다. 흰색부터 시작하세요.";

    drawChessBoard();//체스판 그리기
    placePieces();//기물 배치

    updateLCD(whiteTime, ui->white_timer);
    updateLCD(blackTime, ui->black_timer);

    connect(&whiteTimer, &QTimer::timeout, this, &chess::updateWhiteTimer);
    connect(&blackTimer, &QTimer::timeout, this, &chess::updateBlackTimer);
    //connect:신호가 발생했을때 슬롯을 자동으로 호출
    //여기서는 10ms마다 timeout이 발동할때마다 타이머 함수를 작동해 타이머를 업데이트
}

chess::~chess()//소멸자
{
    delete ui;
}

void chess::drawChessBoard()
{
    int size = 80;//체스판 한칸의 사이즈,80x80
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            QRectF tile(j * size, i * size, size, size);
            //qt사각형,괄호는 순서대로 x위치,y위치,폭,높이
            QBrush brush;
            if ((i + j) % 2 == 0)
            {
                brush = Qt::white;
                //색깔 지정,체스판에 맞게 흰색과 검은색 지정
            }
            else
            {
                brush = Qt::black;
            }
            scene->addRect(tile, QPen(Qt::darkGray), brush);
            //타일을 메인 scene에 추가,테두리는 어두운 회색
        }
    }
}

QGraphicsPixmapItem* chess::addPiece(const QString& imagePath, int i, int j)
{
    QPixmap piece(imagePath);//경로를 사진 객체 piece생성
    piece = piece.scaled(80, 80);//사진의 사이즈
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(piece);
    //piece이미지를 사용하는 생성자,QGraphicsPixmapItem는 이미지 기물처럼 다룰수 있는 객체
    item->setPos(j * 80, i * 80);//위치
    item->setData(0, imagePath);//이미지 경로
    scene->addItem(item);//scene에 기물 배치

    return item;
}

void chess::placePieces()
{
    // 백 기물 배치
    for (int i = 0; i < 8; ++i)
    {
        addPiece(":/images/white_pawn.png", 6, i);//폰은 8개 배치
    }
    addPiece(":/images/white_rook.png", 7, 0);
    addPiece(":/images/white_knight.png", 7, 1);
    addPiece(":/images/white_bishop.png", 7, 2);
    addPiece(":/images/white_queen.png", 7, 3);
    addPiece(":/images/white_king.png", 7, 4);
    addPiece(":/images/white_bishop.png", 7, 5);
    addPiece(":/images/white_knight.png", 7, 6);
    addPiece(":/images/white_rook.png", 7, 7);

    // 흑 기물 배치
    for (int i = 0; i < 8; ++i)
    {
        addPiece(":/images/black_pawn.png", 1, i);
    }
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
//같은색 기물인지를 검사하는 프로그램
{
    QString path1 = piece1->data(0).toString();
    //data(0)슬롯을 사용해 파일 경로 가져옴
    QString path2 = piece2->data(0).toString();
    return path1.contains("white") == path2.contains("white");
    //bool을 사용해 아이템 두개가 각각 경로에 path를 포함하는지 판단하여 반환
}

void chess::mousePressEvent(QMouseEvent *event)
{
    QPointF clickPos = ui->graphicsView->mapToScene(event->pos());
    //mapToScene을 사용해 마우스 클릭위치를 가져와 clickpositiond으로 저
    QGraphicsItem *item = scene->itemAt(clickPos, QTransform());
    //클릭한 위치에 있는 기물 반환
    if (item && event->button() == Qt::LeftButton)
    {//좌클릭 했을때
        QGraphicsPixmapItem *piece = dynamic_cast<QGraphicsPixmapItem *>(item);
        //dynamic_cast는 상속관계가 잇는 클래스간 형 변환에 사용된다
        //QGraphicsPixmapItem의 객체를 다루어 기물이 있는지 검색하기 위해 사용
        //클릭했을때 아이템이 기물일때만
        if (piece)
        {
            if ((isWhiteTurn && piece->data(0).toString().contains("white")) ||
                (!isWhiteTurn && piece->data(0).toString().contains("black")))
            {
                selectedPiece = piece;//선택된 기물 저장
                originalPos = piece->pos();//선택된 기물의 원래 위치 저장
                qDebug() << "기물이 선택되었습니다.";//턴에 맞는 기물을 선택했을때만
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
    }//기물이 선택되지 않을때

    QPointF releasePos = ui->graphicsView->mapToScene(event->pos());
    //마우스를 놓은 위치 좌표 포인터 저장
    int tileSize = 80;
    int col = static_cast<int>(releasePos.x()) / tileSize;
    int row = static_cast<int>(releasePos.y()) / tileSize;
    //좌표를 타일사이즈로 나누어 위치를 n열,n행으로 구분

    if (row < 0 || row > 7 || col < 0 || col > 7)
    {
        qDebug() << "에러: 체스판 외부로 이동할 수 없습니다.";
        selectedPiece->setPos(originalPos);//변수 초기화
        selectedPiece = nullptr;
        return;
    }//체스판 바깥에 기물을 이동하려고 했을때

    if (!debugMode)
    {//턴에 관련된 오류 처리, 만약 디버깅 모드라면 턴과 관련없이 작동
        if (pieceMovedInTurn)
        //pieceMovedInTurn변수를 사용해 한턴에 한번만 움직일수 있도록 처리
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
            //턴이 아닐때 이동을 방지
            selectedPiece->setPos(originalPos);
            selectedPiece = nullptr;
            return;
        }

        QString pieceType = getPieceType(selectedPiece);//기물 종류
        //기물의 이동전 행,열 저장
        int startRow = static_cast<int>(originalPos.y()) / tileSize;
        int startCol = static_cast<int>(originalPos.x()) / tileSize;

        if (!isValidMove(pieceType, startRow, startCol, row, col))
            //행마법에 유효하는지 검사
        {
            qDebug() << "에러: 유효하지 않은 움직임입니다. 해당 기물 규칙을 따르지 않았습니다.";
            selectedPiece->setPos(originalPos);
            selectedPiece = nullptr;
            return;
        }
    }

    QGraphicsPixmapItem* capturedPiece = nullptr;
    //이동 위치에 상대 기물이 있는지 보여주기 위한 변수
    QList<QGraphicsItem*> items = scene->items(QPointF(col * 80 + 40, row * 80 + 40));
    //이동 위치에 이미지 객체를 아이템 리스트로 가져옴

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
        {//색깔 검색을 통해 같은 색의 기물은 잡을수 없음
            qDebug() << "에러: 같은 색의 기물은 잡을 수 없습니다.";
            selectedPiece->setPos(originalPos);
            selectedPiece = nullptr;
            return;
        }

        QString capturedPieceType = getPieceType(capturedPiece);
        //잡은 기물의 종류를 저장
        QGraphicsView *storageView;
        //저장공간 포인터 선언

        if (isWhiteTurn)
        {
            storageView = ui->white_got;
        }
        else
        {
            storageView = ui->black_got;
        }
        capturedShow(capturedPiece, storageView);
        //잡은 기물 표시

        if (capturedPieceType == "king")
        //킹을 잡았다면 게임 종료
        {
            if (isWhiteTurn)
            {
                finishGame("흰색");
            }
            else
            {
                finishGame("검은색");
            }

            return;
        }
        scene->removeItem(capturedPiece);
        delete capturedPiece;
        qDebug() << "알림: 기물을 잡았습니다!";
    }

    selectedPiece->setPos(col * 80, row * 80);
    //행과 열에 맞추어 기물의 새로운 위치를 지정

    QString pieceType = getPieceType(selectedPiece);//기물의 종류 검색
    if (!debugMode && pieceType == "pawn" && (row == 0 || row == 7))
    {
        promotePawn(selectedPiece, row, col);
        //폰이 끝까지 도달했다면 프로모션 발동
    }

    if (!debugMode)
    {
        selectedPiece = nullptr;//변수 초기화 및 설정
        pieceMovedInTurn = true;
        isWhiteTurn = !isWhiteTurn;
        qDebug() << "알림: 기물이 성공적으로 이동했습니다.";
    }
    else
    {
        selectedPiece = nullptr;
    }
}

void chess::capturedShow(QGraphicsPixmapItem* piece, QGraphicsView* storageView)
{
    QGraphicsScene *storageScene = storageView->scene();
    QPixmap pieceImage(piece->data(0).toString());
    //잡힌 기물을 qpixmap으로 가져옴

    pieceImage = pieceImage.scaled(40, 40);//기물 이미지 사이즈
    QGraphicsPixmapItem *storedItem = new QGraphicsPixmapItem(pieceImage);
    //기물 이미지를 storedItem포인터에 저장,동적할당
    int itemCount = storageScene->items().size();
    //저장소에 있는 기물 갯수 가져옴
    int row = itemCount / 4;//갯수에 맞게 행열 재조정
    int col = itemCount % 4;
    storedItem->setPos(col * 40, row * 40);//저장된 기물의 위치 지정
    storageScene->addItem(storedItem);//저장소에 기물 표현
}

void chess::updateLCD(int timeMs, QLCDNumber *lcd)
{
    int minutes = (timeMs / 60000) % 60;//분 표시
    int seconds = (timeMs / 1000) % 60;//초 표시
    lcd->display(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    //2자리수로 0을 채워 시간 표시
}

void chess::updateWhiteTimer()
{
    whiteTime -= 10;//총 시간에서 10ms를 감소
    updateLCD(whiteTime, ui->white_timer);//감소된 시간을 표현

    if (whiteTime <= 0)
    {
        whiteTimer.stop();//시간 음수표현 방지
    }
}

void chess::updateBlackTimer()
{
    blackTime -= 10;
    updateLCD(blackTime, ui->black_timer);

    if (blackTime <= 0)
    {
        blackTimer.stop();
    }
}

void chess::on_white_done_clicked()
{
    if (!pieceMovedInTurn) return;

    whiteTimer.stop();//턴이 종료됬기 때문에 타이머 종료
    blackTimer.start(10);//10ms마다 timeout발동
    isWhiteTurn = false;
    pieceMovedInTurn = false;
    updateTurn();//턴 관련 기능
    qDebug() << "백의 턴이 끝났습니다. 흑의 차례입니다.";
}

void chess::on_black_done_clicked()
{
    if (!pieceMovedInTurn) return;

    blackTimer.stop();
    whiteTimer.start(10);
    isWhiteTurn = true;
    pieceMovedInTurn = false;
    updateTurn();
    qDebug() << "흑의 턴이 끝났습니다. 백의 차례입니다.";
}

QString chess::getPieceType(QGraphicsPixmapItem* piece)
//기물의 종류를 알아내는 함수
{
    QString path = piece->data(0).toString();//이미지 경로에 각각 단어가 포함되있는지 확인
    if (path.contains("pawn"))
    {
        return "pawn";
    }
    if (path.contains("rook"))
    {
        return "rook";
    }
    if (path.contains("knight"))
    {
        return "knight";
    }
    if (path.contains("bishop"))
    {
        return "bishop";
    }
    if (path.contains("queen"))
    {
        return "queen";
    }
    if (path.contains("king"))
    {
        return "king";
    }
    return "";
}

bool chess::isValidMove(const QString& pieceType, int startRow, int startCol, int endRow, int endCol)
{
    int rowDiff = endRow - startRow;
    int colDiff = endCol - startCol;
    //행열 이동 거리 계산및 변수에 저장

    if (endRow < 0 || endRow > 7 || endCol < 0 || endCol > 7)
    {
        return false;
        //체스판을 벗어나는 이동 검사
    }

    QGraphicsPixmapItem* targetPiece = nullptr;
    //이동위치에 있는 기물을 저장하는 포인터
    QList<QGraphicsItem*> items = scene->items(QPointF(endCol * 80 + 40, endRow * 80 + 40));
    for (QGraphicsItem* item : items)
    {
        if (QGraphicsPixmapItem* pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item))
        {
            //위치에 있는 아이템을 가져와 기물이 있다면 targetPiece에 저장
            targetPiece = pixmapItem;
            break;
        }
    }

    if (isWhiteTurn)
    {
        qDebug() << "기물색: 흰색";//디버깅을 위해 기물색 표시
    }
    else
    {
        qDebug() << "기물색: 검은색";
    }
    qDebug() << "가로 칸수:" << rowDiff;
    qDebug() << "세로 칸수:" << colDiff;
    //디버깅을 위해 행열 이동 칸수 표시
    if (targetPiece != nullptr)
    {
        qDebug() << "상대 기물이 존재함";
        //디버깅을 위해 상대기물이 있는지 표시
    }
    else
    {
        qDebug() << "상대 기물이 존재하지 않음";
    }


    if (pieceType == "pawn")
    {
        if (colDiff == 0)//세로 방향 전진일때
        {
            if (targetPiece)
            {
                //폰은 전진으로 기물을 잡을수 없음
                qDebug() << "경로에 기물이 있습니다";
                return false;
            }

            if (isWhiteTurn)
            {
                if (rowDiff == -1)
                {
                    //경로에 기물이 없다면 한칸 전진은 유효
                    return true;
                }
                if (startRow == 6 && rowDiff == -2)
                {
                    //시작 위치에서는 두칸도 움직일수 있음
                    QList<QGraphicsItem*> Items = scene->items(QPointF(startCol * 80 + 40, (startRow - 1) * 80 + 40));
                    //두칸 이동중 가운데있는 항목 가져옴
                    bool midPathClear = true;
                    //중간에 기물이 있는지에 대한 변수
                    for (QGraphicsItem* item : Items)
                    {//만약 중간에 기물이 있다면 경로 불가능으로 반환
                        if (dynamic_cast<QGraphicsPixmapItem*>(item))
                        {
                            midPathClear = false;
                            break;
                        }
                    }
                    return midPathClear;
                }
            }
            else//흑색폰의 이동
            {
                if (rowDiff == 1)
                {
                    return true;
                }
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

        if (abs(colDiff) == 1)//대각선으로 움직일때
        {
            if (isWhiteTurn && rowDiff == -1)
            {
                //상대 기물이 있는지 확인하고 같은색의 기물이 아닌지 확인
                //만약 둘다 유효하다면 이동은 유효
                return targetPiece && !isSameColor(selectedPiece, targetPiece);
            }
            if (!isWhiteTurn && rowDiff == 1)
            {
                return targetPiece && !isSameColor(selectedPiece, targetPiece);
            }
        }

        return false;//아닌경우 폰은 움직일수 없음
    }

    if (pieceType == "rook")
    {
        return (startRow == endRow || startCol == endCol) && isPathClear(startRow, startCol, endRow, endCol);
        //룩은 가로혹은 세로로만 움직일수 있으며
        //경로에 기물이 없어야 함
    }

    if (pieceType == "bishop")
    {
        return abs(endRow - startRow) == abs(endCol - startCol) && isPathClear(startRow, startCol, endRow, endCol);
        //비숍은 대각선으로만 움직여야함으로
        //가로세로 이동칸수 차이가 같고 경로에 기물이 없어야함
    }

    if (pieceType == "queen")
    {
        return (startRow == endRow || startCol == endCol || abs(endRow - startRow) == abs(endCol - startCol)) &&
               isPathClear(startRow, startCol, endRow, endCol);
        //퀸은 룩과 비숍이 합쳐진 형태
    }

    if (pieceType == "knight")
    {
        return (abs(endRow - startRow) == 2 && abs(endCol - startCol) == 1) ||
               (abs(endRow - startRow) == 1 && abs(endCol - startCol) == 2);
        //나이트는 가로 두칸+세로 한칸
        //또는 세로 한칸+가로 두칸으로 이동 가능
    }

    if (pieceType == "king")
    {
        return abs(endRow - startRow) <= 1 && abs(endCol - startCol) <= 1;
        //킹은 어느방향으로든 한칸 이동 가능
    }

    return false;
}

bool chess::isPathClear(int startRow, int startCol, int endRow, int endCol)
{
    int rowStep;//진행해야 하는 행과 열변수
    int colStep;

    if ((endRow - startRow) == 0)//열 계산
    {
        rowStep = 0;
    }
    else
    {
        rowStep = (endRow - startRow) / abs(endRow - startRow);
        //진행 방향의 방향 계산
    }

    if ((endCol - startCol) == 0)//행 계산
    {
        colStep = 0;
    }
    else
    {
        colStep = (endCol - startCol) / abs(endCol - startCol);
    }

    int currentRow = startRow + rowStep;
    int currentCol = startCol + colStep;
    //current:진행방향을 한칸씩 이동하면서 검사하기 위한 변수
    //Step이 더해지는 이유는 시작위치에 있는 원래 기물을 제외하기 위해서

    while (currentRow != endRow || currentCol != endCol)
    //위치가 목표 위치에 도달할때까지 반복
    {
        QList<QGraphicsItem*> items = scene->items(QPointF(currentCol * 80 + 40, currentRow * 80 + 40));
        //진행 방향에 기물이 있는지 검사
        for (QGraphicsItem* item : items)
        {
            if (dynamic_cast<QGraphicsPixmapItem*>(item))
            {
                return false;
                //경로에 기물이 있다면 반환
            }
        }

        currentRow += rowStep;//한칸씩 이동
        currentCol += colStep;
    }
    return true;
}

void chess::finishGame(const QString& winner)
{
    QMessageBox::information(this, "게임 종료", winner + " 승리!");
    //qmessage로 승패 표시
    resetGame();//초기화
}

void chess::checkTimeOver()//시간에 의한 승패
{
    if (whiteTime <= 0)
    {
        finishGame("검은색");
    }
    else if (blackTime<= 0)
    {
        finishGame("흰색");
    }
}

void chess::resetGame()
{
    scene->clear();//scene초기화

    pieceMovedInTurn = false;//변수 초기화
    isWhiteTurn = true;

    updateTurn();//다시 흰색 턴으로

    if (ui->white_got->scene())//잡힌 기물 저장소 초기화
    {
        ui->white_got->scene()->clear();
    }
    if (ui->black_got->scene())
    {
        ui->black_got->scene()->clear();
    }

    whiteTimer.stop();//타이머 초기화
    blackTimer.stop();
    whiteTime = 600000;
    blackTime = 600000;
    updateLCD(whiteTime, ui->white_timer);
    updateLCD(blackTime, ui->black_timer);

    drawChessBoard();
    placePieces();

    qDebug() << "게임이 초기화되었습니다. 백의 턴입니다.";
}


void chess::on_white_giveup_clicked()
{
    finishGame("검은색");//기권 버튼
}

void chess::on_black_giveup_clicked()
{
    finishGame("흰색");
}

void chess::on_help_button_clicked()
{
    QWidget *helpWindow = new QWidget();//창 생성자
    helpWindow->setWindowTitle("Chess Rules");//제목은 체스 규칙

    QLabel *imageLabel = new QLabel(helpWindow);//imageLabel표시 위젯
    QPixmap chessRulesPixmap(":/images/chess_rule.png");
    //체스 룰 이미지 로드해 이미지레이블에 저장

    imageLabel->setPixmap(chessRulesPixmap);//이미지 추가
    imageLabel->setScaledContents(true);//이미지가 qlabel에 맞게 자동으로 조정

    helpWindow->resize(chessRulesPixmap.size());//창이 이미지에 맞게 크기 조정
    helpWindow->show();
}

void chess::updateTurn()//턴을 알려주는 qtextbrowser
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
    QDialog promotion(this);//qdialog 생성
    promotion.setWindowTitle("프로모션 선택");//제목은 프로모션 선택

    QVBoxLayout* layout = new QVBoxLayout(&promotion);
    //QVBoxLayout을 생성, QPushButton을 수직으로 배치
    QPushButton* queenButton = new QPushButton("퀸", &promotion);
    QPushButton* rookButton = new QPushButton("룩", &promotion);
    QPushButton* bishopButton = new QPushButton("비숍", &promotion);
    QPushButton* knightButton = new QPushButton("나이트", &promotion);
    //QPushButton생성,버튼에 각각 기물 이름 표시

    layout->addWidget(queenButton);
    layout->addWidget(rookButton);
    layout->addWidget(bishopButton);
    layout->addWidget(knightButton);
    //버튼을 레이아웃에 추가

    QString color;//기물 변환을 위한 색깔 변수
    if (pawn->data(0).toString().contains("white"))
    //기물색깔 검사
    {
        color = "white";
    }
    else
    {
        color = "black";
    }

    connect(queenButton, &QPushButton::clicked, [&]()
            {//connect를 사용해 버튼을 클릭했을때 기물 변환 작동
        changePiece(pawn, QString(":/images/%1_queen.png").arg(color), row, col);
        promotion.accept();//기물 선택창 닫기
    });
    connect(rookButton, &QPushButton::clicked, [&]()
            {
        changePiece(pawn, QString(":/images/%1_rook.png").arg(color), row, col);
        promotion.accept();
    });
    connect(bishopButton, &QPushButton::clicked, [&]()
            {
        changePiece(pawn, QString(":/images/%1_bishop.png").arg(color), row, col);
        promotion.accept();
    });
    connect(knightButton, &QPushButton::clicked, [&]()
            {
        changePiece(pawn, QString(":/images/%1_knight.png").arg(color), row, col);
        promotion.accept();
    });

    promotion.exec();
}

void chess::changePiece(QGraphicsPixmapItem* oldPiece, const QString& newImagePath, int row, int col)//프로모션에 사용되는 기물 변환 함수
{
    scene->removeItem(oldPiece);//화면에서 기존 기물 삭제,프로모션에서 사용될경우 항상 폰
    delete oldPiece;//메모리 해제
    QGraphicsPixmapItem* newPiece = addPiece(newImagePath, row, col);//동적할당해 포인터에 저장
}

void chess::on_debug_button_clicked()
{
    debugMode = !debugMode;//디버깅 모드 전환

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
