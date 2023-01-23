#include "rotatordial.h"

#include <QPainter>
#include <QPainterPath>
#include <cmath>
#include <QTimer>
RotatorDial::RotatorDial(QWidget *parent)
    : QDial(parent)
{

}

double RotatorDial::angle() const
{
    return fmod(359. * (sliderPosition() - minimum()) / (maximum() - minimum()) + 180, 360);
}

void RotatorDial::setAngle(double degree)
{
    double range = (maximum() - minimum());
    setValue(fmod(minimum() + ((degree + 180) / 359. * range), range) + minimum());
    repaint();
}

double RotatorDial::anglePreview() const
{
    return mAnglePreview;
}


void RotatorDial::setAnglePreview(double degree)
{
    mAnglePreview = degree;
    repaint();
}

void RotatorDial::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    auto radius = std::min(width(), height()) / 2 - 10;

    QPoint r1(radius - painter.fontMetrics().horizontalAdvance("000")/2 - 2, 0);
    QPoint r2 = r1 - QPoint(5, 0);
    QPoint r3 = r2 - QPoint(5, 0);
    QPoint r4 = r3 - QPoint(5, 0);

    QPoint r5 = r4 - QPoint(10, 0);

    for (int i=0; i<360; i+=10)
    {
        painter.save();
        painter.translate(rect().center());
        painter.rotate(i - 90);
        if ((i % 30) == 0)
        {
            if (i < 180)
            {
                {
                    QString text = QString::number(i);
                    QRect textSize;
                    textSize.setSize(painter.fontMetrics().size(Qt::TextSingleLine, text));
                    textSize.moveCenter(QPoint(radius, -1));
                    painter.drawText(textSize, Qt::AlignCenter, text);
                }

                {
                    QString text = QString::number(i + 180);
                    QRect textSize;
                    textSize.setSize(painter.fontMetrics().size(Qt::TextSingleLine, text));
                    textSize.moveCenter(-QPoint(radius,  1));
                    painter.drawText(textSize, Qt::AlignCenter, text);
                }

            }

            painter.drawLine(r3, r1);
        }
        else
        {
            painter.drawLine(r3, r2);
        }
        painter.restore();
    }

    painter.save();
    painter.translate(rect().center());
    painter.rotate(angle() + 90 + 180);
    {
        double sc = 0.65;
        QPainterPath path;
        path.moveTo(r5.x()*sc, -20);
        path.lineTo(r4.x(),  0);
        path.lineTo(r5.x()*sc,  20);
        painter.fillPath(path, QBrush(QColor("#966")));
        painter.setPen(QPen(QColor("#966"), 2));
        painter.drawRect(-r5.x() * sc, -r5.x(), 2*r5.x() * sc, 2*r5.x());
    }
    painter.restore();

    painter.save();
    painter.translate(rect().center());
    painter.rotate(anglePreview() + 90 + 180);
    {
        double sc = 0.65;
        QPainterPath path;
        path.moveTo(r5.x()*sc, -20);
        path.lineTo(r4.x(),  0);
        path.lineTo(r5.x()*sc,  20);
        painter.fillPath(path, QBrush(QColor("#666")));
        painter.drawRect(-r5.x() * sc, -r5.x(), 2*r5.x() * sc, 2*r5.x());
    }
    painter.restore();


    painter.save();
    painter.translate(rect().center());
    QString text = QString::number(anglePreview(), 'f', 2) + "°";
    QRect textSize;
    QFont font = painter.font();
    font.setPointSize(font.pointSize()*1.1);
    painter.setFont(font);
    textSize.setSize(painter.fontMetrics().size(Qt::TextSingleLine, text));
    textSize.moveCenter(QPoint(0, 0));
    painter.setPen(QPen(QColor("#FFF"), 10));
    painter.drawText(textSize, Qt::AlignCenter, text);
    painter.restore();
}
