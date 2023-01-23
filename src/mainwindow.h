#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "indiclient.h"
#include "inditelescope.h"
#include "indirotator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    IndiClient client;
    IndiTelescope telescope{&client};
    IndiRotator rotator{&client};
    QSettings settings{"IndiAltAzDerorator"};

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
