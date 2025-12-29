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
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QMouseEvent>

class EnhancedImageProcessor : public QMainWindow
{
    Q_OBJECT

public:
    EnhancedImageProcessor(QWidget *parent = nullptr);
    ~EnhancedImageProcessor();

private slots:
    void openImage();
    void saveImage();
    void zoomIn();
    void zoomOut();
    void mirroredImage();
    void rotatedImage();
    void binaryImage();
    void histogramImage();
    void thresholdImage();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(const QString &filename);
    void showResultWindow(const QImage &image, const QString &title);
    
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
    QImage          originalImg;
    QString         currentFileName;
    
    // Actions
    QAction         *openAction;
    QAction         *saveAction;
    QAction         *exitAction;
    QAction         *zoomInAction;
    QAction         *zoomOutAction;
    QAction         *geometricAction;
    QAction         *binaryAction;
    QAction         *histogramAction;
    QAction         *thresholdAction;
    
    // Menus
    QMenu           *fileMenu;
    QMenu           *toolMenu;
    
    // Toolbar
    QToolBar        *mainToolBar;
};
#endif // ENHANCEDIMAGEPROCESSOR_H
