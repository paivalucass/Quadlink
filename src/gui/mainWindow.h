#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include "../ros/src/quadlink_controller/src/subscriber.h"


namespace quadlink{
// This is the declaration of our MainWidget class
// The definition/implementation is in mainwidget.cpp
    class MainWindow : public QWidget
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0); //Constructor
        ~MainWindow(); // Destructor

    private:
        QImage currentImage;
        std::shared_ptr<quadlink::Subscriber> subscriber_node;
    };
}

#endif // MAINWIDGET_H