#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots: // function called in response to a particular signal
    void on_actionRectangle_triggered();
    void on_actionCircle_triggered();
    void on_actionTriangle_triggered();
    void on_actionCustom_triggered();
    void disableWidgets(QString str); // Disable non-applicable widgets based on action

private:
    Ui::MainWindow *ui;

signals: // emitted when a particular event occurs
    void actionTriggered(QString str);


};

#endif // MAINWINDOW_H
