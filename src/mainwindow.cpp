#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QObjectList>
#include <QString>
#include <QGroupBox>
#include "cores/corefactory.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QGridLayout *layout = new QGridLayout;
    _centralWidget = new QWidget(this);

    QFile file("/etc/armbian-release");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    file.readLine();    // Skip header
    QString mCoreModel = file.readLine().trimmed().right(3);

    //_core = CoreFactory::make_core(mCoreModel.toStdString());
    _core = CoreFactory::make_core("a04");
    this->setCentralWidget(_centralWidget);
    this->centralWidget()->setLayout(layout);
    this->centralWidget()->layout()->addWidget(_core);

    _core->populateCpuFreq();
    _core->populateGpuFreq();
    _core->populateGovernors();
    _core->populatePresets();
}

MainWindow::~MainWindow()
{
    if (_core != nullptr) {
        delete _core;
        _core = nullptr;
    }
}
