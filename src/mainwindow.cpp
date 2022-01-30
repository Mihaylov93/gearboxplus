#include "mainwindow.hpp"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QObjectList>
#include <QString>
#include <QGroupBox>
#include <QGridLayout>
#include "cores/corefactory.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon("qrc:/icons/cil-gauge.svg"));
    QGridLayout *layout = new QGridLayout(this);
    _centralWidget = new QWidget(this);

    QFile file("/etc/armbian-release");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    file.readLine();    // Skip header
    QString mCoreModel = file.readLine().trimmed().right(3);

    _core = CoreFactory::make_core(mCoreModel.toStdString());
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
