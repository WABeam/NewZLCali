#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include <QGraphicsObject>
#include <QGraphicsLayoutItem>
#include <QPainter>
#include <QDebug>
class ImageItemBase : public QGraphicsObject, public QGraphicsLayoutItem {
    Q_OBJECT
public:
    explicit ImageItemBase(QGraphicsItem* parent = nullptr) :
        QGraphicsObject(parent) {
        setGraphicsItem(this);
    }

    QRectF boundingRect() const override {
        return QRectF(QPointF(0, 0), geometry().size());
    }

    void setGeometry(const QRectF& geom) override {
        prepareGeometryChange();
        QGraphicsLayoutItem::setGeometry(geom);
        setPos(geom.topLeft());
    }

    QSizeF sizeHint(Qt::SizeHint,
                    const QSizeF& constraint = QSizeF()) const override {
        return constraint;
    }

};

class ImageItem:public ImageItemBase
{
    Q_OBJECT
public:
    explicit ImageItem(QGraphicsItem* parent = nullptr) :
        ImageItemBase(parent)
    {

    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*,
               QWidget*) override {
        qDebug()<<"drawpix";
        painter->drawPixmap(QPointF(0, 0), m_pixmap);
    }

private:
    QPixmap m_pixmap;
};



#endif // IMAGE_BASE_H
