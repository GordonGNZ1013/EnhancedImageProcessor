#include "resultwindow.h"
#include <QVBoxLayout>
#include <QPixmap>

ResultWindow::ResultWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("處理結果"));
    
    // 設置為獨立視窗，並添加關閉按鈕
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    imgLabel = new QLabel();
    imgLabel->setAlignment(Qt::AlignCenter);
    imgLabel->setScaledContents(true);
    layout->addWidget(imgLabel);
    
    setLayout(layout);
    
    // 關閉時自動刪除，防止記憶體洩漏
    setAttribute(Qt::WA_DeleteOnClose);
}

ResultWindow::~ResultWindow()
{
}

void ResultWindow::setImage(const QImage &newImg)
{
    imgLabel->setPixmap(QPixmap::fromImage(newImg));
    resize(newImg.width(), newImg.height());
}
