#ifndef CAMERAOUTPUT_H
#define CAMERAOUTPUT_H
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QFontMetricsF>
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include <QLabel>
#include "Image_base.h"

class CameraOutput : public QGraphicsWidget {
    Q_OBJECT
public:
    explicit CameraOutput(QGraphicsItem* parent = nullptr) :
        QGraphicsWidget(parent),
        m_layout(new QGraphicsLinearLayout(this))
    {
        qDebug()<<"CameraOutput";
//        QLabel *label = new QLabel("CH 1");
//        m_layout->addItem(label);
        ImageItemBase *imageItem = new ImageItem(this);
        m_layout->addItem(imageItem);

        setLayout(m_layout);//设置布局

    }
private:
    QGraphicsLinearLayout* m_layout;

};


#endif // CAMERAOUTPUT_H
