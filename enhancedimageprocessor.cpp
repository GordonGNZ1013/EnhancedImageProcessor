#include "enhancedimageprocessor.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QTransform>
#include <QRect>

EnhancedImageProcessor::EnhancedImageProcessor(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("圖像變換工具"));
    setGeometry(100, 100, 1000, 600);
    
    setupUI();
    
    // 創建菜單欄
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    QMenu *fileMenu = menuBar->addMenu(QStringLiteral("檔案"));
    
    QAction *openAction = fileMenu->addAction(QStringLiteral("開啟圖片"));
    QAction *saveAction = fileMenu->addAction(QStringLiteral("儲存圖片"));
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction(QStringLiteral("離開"));
    
    connect(openAction, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveImage()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

EnhancedImageProcessor::~EnhancedImageProcessor()
{
}

void EnhancedImageProcessor::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QHBoxLayout(centralWidget);
    leftLayout = new QVBoxLayout();
    
    mirrorGroup = new QGroupBox(QStringLiteral("鏡射"), this);
    groupLayout = new QVBoxLayout(mirrorGroup);
    
    hCheckBox = new QCheckBox(QStringLiteral("水平"), mirrorGroup);
    vCheckBox = new QCheckBox(QStringLiteral("垂直"), mirrorGroup);
    mirrorButton = new QPushButton(QStringLiteral("執行"), mirrorGroup);
    
    hCheckBox->setGeometry(QRect(13, 28, 87, 19));
    vCheckBox->setGeometry(QRect(13, 54, 87, 19));
    mirrorButton->setGeometry(QRect(13, 80, 93, 28));
    
    groupLayout->addWidget(hCheckBox);
    groupLayout->addWidget(vCheckBox);
    groupLayout->addWidget(mirrorButton);
    
    rotateDial = new QDial(this);
    rotateDial->setGeometry(QRect(20, 130, 100, 100));
    rotateDial->setMinimum(0);
    rotateDial->setMaximum(359);
    rotateDial->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    
    leftLayout->addWidget(mirrorGroup);
    leftLayout->addWidget(rotateDial);
    leftLayout->addSpacerItem(vSpacer);
    
    inWin = new QLabel(this);
    inWin->setScaledContents(true);
    QPixmap initPixmap(300, 200);
    initPixmap.fill(QColor(255, 255, 255));
    inWin->setPixmap(initPixmap);
    inWin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    srcImg = dstImg = initPixmap.toImage();
    
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(inWin);
    
    connect(mirrorButton, SIGNAL(clicked()), this, SLOT(mirroredImage()));
    connect(rotateDial, SIGNAL(valueChanged(int)), this, SLOT(rotatedImage()));
}

void EnhancedImageProcessor::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        QStringLiteral("開啟圖片"), "",
        QStringLiteral("圖片檔案 (*.png *.jpg *.jpeg *.bmp *.gif);;所有檔案 (*)"));
    
    if (!fileName.isEmpty()) {
        srcImg.load(fileName);
        if (!srcImg.isNull()) {
            dstImg = srcImg;
            inWin->setPixmap(QPixmap::fromImage(srcImg));
            QMessageBox::information(this, QStringLiteral("成功"),
                QStringLiteral("圖片載入成功"));
        } else {
            QMessageBox::warning(this, QStringLiteral("錯誤"),
                QStringLiteral("無法載入圖片"));
        }
    }
}

void EnhancedImageProcessor::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        QStringLiteral("儲存圖片"), "",
        QStringLiteral("PNG 圖片 (*.png);;JPG 圖片 (*.jpg);;BMP 圖片 (*.bmp);;所有檔案 (*)"));
    
    if (!fileName.isEmpty()) {
        if (dstImg.save(fileName)) {
            QMessageBox::information(this, QStringLiteral("成功"),
                QStringLiteral("圖片已成功儲存到: ") + fileName);
        } else {
            QMessageBox::warning(this, QStringLiteral("錯誤"),
                QStringLiteral("無法儲存圖片"));
        }
    }
}

void EnhancedImageProcessor::mirroredImage()
{
    bool H, V;
    if (srcImg.isNull())
        return;
    H = hCheckBox->isChecked();
    V = vCheckBox->isChecked();
    Qt::Orientations orientation;
    if (H) orientation |= Qt::Horizontal;
    if (V) orientation |= Qt::Vertical;
    dstImg = srcImg.flipped(orientation);
    inWin->setPixmap(QPixmap::fromImage(dstImg));
    srcImg = dstImg;
}

void EnhancedImageProcessor::rotatedImage()
{
    QTransform tran;
    int angle;
    if (srcImg.isNull())
        return;
    angle = rotateDial->value();
    tran.rotate(angle);
    dstImg = srcImg.transformed(tran);
    inWin->setPixmap(QPixmap::fromImage(dstImg));
}
