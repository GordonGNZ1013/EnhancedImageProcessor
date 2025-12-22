#include "imagetransform.h"
#include <QPixmap>
#include <QFileDialog>

ImageTransform::ImageTransform(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    leftLayout = new QVBoxLayout();
    
    mirrorGroup = new QGroupBox(QString::fromLocal8Bit("鏡射"), this);
    groupLayout = new QVBoxLayout(mirrorGroup);
    
    hCheckBox = new QCheckBox(QString::fromLocal8Bit("水平"), mirrorGroup);
    vCheckBox = new QCheckBox(QString::fromLocal8Bit("垂直"), mirrorGroup);
    mirrorButton = new QPushButton(QString::fromLocal8Bit("執行"), mirrorGroup);
    
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
    if (srcImg.isNull()) {
        QPixmap *initPixmap = new QPixmap(300, 200);
        initPixmap->fill(QColor(255, 255, 255));
        inWin->setPixmap(*initPixmap);
        srcImg = dstImg = initPixmap->toImage();
    }
    
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(inWin);
    
    connect(mirrorButton, SIGNAL(clicked()), this, SLOT(mirroredImage()));
    connect(rotateDial, SIGNAL(valueChanged(int)), this, SLOT(rotatedImage()));
}

ImageTransform::~ImageTransform()
{
}

void ImageTransform::loadImage(const QString &fileName)
{
    srcImg.load(fileName);
    if (!srcImg.isNull()) {
        dstImg = srcImg;
        inWin->setPixmap(QPixmap::fromImage(srcImg));
    }
}

bool ImageTransform::saveImage(const QString &fileName) const
{
    return dstImg.save(fileName);
}

QImage ImageTransform::getCurrentImage() const
{
    return dstImg;
}

void ImageTransform::mirroredImage()
{
    bool H, V;
    if (srcImg.isNull())
        return;
    H = hCheckBox->isChecked();
    V = vCheckBox->isChecked();
    dstImg = srcImg.mirrored(H, V);
    inWin->setPixmap(QPixmap::fromImage(dstImg));
    srcImg = dstImg;
}

void ImageTransform::rotatedImage()
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
