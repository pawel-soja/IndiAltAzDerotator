#pragma once

#include <libindi/baseclient.h>

#include <QString>
#include <QObject>

class IndiTelescope: public QObject
{
    Q_OBJECT
public:
    IndiTelescope(INDI::BaseClient *baseClient, QObject *parent = nullptr);

public:
    void setDevice(const QString &deviceName);

public:
    double latAsDegree() const;
    double lonAsDegree() const;
    double raAsDegree()  const;
    double decAsDegree() const;

public:
    double lat() const;
    double lon() const;
    double ra()  const;
    double dec() const;

signals:
    void changed();
    
private:
    INDI::BaseClient *client;
    QString deviceName;
    double mLat = 0;
    double mLon = 0;
    double mRa  = 0;
    double mDec = 0;
};
