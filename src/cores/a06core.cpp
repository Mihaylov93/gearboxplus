#include "a06core.hpp"
#include <QComboBox>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

A06Core::A06Core()
{
    QFile file("/usr/share/gearboxplus/presets.csv");
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << file.errorString();
    }
    if (file.readAll().size() == 0) {
        QFile presetFile(":/a06presets.csv");
        presetFile.open(QFile::ReadOnly);
        QByteArray content = presetFile.readAll();
        file.write(content);
        presetFile.close();
    }
    file.close();

    _widgetLayout = new QGridLayout(this);
    this->setLayout(_widgetLayout);
    QGroupBox *gbA53 = new QGroupBox("Cortex-A53", this);
    gbA53->setObjectName("gbA53");
    _widgetLayout->addWidget(gbA53, 0, 0);
    QGroupBox *gbA72 = new QGroupBox("Cortex-A72", this);
    _widgetLayout->addWidget(gbA72, 0, 1);
    QGroupBox *gbGpu = new QGroupBox("Mali-T860", this);
    _widgetLayout->addWidget(gbGpu, 0, 2);
    QGroupBox *gbGov = new QGroupBox("Governor", this);
    _widgetLayout->addWidget(gbGov, 1, 0);
    QGroupBox *gbPreset = new QGroupBox("Preset", this);
    _widgetLayout->addWidget(gbPreset, 1, 1);

    QGridLayout *mA53layout = new QGridLayout(gbA53);
    gbA53->setLayout(mA53layout);
    QComboBox *cbCpu0 = new QComboBox(gbA53);
    cbCpu0->setObjectName("cbCpu0");
    cbCpu0->addItem("Off");
    QComboBox *cbCpu1 = new QComboBox(gbA53);
    cbCpu1->setObjectName("cbCpu1");
    cbCpu1->addItem("Off");
    QComboBox *cbCpu2 = new QComboBox(gbA53);
    cbCpu2->setObjectName("cbCpu2");
    cbCpu2->addItem("Off");
    QComboBox *cbCpu3 = new QComboBox(gbA53);
    cbCpu3->setObjectName("cbCpu3");
    cbCpu3->addItem("Off");

    mA53layout->addWidget(new QLabel("CPU 0"), 0, 0);
    mA53layout->addWidget(new QLabel("CPU 1"), 0, 1);
    mA53layout->addWidget(new QLabel("CPU 2"), 0, 2);
    mA53layout->addWidget(new QLabel("CPU 3"), 0, 3);

    mA53layout->addWidget(cbCpu0, 1, 0);
    mA53layout->addWidget(cbCpu1, 1, 1);
    mA53layout->addWidget(cbCpu2, 1, 2);
    mA53layout->addWidget(cbCpu3, 1, 3);

    QGridLayout *mA72layout = new QGridLayout(gbA72);
    gbA53->setLayout(mA53layout);
    QComboBox *cbCpu4 = new QComboBox(gbA72);
    cbCpu4->setObjectName("cbCpu4");
    cbCpu4->addItem("Off");
    QComboBox *cbCpu5 = new QComboBox(gbA72);
    cbCpu5->setObjectName("cbCpu5");
    cbCpu5->addItem("Off");

    mA72layout->addWidget(new QLabel("CPU 4"), 0, 0);
    mA72layout->addWidget(new QLabel("CPU 5"), 0, 1);

    mA72layout->addWidget(cbCpu4, 1, 0);
    mA72layout->addWidget(cbCpu5, 1, 1);

    gbGpu->setLayout(new QGridLayout(gbGpu));
    gbGpu->layout()->addWidget(new QLabel("GPU"));
    QComboBox *cbGpu = new QComboBox(gbGpu);
    cbGpu->setObjectName("cbGpu");
    gbGpu->layout()->addWidget(cbGpu);

    QGridLayout *mGovLayout = new QGridLayout(gbGov);
    gbGov->setLayout(mGovLayout);
    mGovLayout->addWidget(new QLabel("CPU"), 0, 0);
    mGovLayout->addWidget(new QLabel("GPU"), 0, 1);

    QComboBox *cbCpuGov = new QComboBox(gbGov);
    cbCpuGov->setObjectName("cbCpuGov");
    QComboBox *cbGpuGov = new QComboBox(gbGov);
    cbGpuGov->setObjectName("cbGpuGov");
    mGovLayout->addWidget(cbCpuGov, 1, 0);
    mGovLayout->addWidget(cbGpuGov, 1, 1);

    QGridLayout *mPresetLayout = new QGridLayout(gbPreset);
    gbPreset->setLayout(mPresetLayout);
    mPresetLayout->addWidget(new QLabel(""), 0, 0);
    QComboBox *cbPreset = new QComboBox(gbPreset);
    cbPreset->addItem("Custom");
    cbPreset->setObjectName("cbPreset");
    mPresetLayout->addWidget(cbPreset, 1, 0);

    QPushButton *pbApply = new QPushButton("Apply", this);
    QWidget *wdgApply = new QWidget(this);
    wdgApply->setLayout(new QGridLayout());
    wdgApply->layout()->addWidget(new QLabel("", this));
    wdgApply->layout()->addWidget(pbApply);
    _widgetLayout->addWidget(wdgApply, 1, 2);

    QList<QComboBox *> mA53comboBoxes = gbA53->findChildren<QComboBox *>();
    QList<QComboBox *> mA72comboBoxes = gbA72->findChildren<QComboBox *>();

    foreach (QComboBox *mComboBox, mA53comboBoxes) {
        connect(mComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &A06Core::onComboBoxChanged);
    }
    foreach (QComboBox *mComboBox, mA72comboBoxes) {
        connect(mComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &A06Core::onComboBoxChanged);
    }

    connect(cbPreset, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &A06Core::onPresetChanged);
    connect(pbApply, &QPushButton::released, this, &A06Core::onApplyPressed);
}

