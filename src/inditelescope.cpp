#include "inditelescope.h"
#include <libindi/basedevice.h>
#include <cmath>

IndiTelescope::IndiTelescope(INDI::BaseClient *baseClient, QObject *parent)
    : QObject(parent)
    , client(baseClient)
{ }

void IndiTelescope::setDevice(const QString &name)
{
    deviceName = name;

    auto newDevice = [this](INDI::BaseDevice device)
    {
        if (device.getDeviceName() != deviceName)
            return;
        auto geographicCoord = [this, device](INDI::PropertyNumber prop)
        {
            prop.onUpdate([this, prop]
            {
                if (prop.getDeviceName() != deviceName)
                    return;
                mLat = prop[0].getValue();
                mLon = prop[1].getValue();
                emit changed();
            });
            prop.emitUpdate();
        };

        auto equatorialEodCoord = [this, device](INDI::PropertyNumber prop)
        {
            prop.onUpdate([this, prop]
            {
                if (prop.getDeviceName() != deviceName)
                    return;
                mRa  = prop[0].getValue()*15;
                mDec = prop[1].getValue();
                emit changed();
            });
            prop.emitUpdate();
        };

        device.watchProperty("GEOGRAPHIC_COORD", geographicCoord);
        device.watchProperty("EQUATORIAL_EOD_COORD", equatorialEodCoord);

        geographicCoord(device.getProperty("GEOGRAPHIC_COORD"));
        equatorialEodCoord(device.getProperty("EQUATORIAL_EOD_COORD"));
    };

    newDevice(client->getDevice(deviceName.toLatin1().data()));

    client->watchDevice(deviceName.toLatin1().data(), newDevice);
}

double IndiTelescope::latAsDegree() const { return mLat; }

double IndiTelescope::lonAsDegree() const { return mLon; }

double IndiTelescope::raAsDegree() const { return mRa;  }

double IndiTelescope::decAsDegree() const { return mDec; }

double IndiTelescope::lat() const { return mLat * M_PI / 180; }

double IndiTelescope::lon() const { return mLon * M_PI / 180; }

double IndiTelescope::ra() const { return mRa * M_PI / 180;  }

double IndiTelescope::dec() const { return mDec * M_PI / 180; }
