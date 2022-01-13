#pragma once
#include <QWidget>
#include <QFile>
#include <QSignalBlocker>
#include <QComboBox>

struct Preset {
    QString text;
    QVector<short> indexes;
};

class BaseCore : public QWidget {
public:
    BaseCore() = default;
    virtual ~BaseCore() = default;
    virtual void populateCpuFreq() = 0;
    virtual void populateGpuFreq() = 0;
    virtual void populateGovernors() = 0;
    virtual void populatePresets() = 0;
    QString getValueFromFile(const QString &iPath)
    {
        QFile mFile(iPath, this);
        if (mFile.open(QIODevice::ReadOnly)) {
            return QString(mFile.readLine().trimmed());
        }
        return "-1";
    }

    static void addMhzToItems(QStringList &iList, const int &scale = 1000)
    {
        for (auto &i : iList) {
            i = valueToMhz(i, scale);
        }
    }

    static QString valueToMhz(const QString &iKhz, const int &scale = 1000)
    {
        return QString::number(iKhz.toInt() / scale) + " Mhz";
    }
    static QString mhzToValue(const QString &iMhz, const int &scale = 1000)
    {
        return QString::number(iMhz.split(" ").constFirst().toInt() * scale);
    }

    static void setComboBoxIndex(QComboBox *ioCombo, const int &iIndex)
    {
        QSignalBlocker mBlocker(ioCombo);
        ioCombo->setCurrentIndex(iIndex);
        mBlocker.unblock();
    }

protected slots:
    void onComboBoxChanged(int index)
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
    virtual void onApplyPressed() = 0;
    virtual void onPresetChanged(int index) = 0;
};
