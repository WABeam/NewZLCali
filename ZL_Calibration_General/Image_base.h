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
        qDebug()<<geometry().size();
        //return QRectF(QPointF(0, 0), geometry().size());
        return QRectF(QPointF(0, 0), QSizeF(300,300));
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
        m_pixmap = new QPixmap("E:\\0000.bmp");
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*,
               QWidget*) override {
        qDebug()<<"drawpix";
        const QRectF rect = boundingRect();
        if (rect.isEmpty()) {
            qDebug()<<"rect.isEmpty()";
            return;
        }
        painter->fillRect(rect, Qt::black);
        if (!(*m_pixmap)) {
            return;
        }
        const QSize size = rect.size().toSize();
        if (!m_scaled || m_scaled.size() != size) {
            m_scaled = (*m_pixmap).scaled(size, Qt::KeepAspectRatio);
        }

        const QPointF topLeft(
                    rect.x() + (rect.width() - m_scaled.width()) * 0.5,
                    rect.y() + (rect.height() - m_scaled.height()) * 0.5);
        painter->drawPixmap(topLeft, m_scaled);
    }

private:
    QPixmap *m_pixmap;
    QPixmap m_scaled;
};



#endif // IMAGE_BASE_H
