#ifndef TESTVIEW_H
#define TESTVIEW_H
#include <QGraphicsView>
// This is a test view to demonstrate MoveToZoom
class QGraphicsTextItem;
class TestView : public QGraphicsView
{
    Q_OBJECT
public:
    TestView(QWidget* parent = 0);
};

#endif // TESTVIEW_H
