#include "indiclient.h"

Q_DECLARE_METATYPE(INDI::BaseDevice);

IndiClient::IndiClient(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<INDI::BaseDevice>();
}

void IndiClient::newDevice(INDI::BaseDevice baseDevice)
{
    emit onNewDevice(baseDevice);
}
