#ifndef ENHANCEDIMAGEPROCESSOR_H
#define ENHANCEDIMAGEPROCESSOR_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QDial>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

class EnhancedImageProcessor : public QMainWindow
{
    Q_OBJECT

public:
    EnhancedImageProcessor(QWidget *parent = nullptr);
    ~EnhancedImageProcessor();

private slots:
    void openImage();
    void saveImage();
    void mirroredImage();
    void rotatedImage();

private:
    void setupUI();
    
    QWidget         *centralWidget;
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
#endif // ENHANCEDIMAGEPROCESSOR_H