A06Core::~A06Core() = default;

void A06Core::populateCpuFreq()
{

    _cpuA53Frequencies
        = getValueFromFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies").split(' ');
    addMhzToItems(_cpuA53Frequencies);
    QString mCurrentA53Frequency = getValueFromFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq");
    for (int i = 0; i < 4; i++) {
        auto *mChild = this->findChild<QComboBox *>("cbCpu" + QString::number(i));
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
        auto *mChild = this->findChild<QComboBox *>("cbCpu" + QString::number(i));
        mChild->addItems(_cpuA72Frequencies);
        QString mCpuStatus = getValueFromFile("/sys/devices/system/cpu/cpu" + QString::number(i) + "/online");
        if (mCpuStatus == "1") {
            mChild->setCurrentText(valueToMhz(mCurrentA72Frequency));
        } else {
            mChild->setCurrentIndex(0);
        }
    }
}

void A06Core::populateGpuFreq()
{

    _gpuFrequencies
        = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/available_frequencies").split(' ');
    addMhzToItems(_gpuFrequencies, 1000000);

    const QString mGpuMaxFreq = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/max_freq");
    QComboBox *cbGpu = this->findChild<QComboBox *>("cbGpu");
    cbGpu->addItems(_gpuFrequencies);
    cbGpu->setCurrentText(valueToMhz(mGpuMaxFreq, 1000000));
}

void A06Core::populateGovernors()
{
    _cpuGovernors = getValueFromFile("/sys/devices/system/cpu/cpufreq/policy0/scaling_available_governors").split(' ');
    const QString mCurrentCpuGov = getValueFromFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    QComboBox *cbCpuGov = this->findChild<QComboBox *>("cbCpuGov");
    cbCpuGov->addItems(_cpuGovernors);
    cbCpuGov->setCurrentText(mCurrentCpuGov);

    _gpuGovernors
        = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/available_governors").split(' ');
    const QString mCurrentGpuGov = getValueFromFile("/sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/governor");
    QComboBox *cbGpuGov = this->findChild<QComboBox *>("cbGpuGov");
    cbGpuGov->addItems(_gpuGovernors);
    cbGpuGov->setCurrentText(mCurrentGpuGov);
}

void A06Core::populatePresets()
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
        this->findChild<QComboBox *>("cbPreset")->addItem(mLine[0]);
        _presets.append(mPreset);
    }
}

void A06Core::onApplyPressed()
{
    bool mA53Enabled = false;
    bool mA72Enabled = false;

    QGroupBox *gbA53 = this->findChild<QGroupBox *>("gbA53");
    QGroupBox *gbA72 = this->findChild<QGroupBox *>("gbA72");
    QList<QComboBox *> mCpuComboBoxes = gbA53->findChildren<QComboBox *>() + gbA72->findChildren<QComboBox *>();
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

    QComboBox *cbGpu = this->findChild<QComboBox *>("cbGpu");
    QString mGpuFreq = mhzToValue(cbGpu->currentText(), 1000000);
    system(QString("echo " + mGpuFreq + " | sudo tee /sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/max_freq")
               .toStdString()
               .c_str());
    if (mA53Enabled) {
        system(QString("echo " + this->findChild<QComboBox *>("cbCpuGov")->currentText()
                       + " | sudo tee /sys/devices/system/cpu/cpufreq/policy0/scaling_governor")
                   .toStdString()
                   .c_str());
    }
    if (mA72Enabled) {
        system(QString("echo " + this->findChild<QComboBox *>("cbCpuGov")->currentText()
                       + " | sudo tee /sys/devices/system/cpu/cpufreq/policy4/scaling_governor")
                   .toStdString()
                   .c_str());
    }

    system(QString("echo " + this->findChild<QComboBox *>("cbGpuGov")->currentText()
                   + " | sudo tee /sys/devices/platform/ff9a0000.gpu/devfreq/ff9a0000.gpu/governor")
               .toStdString()
               .c_str());
}

void A06Core::onPresetChanged(int index)
{
    if (index > 0) {
        Preset mPreset = _presets[index - 1];
        setComboBoxIndex(this->findChild<QComboBox *>("cbCpu0"), mPreset.indexes[0]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbCpu1"), mPreset.indexes[1]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbCpu2"), mPreset.indexes[2]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbCpu3"), mPreset.indexes[3]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbCpu4"), mPreset.indexes[4]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbCpu5"), mPreset.indexes[5]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbGpu"), mPreset.indexes[6]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbCpuGov"), mPreset.indexes[7]);
        setComboBoxIndex(this->findChild<QComboBox *>("cbGpuGov"), mPreset.indexes[8]);
    }
}
