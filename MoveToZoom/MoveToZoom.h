/****************************************************************************
**
** Contact: Srikanth Sombhatla (srikanthsombhatla@gmail.com)
**
** This file is part of the Decii projects
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 or 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/

#ifndef MOVETOZOOM_H
#define MOVETOZOOM_H

/*! \class MoveToZoom
  \brief Zooms the supplied GraphicsItem when user moves the device towards/away from face.
         Uses accelerometer data to decide on user movement. To start zoom user should put two fingers
         on the device and start moving the device.
  \attention Currently supports only GraphicsItems only.
  \code
  \\Usage
    QGraphicsWebView* graphicsWebView = new QGraphicsWebView;
    ...
    scene->addItem(graphicsWebView)
    moveToZoom->setZoom(graphicsWebView);
  \endcode

  \code
  \\ removing zoom
    moveToZoom->removeZoom();
  \endcode
**/

#include <QObject>
#include <QAccelerometerReading>

QTM_USE_NAMESPACE
class QGraphicsItem;
class QGraphicsView;
class MoveToZoomPrivate;
class MoveToZoom : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom);
public:
    explicit MoveToZoom(QObject *parent = 0);
    ~MoveToZoom();
    qreal zoom() const;
    void setZoom(qreal scale);
public:
    bool event(QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
public slots:
    void setZoom(QGraphicsItem* target);
    void removeZoom();
private:
    void startSensor();
    void handleSensorReading(const QAccelerometerReading* sensorReading);
private:
    MoveToZoomPrivate* d;
};

#endif // MOVETOZOOM_H
