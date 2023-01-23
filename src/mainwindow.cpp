#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <math.h>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix3x3>
#include <QTimer>

QMatrix3x3 rotX(float a)
{
    float matrix[] = {
        1,        0,        0,
        0, + cos(a), - sin(a),
        0, + sin(a), + cos(a)
    };
    return QMatrix3x3(matrix);
}

QMatrix3x3 rotY(float a)
{
    float matrix[] = {
        + cos(a), 0, + sin(a),
        0,        1,   0,
        - sin(a), 0, + cos(a)
    };
    return QMatrix3x3(matrix);
}

QMatrix3x3 rotZ(float a)
{
    float matrix[] = {
        + cos(a), - sin(a), 0,
        + sin(a), + cos(a), 0,
               0,        0, 1
    };
    return QMatrix3x3(matrix);
}

QVector3D operator*(const QMatrix3x3 &m, const QVector3D &v)
{
    return QVector3D(
        m(0, 0) * v.x() + m(0, 1) * v.y() + m(0, 2) * v.z(),
        m(1, 0) * v.x() + m(1, 1) * v.y() + m(1, 2) * v.z(),
        m(2, 0) * v.x() + m(2, 1) * v.y() + m(2, 2) * v.z()
    );
}

class MountCoordinate
{
public:
    MountCoordinate(double D, double H, double L)
        : D(D)
        , H(H)
        , L(L)
    { }

public:
    QVector3D xyz() const
    {
        return rotY(L) * rotZ(H) * rotY(-D) * QVector3D(1, 0, 0);
    }

    QVector2D altAz() const
    {
        QVector3D xyz = this->xyz();
        return QVector2D(
            asin(xyz.x()),
            atan2(xyz.y(), xyz.z())
        );
    }

    QVector3D versorSpace() const
    {
        QVector3D xyz = this->xyz();
        QVector3D earthRotation = rotY(L) * QVector3D(0, 0, 1);
        return QVector3D::crossProduct(xyz, earthRotation);
    }

    QVector3D versor() const
    {
        QVector2D altAz = this->altAz();
        QVector3D versorSpace = this->versorSpace();
        return rotY(-altAz.x()) * rotX(altAz.y()) * versorSpace;
    }

    double rotationAngle() const
    {
        QVector3D versor = this->versor();
        return -atan2(versor.x(), versor.y());
    }

private:
    double D, H, L;
};

static QString degreesAsString(double value)
{
    return QString::number(value * 180 / M_PI, 'f', 3);
}

#include <chrono>

struct jdate_clock
{
    using rep        = double;
    using period     = std::ratio<86400>;
    using duration   = std::chrono::duration<rep, period>;
    using time_point = std::chrono::time_point<jdate_clock>;

    static constexpr bool is_steady = false;

    static double now() noexcept
    {
        using namespace std::chrono;
        using namespace std::chrono_literals;
        return (time_point{duration{system_clock::now().time_since_epoch()} + 58574100h}).time_since_epoch().count();
    }
};

double earthRotationAngle(double jd)
{
    const double t = jd - 2451545.0;
    return 2 * M_PI * (fmod(jd, 1.0) + 0.7790572732640 + 0.00273781191135448 * t); //eq 14
}

double greenwichMeanSiderealTime(double jd)
{
    const double t = (jd - 2451545.0) / 36525.0;
    return fmod(earthRotationAngle(jd) + (
                0.014506 +
                4612.156534 * t +
                1.3915817 * t * t -
                0.00000044 * t * t * t -
                0.000029956 * t * t * t * t -
                0.0000000368 * t * t * t * t * t
            ) / 60.0 / 60.0 * M_PI / 180.0, 2*M_PI);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->rotator->setAngle(0);

    setWindowFlags(Qt::WindowStaysOnTopHint);

    auto process = [this]()
    {
        double offset[] = {-90, -45, 0, 45, 90};
        const double lat   = telescope.lat();
        const double lon   = telescope.lon();
        const double ra    = telescope.ra() - greenwichMeanSiderealTime(jdate_clock::now()) - lon;
        const double dec   = telescope.dec();

        const MountCoordinate mc(dec, ra, lat);

        double angle = mc.rotationAngle()
                * (ui->rotationInverse->isChecked() ? -1 : 1)
                + offset[ui->rotationOffset->value()] * M_PI / 180;

        // Telescope info
        QVector2D altAz = mc.altAz();
        ui->altitude->setText(degreesAsString(altAz.x()));
        ui->azimuth->setText(degreesAsString(altAz.y()));

        ui->longitude->setText(degreesAsString(lon));
        ui->latitude->setText(degreesAsString(lat));
        ui->rightAscension->setText(degreesAsString(ra));
        ui->declination->setText(degreesAsString(dec));

        // Rotation
        ui->rotation->setText(degreesAsString(angle));

        if (ui->rotatorEnable->isChecked())
        {
            ui->rotator->setAngle(angle * 180 / M_PI);
            rotator.setAngle(angle * 180 / M_PI);
        }
    };

    ([this, process](QTimer *timer)
    {
        connect(timer, &QTimer::timeout, this, process);
        timer->start(500);
    })(new QTimer(this));

    //connect(&telescope, &IndiTelescope::changed, this, process);

    // Update UI rotator angle
    connect(&rotator, &IndiRotator::angleChanged, this, [this]
    {
        ui->rotator->setAnglePreview(rotator.angle());
    });

    // Manual change rotator angle
    connect(ui->rotator, &RotatorDial::valueChanged, this, [this](int)
    {
        if (!ui->rotatorEnable->isChecked())
        {
            rotator.setAngle(ui->rotator->angle());
        }
    });

    // Select telescope
    connect(ui->telescopeName, &QComboBox::currentTextChanged, this, [this] (const QString &deviceName)
    {
        telescope.setDevice(deviceName);
        settings.setValue("telescope", deviceName);
    });

    // Select rotator
    connect(ui->rotatorName, &QComboBox::currentTextChanged, this, [this] (const QString &deviceName)
    {
        rotator.setDevice(deviceName);
        settings.setValue("rotator", deviceName);
    });

    // Discover telescope and rotator
    connect(&client, &IndiClient::onNewDevice, this, [this](INDI::BaseDevice baseDevice)
    {
        QString deviceName = baseDevice.getDeviceName();
        ui->telescopeName->addItem(deviceName);
        ui->rotatorName->addItem(deviceName);

        if (deviceName == settings.value("telescope"))
        {
            ui->telescopeName->setCurrentText(deviceName);
        }

        if (deviceName == settings.value("rotator"))
        {
            ui->rotatorName->setCurrentText(deviceName);
        }
    });

    qDebug() << "Connected:" << client.connectServer();
    client.setVerbose(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

