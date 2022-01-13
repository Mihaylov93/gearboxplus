#pragma once

#include "basecore.hpp"

class QGridLayout;

class A04Core : public BaseCore
{
public:
    A04Core();
    ~A04Core() = default;
    virtual void populateCpuFreq() override;
    virtual void populateGpuFreq() override;
    virtual void populateGovernors() override;
    virtual void populatePresets() override;

private:
    QGridLayout *_widgetLayout = nullptr;
    QStringList _cpuA53Frequencies;
    QStringList _gpuFrequencies;
    QStringList _cpuGovernors;
    QStringList _gpuGovernors;
    QVector<Preset> _presets;
private slots:
    void onApplyPressed() override;
    void onPresetChanged(int index) override;
};

