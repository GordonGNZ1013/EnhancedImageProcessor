#ifndef IMAGETRANSFORM_H
#define IMAGETRANSFORM_H

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDial>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImage>

class ImageTransform : public QWidget
{
    Q_OBJECT

public:
    ImageTransform(QWidget *parent = 0);
    ~ImageTransform();
    
    // 公開方法用於載入和儲存圖片
    void loadImage(const QString &fileName);
    bool saveImage(const QString &fileName) const;
    QImage getCurrentImage() const;

private slots:
    void mirroredImage();
    void rotatedImage();

private:
    QLabel          *inWin;
    QGroupBox       *mirrorGroup;
    QCheckBox       *hCheckBox;
    QCheckBox       *vCheckBox;
    QPushButton     *mirrorButton;
    QDial           *rotateDial;
    QSpacerItem     *vSpacer;
    QHBoxLayout     *mainLayout;
    QVBoxLayout     *groupLayout;
    QVBoxLayout     *leftLayout;
    QImage          srcImg;
    QImage          dstImg;
};

#endif // IMAGETRANSFORM_H
