#pragma once

#include <libindi/baseclient.h>
#include <QObject>

class IndiRotator : public QObject
{
    Q_OBJECT
public:
    explicit IndiRotator(INDI::BaseClient *baseClient, QObject *parent = nullptr);

public:
    void setDevice(const QString &deviceName);

public:
    double angle() const;

    bool setAngle(double degree);

    bool setCurrentAngle(double degree);

signals:
    void angleChanged();

private:
    INDI::BaseClient *client;
    QString deviceName;
};
