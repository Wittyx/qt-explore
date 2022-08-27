#include "SysInfoWidget.h"

#include <QVBoxLayout>

SysInfoWidget::SysInfoWidget(QWidget *parent,
                             int startDelayMs,
                             int updateSeriesDelayMs) :
    QWidget(parent),
    mDisplayer(this)
{
    mRefreshTimer.setInterval(updateSeriesDelayMs);
    connect(&mRefreshTimer, &QTimer::timeout,
            this, &SysInfoWidget::updateSeries);
    QTimer::singleShot(startDelayMs, [this] { mRefreshTimer.start(); });

    mDisplayer.setDigitCount(5);
    mDisplayer.setSmallDecimalPoint(true);
    mDisplayer.setSegmentStyle(QLCDNumber::Flat);

    mDisplayer.setStyleSheet("background: black; color: #00FF00; border: none;");
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(&mDisplayer);
    setLayout(layout);

}
