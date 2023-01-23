#pragma once

#include <libindi/baseclient.h>
#include <libindi/basedevice.h>

#include <QObject>

class IndiClient : public QObject, public INDI::BaseClient
{
    Q_OBJECT
public:
    explicit IndiClient(QObject *parent = nullptr);

protected:
    void newDevice(INDI::BaseDevice baseDevice);

signals:
    void onNewDevice(INDI::BaseDevice baseDevice);
};
