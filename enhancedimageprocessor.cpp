#include "enhancedimageprocessor.h"
#include "resultwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QTransform>
#include <QRect>
#include <QInputDialog>
#include <QPainter>
#include <QStatusBar>
#include <QFont>

EnhancedImageProcessor::EnhancedImageProcessor(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("影像處理"));
    setGeometry(100, 100, 1000, 600);
    
    setupUI();
    createActions();
    createMenus();
    createToolBars();
}

EnhancedImageProcessor::~EnhancedImageProcessor()
{
}

void EnhancedImageProcessor::createActions()
{
    // 檔案菜單動作
    openAction = new QAction(QStringLiteral("開啟檔案(&O)"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(QStringLiteral("開啟影像檔案"));
    connect(openAction, &QAction::triggered, this, &EnhancedImageProcessor::openImage);
    
    saveAction = new QAction(QStringLiteral("儲存圖片(&S)"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(QStringLiteral("儲存處理後的圖片"));
    connect(saveAction, &QAction::triggered, this, &EnhancedImageProcessor::saveImage);
    
    exitAction = new QAction(QStringLiteral("離開(&Q)"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(QStringLiteral("退出程式"));
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    // 工具菜單動作
    zoomInAction = new QAction(QStringLiteral("放大"), this);
    zoomInAction->setShortcut(tr("Ctrl+]"));
    zoomInAction->setStatusTip(QStringLiteral("放大影像"));
    connect(zoomInAction, &QAction::triggered, this, &EnhancedImageProcessor::zoomIn);
    
    zoomOutAction = new QAction(QStringLiteral("縮小"), this);
    zoomOutAction->setShortcut(tr("Ctrl+["));
    zoomOutAction->setStatusTip(QStringLiteral("縮小影像"));
    connect(zoomOutAction, &QAction::triggered, this, &EnhancedImageProcessor::zoomOut);
    
    geometricAction = new QAction(QStringLiteral("幾何轉換"), this);
    geometricAction->setStatusTip(QStringLiteral("鏡射和旋轉功能"));
    geometricAction->setCheckable(false);
    
    binaryAction = new QAction(QStringLiteral("二值化"), this);
    binaryAction->setStatusTip(QStringLiteral("將圖像轉換為黑白二值圖"));
    connect(binaryAction, &QAction::triggered, this, &EnhancedImageProcessor::binaryImage);
    
    histogramAction = new QAction(QStringLiteral("直方圖"), this);
    histogramAction->setStatusTip(QStringLiteral("顯示圖像直方圖"));
    connect(histogramAction, &QAction::triggered, this, &EnhancedImageProcessor::histogramImage);
    
    thresholdAction = new QAction(QStringLiteral("閾值化"), this);
    thresholdAction->setStatusTip(QStringLiteral("應用閾值處理"));
    connect(thresholdAction, &QAction::triggered, this, &EnhancedImageProcessor::thresholdImage);
}

void EnhancedImageProcessor::createMenus()
{
    // 檔案菜單
    fileMenu = menuBar()->addMenu(QStringLiteral("檔案(&F)"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    
    // 工具菜單
    toolMenu = menuBar()->addMenu(QStringLiteral("工具(&T)"));
    toolMenu->addAction(zoomInAction);
    toolMenu->addAction(zoomOutAction);
    toolMenu->addSeparator();
    toolMenu->addAction(geometricAction);
    toolMenu->addAction(binaryAction);
    toolMenu->addAction(histogramAction);
    toolMenu->addAction(thresholdAction);
}

void EnhancedImageProcessor::createToolBars()
{
    mainToolBar = addToolBar(QStringLiteral("主工具列"));
    mainToolBar->addAction(openAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(zoomInAction);
    mainToolBar->addAction(zoomOutAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(geometricAction);
    mainToolBar->addAction(binaryAction);
    mainToolBar->addAction(histogramAction);
    mainToolBar->addAction(thresholdAction);
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
        loadFile(fileName);
    }
}

void EnhancedImageProcessor::loadFile(const QString &filename)
{
    if (srcImg.load(filename)) {
        currentFileName = filename;
        originalImg = srcImg;  // 保存原始圖像
        dstImg = srcImg;
        inWin->setPixmap(QPixmap::fromImage(srcImg));
        
        // 重置旋轉角度
        rotateDial->setValue(0);
        
        // 取消鏡射選項
        hCheckBox->setChecked(false);
        vCheckBox->setChecked(false);
        
        statusBar()->showMessage(QStringLiteral("已載入: ") + filename, 3000);
    } else {
        QMessageBox::warning(this, QStringLiteral("錯誤"),
            QStringLiteral("無法載入圖片"));
    }
}

void EnhancedImageProcessor::saveImage()
{
    if (dstImg.isNull()) {
        QMessageBox::warning(this, QStringLiteral("警告"),
            QStringLiteral("沒有可儲存的圖片"));
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this,
        QStringLiteral("儲存圖片"), "",
        QStringLiteral("PNG 圖片 (*.png);;JPG 圖片 (*.jpg);;BMP 圖片 (*.bmp);;所有檔案 (*)"));
    
    if (!fileName.isEmpty()) {
        if (dstImg.save(fileName)) {
            statusBar()->showMessage(QStringLiteral("圖片已儲存: ") + fileName, 3000);
        } else {
            QMessageBox::warning(this, QStringLiteral("錯誤"),
                QStringLiteral("無法儲存圖片"));
        }
    }
}

void EnhancedImageProcessor::zoomIn()
{
    if (originalImg.isNull()) {
        QMessageBox::information(this, QStringLiteral("提示"),
            QStringLiteral("請先載入圖片"));
        return;
    }
    
    // 從當前顯示的圖像（dstImg）放大
    QImage newImg = dstImg.scaled(dstImg.width() * 2, dstImg.height() * 2,
        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    showResultWindow(newImg, QStringLiteral("放大結果"));
}

void EnhancedImageProcessor::zoomOut()
{
    if (originalImg.isNull()) {
        QMessageBox::information(this, QStringLiteral("提示"),
            QStringLiteral("請先載入圖片"));
        return;
    }
    
    // 從當前顯示的圖像（dstImg）縮小
    QImage newImg = dstImg.scaled(dstImg.width() / 2, dstImg.height() / 2,
        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    showResultWindow(newImg, QStringLiteral("縮小結果"));
}

void EnhancedImageProcessor::showResultWindow(const QImage &image, const QString &title)
{
    ResultWindow *resultWin = new ResultWindow(this);
    resultWin->setWindowTitle(title);
    resultWin->setImage(image);
    resultWin->show();
}

void EnhancedImageProcessor::mirroredImage()
{
    bool H, V;
    if (originalImg.isNull())
        return;
    H = hCheckBox->isChecked();
    V = vCheckBox->isChecked();
    
    // 從原始圖像開始處理
    srcImg = originalImg;
    
    // 應用鏡射
    if (H || V) {
        Qt::Orientations orientation;
        if (H) orientation |= Qt::Horizontal;
        if (V) orientation |= Qt::Vertical;
        srcImg = srcImg.flipped(orientation);
    }
    
    // 重新應用旋轉（如果有的話）
    int angle = rotateDial->value();
    if (angle != 0) {
        QTransform tran;
        tran.rotate(angle);
        dstImg = srcImg.transformed(tran);
    } else {
        dstImg = srcImg;
    }
    
    inWin->setPixmap(QPixmap::fromImage(dstImg));
}

void EnhancedImageProcessor::rotatedImage()
{
    if (originalImg.isNull())
        return;
    
    // 從原始圖像開始
    srcImg = originalImg;
    
    // 先應用鏡射（如果有的話）
    bool H = hCheckBox->isChecked();
    bool V = vCheckBox->isChecked();
    if (H || V) {
        Qt::Orientations orientation;
        if (H) orientation |= Qt::Horizontal;
        if (V) orientation |= Qt::Vertical;
        srcImg = srcImg.flipped(orientation);
    }
    
    // 再應用旋轉
    int angle = rotateDial->value();
    if (angle != 0) {
        QTransform tran;
        tran.rotate(angle);
        dstImg = srcImg.transformed(tran);
    } else {
        dstImg = srcImg;
    }
    
    inWin->setPixmap(QPixmap::fromImage(dstImg));
}

void EnhancedImageProcessor::binaryImage()
{
    if (originalImg.isNull()) {
        QMessageBox::information(this, QStringLiteral("提示"),
            QStringLiteral("請先載入圖片"));
        return;
    }
    
    // 從當前處理後的圖像轉換為灰階
    QImage grayImg = dstImg.convertToFormat(QImage::Format_Grayscale8);
    
    // 使用固定閾值127進行二值化
    QImage binaryImg = grayImg.copy();
    int threshold = 127;
    
    for (int y = 0; y < binaryImg.height(); ++y) {
        for (int x = 0; x < binaryImg.width(); ++x) {
            QRgb pixel = binaryImg.pixel(x, y);
            int gray = qGray(pixel);
            
            if (gray > threshold) {
                binaryImg.setPixel(x, y, qRgb(255, 255, 255)); // 白色
            } else {
                binaryImg.setPixel(x, y, qRgb(0, 0, 0)); // 黑色
            }
        }
    }
    
    showResultWindow(binaryImg, QStringLiteral("二值化結果 (閾值=127)"));
}

void EnhancedImageProcessor::histogramImage()
{
    if (originalImg.isNull()) {
        QMessageBox::information(this, QStringLiteral("提示"),
            QStringLiteral("請先載入圖片"));
        return;
    }
    
    // 計算灰階直方圖（使用處理後的圖像）
    int histogram[256] = {0};
    QImage grayImg = dstImg.convertToFormat(QImage::Format_Grayscale8);
    
    for (int y = 0; y < grayImg.height(); ++y) {
        for (int x = 0; x < grayImg.width(); ++x) {
            QRgb pixel = grayImg.pixel(x, y);
            int gray = qGray(pixel);
            histogram[gray]++;
        }
    }
    
    // 找出最大值用於正規化
    int maxCount = 0;
    for (int i = 0; i < 256; ++i) {
        if (histogram[i] > maxCount) {
            maxCount = histogram[i];
        }
    }
    
    // 繪製直方圖
    int histWidth = 512;
    int histHeight = 400;
    QImage histImg(histWidth, histHeight, QImage::Format_RGB32);
    histImg.fill(Qt::white);
    
    // 繪製網格線
    QPainter painter(&histImg);
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    for (int i = 0; i <= 10; ++i) {
        int y = i * histHeight / 10;
        painter.drawLine(0, y, histWidth, y);
    }
    
    // 繪製直方圖柱狀圖
    painter.setPen(QPen(Qt::blue, 2));
    int barWidth = histWidth / 256;
    for (int i = 0; i < 256; ++i) {
        if (maxCount > 0) {
            int barHeight = (histogram[i] * (histHeight - 20)) / maxCount;
            int x = i * barWidth;
            painter.drawLine(x, histHeight - barHeight, x, histHeight);
        }
    }
    
    // 添加標題
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(10, 20, QStringLiteral("灰階直方圖"));
    
    showResultWindow(histImg, QStringLiteral("直方圖"));
}

void EnhancedImageProcessor::thresholdImage()
{
    if (originalImg.isNull()) {
        QMessageBox::information(this, QStringLiteral("提示"),
            QStringLiteral("請先載入圖片"));
        return;
    }
    
    // 讓用戶輸入閾值
    bool ok;
    int threshold = QInputDialog::getInt(this, 
        QStringLiteral("閾值化"),
        QStringLiteral("請輸入閾值 (0-255):"),
        127, 0, 255, 1, &ok);
    
    if (!ok) return;
    
    // 轉換為灰階（使用處理後的圖像）
    QImage grayImg = dstImg.convertToFormat(QImage::Format_Grayscale8);
    QImage thresholdImg = grayImg.copy();
    
    for (int y = 0; y < thresholdImg.height(); ++y) {
        for (int x = 0; x < thresholdImg.width(); ++x) {
            QRgb pixel = thresholdImg.pixel(x, y);
            int gray = qGray(pixel);
            
            if (gray > threshold) {
                thresholdImg.setPixel(x, y, qRgb(255, 255, 255));
            } else {
                thresholdImg.setPixel(x, y, qRgb(0, 0, 0));
            }
        }
    }
    
    showResultWindow(thresholdImg, 
        QStringLiteral("閾值化結果 (閾值=%1)").arg(threshold));
}
