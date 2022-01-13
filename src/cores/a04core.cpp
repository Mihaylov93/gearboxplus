#include "a04core.hpp"
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
A04Core::A04Core()
{
    _widgetLayout = new QGridLayout(this);
    this->setLayout(_widgetLayout);
    QGroupBox *gbA53 = new QGroupBox("Cortex-A53", this);
    gbA53->setObjectName("gbA53");
    _widgetLayout->addWidget(gbA53, 0, 0, 1, 2);
    // gbA53->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QGroupBox *gbGpu = new QGroupBox("Mali-T720", this);
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
}

void A04Core::populateCpuFreq()
{
}

void A04Core::populateGpuFreq()
{
}

void A04Core::populateGovernors()
{
}

void A04Core::populatePresets()
{
}

void A04Core::onApplyPressed()
{
}

void A04Core::onPresetChanged(int index)
{
}
