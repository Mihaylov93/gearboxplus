#pragma once

#include <QMainWindow>
#include <QComboBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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
    void addMhzToItems(QStringList &iList, const int &scale = 1000);

    QString valueToMhz(const QString &iKhz, const int &scale = 1000);
    QString mhzToValue(const QString &iMhz, const int &scale = 1000);

    QString getValueFromFile(const QString &iPath);

    QStringList _cpuA53Frequencies;
    QStringList _cpuA72Frequencies;
    QStringList _gpuFrequencies;
    QStringList _cpuGovernors;
    QStringList _gpuGovernors;
private slots:
    void onComboBoxChanged(int iIndex);
};
