#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/statvfs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>
#include "shared.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Start(int core_index);

public slots:
    void HorizontalSliderFunc1(int value);
    void HorizontalSliderFunc2(int value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void Process(int core_index);

    std::thread th;
    std::mutex mtx;

    int counter = 0;
    int value_1 = 0;
    int value_2 = 0;
};
#endif // MAINWINDOW_H
