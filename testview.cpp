#include "testview.h"
#include "MoveToZoom.h"
#include <QDebug>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsWebView>
//#define TEST_WEBVIEW
TestView::TestView(QWidget* parent)
         :QGraphicsView(parent)
{
    this->setObjectName("graphicsview");
#if QT_VERSION > 0x040702
    this->setAttribute(Qt::WA_LockPortraitOrientation,true);
#endif
    this->setAttribute(Qt::WA_AcceptTouchEvents,true);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGraphicsScene* s = new QGraphicsScene(this);
    this->setScene(s);

#ifdef TEST_WEBVIEW
    QGraphicsWebView* item = new QGraphicsWebView;
    item->setObjectName("webview");
    item->setAcceptTouchEvents(true);
    item->resize(360,640);
    item->setUrl(QUrl("http://m.engadget.com"));
    s->addItem(item);
    item->show();
#else
    QGraphicsPixmapItem* item = s->addPixmap(QPixmap(":/images/2.jpg"));
    QGraphicsTextItem* textitem = s->addText("Touch with 2 fingers and move the device towards/away from your face");
    textitem->setTextWidth(300);
    textitem->setPos(20,100);
#endif
    MoveToZoom* zm = new MoveToZoom(this);
    zm->setZoom(item);
}

// eof
