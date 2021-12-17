#pragma once

#include <QMainWindow>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Preset {
    QString text;
    QVector<short> indexes;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void populateCpuFreq();
    void populateGpuFreq();
    void populateGovernors();
    void populatePresets();
    static void addMhzToItems(QStringList &iList, const int &scale = 1000);

    static QString valueToMhz(const QString &iKhz, const int &scale = 1000);
    static QString mhzToValue(const QString &iMhz, const int &scale = 1000);

    QString getValueFromFile(const QString &iPath);
    static void setComboBoxIndex(QComboBox *ioCombo, const int &iIndex);
    QStringList _cpuA53Frequencies;
    QStringList _cpuA72Frequencies;
    QStringList _gpuFrequencies;
    QStringList _cpuGovernors;
    QStringList _gpuGovernors;
    QVector<Preset> _presets;
private slots:
    void onComboBoxChanged(int index);
    void onApplyPressed();
    void onPresetChanged(int index);
};
