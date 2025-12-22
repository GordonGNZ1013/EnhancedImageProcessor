#include "enhancedimageprocessor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EnhancedImageProcessor w;
    w.show();
    return a.exec();
}
