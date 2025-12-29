#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QImage>

class ResultWindow : public QWidget
{
    Q_OBJECT

public:
    ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();
    void setImage(const QImage &newImg);

private:
    QLabel *imgLabel;
};

#endif // RESULTWINDOW_H
