
#include <QtGui/QApplication>
#include "testview.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestView v;
    v.showFullScreen();
    return app.exec();
}
