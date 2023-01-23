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
    //QDial::paintEvent(p);

    QPainter painter(this);
    auto textWidth  = painter.fontMetrics().horizontalAdvance("000");
    //auto textHeight = fontMetrics().;
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    auto radius = std::min(width(), height()) / 2 - 10;


    //qDebug() << textHeight;

    QPoint r1(radius - textWidth/2 - 2, 0);
    QPoint r2 = r1 - QPoint(5, 0);
    QPoint r3 = r2 - QPoint(5, 0);
    QPoint r4 = r3 - QPoint(5, 0);

    QPoint r5 = r4 - QPoint(10, 0);
    QPoint r6 = r5 - QPoint(10, 0);

    QPoint r7 = r5 - QPoint(10, 0);


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
    painter.setPen(QPen(Qt::red, 3));
    painter.drawLine(r4, r1);
    painter.restore();

#if 0
    painter.save();
    painter.translate(rect().center());
    painter.rotate(angle + 90 + 180);
    {
        QPainterPath path;
        path.lineTo(r4.x(), 0);
        path.arcTo    (-r5.x(), -r5.x(), 2*r5.x(), 2*r5.x(), 10, 340);
        path.lineTo(r4.x(), 0);
        painter.fillPath(path, QBrush(QColor("#666")));
    }
    painter.restore();
#endif
#if 1
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
#endif

#if 0
    painter.save();
    painter.translate(rect().center());
    {
        QPainterPath path;
        path.arcTo    (-r6.x(), -r6.x(), 2*r6.x(), 2*r6.x(), 0, 360);
        painter.fillPath(path, QBrush(QColor("#AAA")));
    }
    painter.restore();
#endif

    painter.save();
    painter.translate(rect().center());
    QString text = QString::number(anglePreview(), 'f', 2) + "°";
    QRect textSize;
    QFont font = painter.font();
    font.setPointSize(font.pointSize()*1.2);
    painter.setFont(font);
    textSize.setSize(painter.fontMetrics().size(Qt::TextSingleLine, text));
    textSize.moveCenter(QPoint(0, 0));
    painter.setPen(QPen(QColor("#FFF"), 10));
    painter.drawText(textSize, Qt::AlignCenter, text);
    painter.restore();

#if 0
    auto ref = QLineF::fromPolar(150, 0).translated(this->rect().center());
    qDebug() << ref;
    for (int i=0; i<360; i+=10)
    {
        ref.setAngle(i);

        QRect textRect;
        QString text = QString::number(i);

        textRect.setSize(fontMetrics().size(Qt::TextSingleLine, text));
        textRect.moveCenter(ref.p2().toPoint());
                    //qp.drawText(textRect, QtCore.Qt.AlignCenter, str(value))

        painter.drawText(textRect, Qt::AlignCenter, text);
    }
#endif

}
