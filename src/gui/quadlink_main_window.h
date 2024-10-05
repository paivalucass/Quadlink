#ifndef QUADLINK_MAIN_WINDOW_H
#define QUADLINK_MAIN_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class quadlink_main_window;
}
QT_END_NAMESPACE

class quadlink_main_window : public QMainWindow
{
    Q_OBJECT

public:
    quadlink_main_window(QWidget *parent = nullptr);
    ~quadlink_main_window();

private:
    Ui::quadlink_main_window *ui;
};
#endif // QUADLINK_MAIN_WINDOW_H
