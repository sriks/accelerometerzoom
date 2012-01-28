#include <QDebug>
#include <QTimerEvent>
#include <QAccelerometer>
#include <QAccelerometerReading>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QTouchEvent>
#include "MoveToZoom.h"
QTM_USE_NAMESPACE

struct MoveToZoomPrivate {
    QAccelerometer* accelerometer;
    QPropertyAnimation* animator;
    QGraphicsItem* target;
    QObject* targetQObject;
    qreal z;
    qreal initialZ;
    int idleTimerId;
    bool readStarted;
};

MoveToZoom::MoveToZoom(QObject *parent) :
    QObject(parent)
{
    d = new MoveToZoomPrivate;
    d->readStarted = false;
    d->target = NULL;
    d->targetQObject = NULL;
    d->initialZ = 0;
    d->accelerometer = new QAccelerometer(this);
    d->animator = new QPropertyAnimation(this);
    d->animator->setTargetObject(this);
    d->animator->setPropertyName("zoom");
    d->animator->setDuration(5000);
    d->animator->setEasingCurve(QEasingCurve::OutQuad);
    d->z = 0;
}

MoveToZoom::~MoveToZoom() {
    removeZoom();
    delete d;
}

void MoveToZoom::setZoom(QGraphicsItem* target) {
    removeZoom();
    d->target = target;
    if(d->target->toGraphicsObject()) {
        d->targetQObject = d->target->toGraphicsObject();
    }
    else {
        QGraphicsScene* s = target->scene();
        if(s)
            d->targetQObject = s->views().at(0);
    }

    if(d->targetQObject) {
        d->targetQObject->installEventFilter(this);
        d->target->setTransformOriginPoint(d->target->boundingRect().center());
    }
    else {
        d->target = NULL;
        qWarning()<<__PRETTY_FUNCTION__<<" target does not have a graphics view. Zoom not set.";
    }
}

void MoveToZoom::removeZoom() {
    if(d->targetQObject) {
        d->targetQObject->removeEventFilter(this);
        d->readStarted = false;
    }
}

bool MoveToZoom::event(QEvent *event) {
    if(QTimerEvent::Timer == event->type() &&
      ((QTimerEvent*)event)->timerId() == d->idleTimerId) {
      d->readStarted = true;
      startSensor();
    }
}

bool MoveToZoom::eventFilter(QObject *obj, QEvent *event) {
    if(QEvent::TouchBegin == event->type() ||
       QEvent::TouchUpdate == event->type() ||
       QEvent::TouchEnd == event->type()) {
        QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
        int touchpoints = touchEvent->touchPoints().count();
        if(QEvent::TouchEnd == event->type()) {
            if(touchpoints < 2) {
            d->readStarted = false;
            killTimer(d->idleTimerId);
            d->accelerometer->stop();
            d->animator->stop();
            d->animator->setEndValue(1);
            d->animator->setDuration(500);
            d->animator->start();
            }
        }
        else {
        if(2 == touchpoints && !d->readStarted) {
            d->readStarted = true;
            d->idleTimerId = startTimer(0);
            }
        }
        event->accept();
        return true;
     }
return false;
}

void MoveToZoom::startSensor() {
    if(!d->accelerometer->isActive())
        d->accelerometer->start();
    handleSensorReading(d->accelerometer->reading());
}

void MoveToZoom::handleSensorReading(const QAccelerometerReading* sensorReading) {

    // When zoomin/zoomout animation is running, check the scaleFactor and pause the animation if
    // it is an opposite movement.

    if(0 == d->initialZ) {
       d->initialZ = sensorReading->z();
       return;
    }

    bool zoomin = false;
    if(d->z != 0) {
        qreal scaleFactor = 0;
        // device is moved towards face
        if(sensorReading->z() < d->z) {
            scaleFactor = d->z - sensorReading->z();
            zoomin = true;
        }
        // device is moved away from face
        else if(sensorReading->z() > d->z){
            scaleFactor = sensorReading->z() - d->z;
            zoomin = false;
        }

        // Consider this as no moment
        if(scaleFactor < 0.3 && QPropertyAnimation::Running == d->animator->state()) {
            d->animator->pause();
            return;
        }

        if(zoomin) {
            if(d->animator->endValue() == 3 && QPropertyAnimation::Running == d->animator->state())
                return;
            scaleFactor = 3;
        }
        else if(!zoomin) {
            if(d->animator->endValue() == 1 && QPropertyAnimation::Running == d->animator->state())
                return;
            scaleFactor = 1;
        }

        d->animator->stop();
        d->animator->setEndValue(scaleFactor);
        d->animator->start();
        }
    d->z = sensorReading->z();
}

qreal MoveToZoom::zoom() const {
    if(d->target)
        return d->target->scale();
}

void MoveToZoom::setZoom(qreal scale) {
    if(d->target)
        d->target->setScale(scale);
}

// eof
