#ifndef ROTATORDIAL_H
#define ROTATORDIAL_H

#include <QDial>
#include <QWidget>

class RotatorDial : public QDial
{
    Q_OBJECT
public:
    RotatorDial(QWidget *parent = nullptr);

public:
    double angle() const;
    void setAngle(double degree);

    double anglePreview() const;
    void setAnglePreview(double degree);


protected:
    void paintEvent(QPaintEvent *);

private:
    double mAnglePreview {0};
};

#endif // ROTATORDIAL_H
