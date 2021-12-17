#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QObjectList>
#include <QString>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populateCpuFreq();
    populateGpuFreq();
    populateGovernors();
    populatePresets();
    QList<QComboBox *> mA53comboBoxes = ui->gbA53->findChildren<QComboBox *>();
    QList<QComboBox *> mA72comboBoxes = ui->gbA72->findChildren<QComboBox *>();

    foreach (QComboBox *mComboBox, mA53comboBoxes) {
        connect(mComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxChanged);
    }
    foreach (QComboBox *mComboBox, mA72comboBoxes) {
        connect(mComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxChanged);
    }

    connect(ui->cbPreset, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onPresetChanged);
    connect(ui->pbApply, &QPushButton::released, this, &MainWindow::onApplyPressed);
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
        auto *mChild = ui->gbA53->findChild<QComboBox *>("cbCpu" + QString::number(i));
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
        auto *mChild = ui->gbA72->findChild<QComboBox *>("cbCpu" + QString::number(i));
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

    const QString mGpuMaxFreq = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/max_freq");
    ui->cbGpu->addItems(_gpuFrequencies);
    ui->cbGpu->setCurrentText(valueToMhz(mGpuMaxFreq, 1000000));
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

void MainWindow::populatePresets()
{
    // TODO(Petrov): If not installed as deb package check the root of the executable for this file.
    QFile file("/usr/share/gearboxplus/presets.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    file.readLine();    // Skip header
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> mLine = line.split(',');
        Preset mPreset
            = {mLine[0],
               {mLine[1].toShort(), mLine[2].toShort(), mLine[3].toShort(), mLine[4].toShort(), mLine[5].toShort(),
                mLine[6].toShort(), mLine[7].toShort(), mLine[8].toShort(), mLine[9].toShort()}};
        ui->cbPreset->addItem(mLine[0]);
        _presets.append(mPreset);
    }
}

void MainWindow::addMhzToItems(QStringList &iList, const int &scale)
{
    for (auto &i : iList) {
        i = valueToMhz(i, scale);
    }
}

auto MainWindow::valueToMhz(const QString &iKhz, const int &scale) -> QString
{
    return QString::number(iKhz.toInt() / scale) + " Mhz";
}

auto MainWindow::mhzToValue(const QString &iMhz, const int &scale) -> QString
{
    return QString::number(iMhz.split(" ").constFirst().toInt() * scale);
}

auto MainWindow::getValueFromFile(const QString &iPath) -> QString
{
    QFile mFile(iPath, this);
    if (mFile.open(QIODevice::ReadOnly)) {
        return QString(mFile.readLine().trimmed());
    }
    return "-1";
}

void MainWindow::setComboBoxIndex(QComboBox *ioCombo, const int &iIndex)
{
    QSignalBlocker mBlocker(ioCombo);
    ioCombo->setCurrentIndex(iIndex);
    mBlocker.unblock();
}

void MainWindow::onComboBoxChanged(int index)
{
    foreach (QComboBox *mComboBox, sender()->parent()->findChildren<QComboBox *>()) {
        if (mComboBox != sender()) {
            if (index > 0 and mComboBox->currentIndex() > 0) {
                QSignalBlocker mBlocker(mComboBox);
                mComboBox->setCurrentIndex(index);
                mBlocker.unblock();
            }
        }
    }
}

void MainWindow::onApplyPressed()
{
    bool mA53Enabled = false;
    bool mA72Enabled = false;

    QList<QComboBox *> mCpuComboBoxes = ui->gbA53->findChildren<QComboBox *>() + ui->gbA72->findChildren<QComboBox *>();
    foreach (QComboBox *mComboBox, mCpuComboBoxes) {
        const QString mCore = mComboBox->objectName().right(1);
        if (mComboBox->currentText() == "Off") {
            system(QString("echo 0 | sudo tee /sys/devices/system/cpu/cpu" + mCore + "/online").toStdString().c_str());
        } else {
            system(QString("echo 1 | sudo tee /sys/devices/system/cpu/cpu" + mCore + "/online").toStdString().c_str());
            system(QString("echo " + mhzToValue(mComboBox->currentText()) + " | sudo tee /sys/devices/system/cpu/cpu"
                           + mCore + "/cpufreq/scaling_max_freq")
                       .toStdString()
                       .c_str());
            if (mCore.toInt() >= 0 and mCore.toInt() < 5) {
                mA53Enabled = true;
            } else {
                mA72Enabled = true;
            }
        }
    }

    QString mGpuFreq = mhzToValue(ui->cbGpu->currentText(), 1000000);
    system(QString("echo " + mGpuFreq + " | sudo tee /sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/max_freq")
               .toStdString()
               .c_str());
    if (mA53Enabled) {
        system(QString("echo " + ui->cbCpuGov->currentText()
                       + " | sudo tee /sys/devices/system/cpu/cpufreq/policy0/scaling_governor")
                   .toStdString()
                   .c_str());
    }
    if (mA72Enabled) {
        system(QString("echo " + ui->cbCpuGov->currentText()
                       + " | sudo tee /sys/devices/system/cpu/cpufreq/policy4/scaling_governor")
                   .toStdString()
                   .c_str());
    }

    system(QString("echo " + ui->cbCpuGov->currentText()
                   + " | sudo tee /sys/devices/system/cpu/cpufreq/policy0/scaling_governor")
               .toStdString()
               .c_str());

    system(QString("echo " + ui->cbGpuGov->currentText()
                   + " | sudo tee /sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/governor")
               .toStdString()
               .c_str());
}

void MainWindow::onPresetChanged(int index)
{
    if (index > 0) {
        Preset mPreset = _presets[index - 1];
        setComboBoxIndex(ui->cbCpu0, mPreset.indexes[0]);
        setComboBoxIndex(ui->cbCpu1, mPreset.indexes[1]);
        setComboBoxIndex(ui->cbCpu2, mPreset.indexes[2]);
        setComboBoxIndex(ui->cbCpu3, mPreset.indexes[3]);
        setComboBoxIndex(ui->cbCpu4, mPreset.indexes[4]);
        setComboBoxIndex(ui->cbCpu5, mPreset.indexes[5]);
        setComboBoxIndex(ui->cbGpu, mPreset.indexes[6]);
        setComboBoxIndex(ui->cbCpuGov, mPreset.indexes[7]);
        setComboBoxIndex(ui->cbGpuGov, mPreset.indexes[8]);
    }
}
