#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/statvfs.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
            this, SLOT(HorizontalSliderFunc1(int)));
    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)),
            this, SLOT(HorizontalSliderFunc2(int)));

    Start(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Start(int core_index)
{
    th = std::thread(&MainWindow::Process,
                     this, core_index);
}

void MainWindow::Process(int core_index)
{
    cpu_set_t cpu;
    CPU_ZERO(&cpu);
    CPU_SET(core_index, &cpu);

    if (sched_setaffinity(syscall(SYS_gettid),sizeof(cpu),&cpu) == -1)
        std::cout << "Core Error!" << std::endl;

    for (;;) {
        std::lock_guard<std::mutex> grd(mtx);

        if (Shared::GetTCPConnStatus())
            ui->label_2->setText("Connected");
        else
            ui->label_2->setText("Not connected");

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (Shared::GetTCPConnStatus()) {
        Shared::send_data.clear();
        Shared::send_data.push_back(static_cast<unsigned char>(value_1));
        Shared::send_data.push_back(static_cast<unsigned char>(value_2));
    }
}

void MainWindow::HorizontalSliderFunc1(int value)
{
    value_1 = value;
    ui->label_3->setText(QString::number(value_1));
}

void MainWindow::HorizontalSliderFunc2(int value)
{
    value_2 = value;
    ui->label_4->setText(QString::number(value_2));
}
