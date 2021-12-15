#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include <QDir>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QObjectList>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populateCpuFreq();
    populateGpuFreq();
    populateGovernors();

    QList<QComboBox *> mA53comboBoxes = ui->gbA53->findChildren<QComboBox *>();
    QList<QComboBox *> mA72comboBoxes = ui->gbA72->findChildren<QComboBox *>();

    foreach (QComboBox *mComboBox, mA53comboBoxes) {
        connect(mComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxChanged);
    }
    foreach (QComboBox *mComboBox, mA72comboBoxes) {
        connect(mComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxChanged);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::populateCpuFreq()
{

    _cpuA53Frequencies
        = getValueFromFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies").split(' ');
    addMhzToItems(_cpuA53Frequencies);
    QString mCurrentA53Frequency = getValueFromFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq");
    for (int i = 0; i < 4; i++) {
        QComboBox *mChild = ui->gbA53->findChild<QComboBox *>("cbCpu" + QString::number(i));
        mChild->addItems(_cpuA53Frequencies);

        QString mCpuStatus = getValueFromFile(QString("/sys/devices/system/cpu/cpu" + QString::number(i) + "/online"));

        if (mCpuStatus == "1") {
            mChild->setCurrentText(valueToMhz(mCurrentA53Frequency));
        } else {
            mChild->setCurrentIndex(0);
        }
    }

    _cpuA72Frequencies
        = getValueFromFile("/sys/devices/system/cpu/cpu4/cpufreq/scaling_available_frequencies").split(' ');
    addMhzToItems(_cpuA72Frequencies);

    QString mCurrentA72Frequency = getValueFromFile("/sys/devices/system/cpu/cpu4/cpufreq/cpuinfo_cur_freq");
    for (int i = 4; i < 6; i++) {
        QComboBox *mChild = ui->gbA72->findChild<QComboBox *>("cbCpu" + QString::number(i));
        mChild->addItems(_cpuA72Frequencies);
        QString mCpuStatus = getValueFromFile("/sys/devices/system/cpu/cpu" + QString::number(i) + "/online");
        if (mCpuStatus == "1") {
            mChild->setCurrentText(valueToMhz(mCurrentA72Frequency));
        } else {
            mChild->setCurrentIndex(0);
        }
    }
}

void MainWindow::populateGpuFreq()
{

    _gpuFrequencies
        = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/available_frequencies").split(' ');
    addMhzToItems(_gpuFrequencies, 1000000);

    const QString mGpuCurrentFreq
        = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/cur_freq");
    ui->cbGpu->addItems(_gpuFrequencies);
    ui->cbGpu->setCurrentText(valueToMhz(mGpuCurrentFreq, 1000000));
}

void MainWindow::populateGovernors()
{

    _cpuGovernors = getValueFromFile("/sys/devices/system/cpu/cpufreq/policy0/scaling_available_governors").split(' ');
    const QString mCurrentCpuGov = getValueFromFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    ui->cbCpuGov->addItems(_cpuGovernors);
    ui->cbCpuGov->setCurrentText(mCurrentCpuGov);

    _gpuGovernors
        = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/available_governors").split(' ');
    const QString mCurrentGpuGov = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/governor");
    ui->cbGpuGov->addItems(_gpuGovernors);
    ui->cbGpuGov->setCurrentText(mCurrentGpuGov);
}

void MainWindow::addMhzToItems(QStringList &iList, const int &scale)
{
    for (auto &i : iList) {
        i = valueToMhz(i, scale);
    }
}

void MainWindow::validate()
{
}

QString MainWindow::valueToMhz(const QString &iKhz, const int &scale)
{
    return QString::number(iKhz.toInt() / scale) + " Mhz";
}

QString MainWindow::mhzToValue(const QString &iMhz, const int &scale)
{
    return QString::number(iMhz.split(" ").constFirst().toInt() * scale);
}

QString MainWindow::getValueFromFile(const QString &iPath)
{
    QFile mFile(iPath, this);
    if (mFile.open(QIODevice::ReadOnly)) {
        return QString(mFile.readLine().trimmed());
    }
    return "-1";
}

void MainWindow::onComboBoxChanged(int iIndex)
{
    foreach (QComboBox *mComboBox, sender()->parent()->findChildren<QComboBox *>()) {
        if (mComboBox != sender()) {
            if (iIndex > 0 and mComboBox->currentIndex() > 0) {
                QSignalBlocker mBlocker(mComboBox);
                mComboBox->setCurrentIndex(iIndex);
                mBlocker.unblock();
            }
        }
    }
}
