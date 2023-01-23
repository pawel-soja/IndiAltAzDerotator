#include "indirotator.h"
#include <libindi/basedevice.h>

IndiRotator::IndiRotator(INDI::BaseClient *baseClient, QObject *parent)
    : QObject(parent)
    , client(baseClient)
{ }

void IndiRotator::setDevice(const QString &name)
{
    deviceName = name;

    auto newDevice = [this](INDI::BaseDevice device)
    {
        if (device.getDeviceName() != deviceName)
            return;

        auto connection = [this](INDI::PropertySwitch prop)
        {
            if (prop[0].getState() == ISS_OFF)
                client->connectDevice(deviceName.toLatin1().data());
        };

        auto pollingPeriod = [this](INDI::PropertyNumber prop)
        {
            if (!prop.isValid() || prop.getDeviceName() != deviceName)
                return;

            prop[0].setValue(100);
            client->newProperty(prop);
        };

        auto absRotatorAngle = [this](INDI::PropertyNumber property)
        {
            property.onUpdate([this, property]
            {
                if (property.getDeviceName() != deviceName)
                    return;
                emit angleChanged();
            });
            property.emitUpdate();
        };

        device.watchProperty("CONNECTION", connection);
        connection(device.getProperty("CONNECTION"));

        device.watchProperty("POLLING_PERIOD", pollingPeriod);
        pollingPeriod(device.getProperty("POLLING_PERIOD"));

        device.watchProperty("ABS_ROTATOR_ANGLE", absRotatorAngle);
        absRotatorAngle(device.getProperty("ABS_ROTATOR_ANGLE"));
    };

    client->watchDevice(deviceName.toLatin1().data(), newDevice);
    newDevice(client->getDevice(deviceName.toLatin1().data()));
}

double IndiRotator::angle() const
{
    INDI::BaseDevice device = client->getDevice(deviceName.toLatin1().data());
    INDI::PropertyNumber property = device.getProperty("ABS_ROTATOR_ANGLE");
    return property[0].getValue();
}

bool IndiRotator::setAngle(double degree)
{
    INDI::BaseDevice device = client->getDevice(deviceName.toLatin1().data());
    INDI::PropertyNumber property = device.getProperty("ABS_ROTATOR_ANGLE");
    auto old = property[0].getValue();
    property[0].setValue(degree);
    client->sendNewProperty(property);
    property[0].setValue(old);
    return true;
}

bool IndiRotator::setCurrentAngle(double degree)
{
    return false;
    INDI::PropertyNumber property = client->getDevice(deviceName.toLatin1().data()).getProperty("SYNC_ROTATOR_ANGLE");
    if (!property.isValid())
        return false;

    property[0].setValue(degree);
    client->sendNewProperty(property);

    return true;
}
