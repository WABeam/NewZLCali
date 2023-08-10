#ifndef IMAGE_BASE_H
#define IMAGE_BASE_H

#include <QGraphicsObject>
#include <QGraphicsLayoutItem>
#include <QPainter>
#include <QDebug>
#include <QThread>
#include "ImageItem.h"
class ImageItemBase : public QGraphicsObject, public QGraphicsLayoutItem {
	Q_OBJECT
		Q_INTERFACES(QGraphicsLayoutItem)
		Q_PROPERTY(core::Image image READ image WRITE setImage NOTIFY imageChanged)
public:

	explicit ImageItemBase(QGraphicsItem* parent = nullptr) :
		QGraphicsObject(parent) {
		setGraphicsItem(this);
	}

	QRectF boundingRect() const override {
		qDebug() << geometry().size();
		return QRectF(QPointF(0, 0), geometry().size());
		//return QRectF(QPointF(0, 0), QSizeF(300,300));
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

	core::Image image() const noexcept {
		return m_image;
	}

protected:
	virtual bool acceptImage(const core::Image& image) = 0;

private:
	core::Image m_image;

Q_SIGNALS:
	void imageChanged(const core::Image& image);

public Q_SLOTS:
	void setImage(const core::Image& image) {
		if (acceptImage(image)) {
			m_image = image;
			Q_EMIT imageChanged(m_image);
		}
	}

};

class ImageItem :public ImageItemBase
{
	Q_OBJECT
public:
	explicit ImageItem(QGraphicsItem* parent = nullptr) :
		ImageItemBase(parent), m_thread(this),
		m_converter(new ImageConverter())
	{
		m_converter->moveToThread(&m_thread);

		connect(&m_thread, &QThread::finished, m_converter,
			&ImageConverter::deleteLater);
		connect(m_converter, &ImageConverter::qRGB32Available, this,
			&ImageItem::setPixmap);

		m_thread.start();
	}
	virtual ~ImageItem() {
		QMetaObject::invokeMethod(m_converter, &ImageConverter::stop);
		m_thread.quit();
		m_thread.wait();
	}
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem*,
		QWidget*) override {
		qDebug() << "drawpix";
		const QRectF rect = boundingRect();
		if (rect.isEmpty()) {
			qDebug() << "rect.isEmpty()";
			return;
		}
		painter->fillRect(rect, Qt::black);
		if (!m_pixmap) {
			return;
		}
		const QSize size = rect.size().toSize();
		if (!m_scaled || m_scaled.size() != size) {
			m_scaled = m_pixmap.scaled(size, Qt::KeepAspectRatio);
		}

		const QPointF topLeft(
			rect.x() + (rect.width() - m_scaled.width()) * 0.5,
			rect.y() + (rect.height() - m_scaled.height()) * 0.5);
		painter->drawPixmap(topLeft, m_scaled);
	}

protected:
	virtual bool acceptImage(const core::Image& image) override {
		if (!m_converter->isStart()) {
			QMetaObject::invokeMethod(m_converter,
				&ImageConverter::start);
		}

		return m_converter->requestConvert(image);
	}
private:

	QThread m_thread;
	ImageConverter* m_converter;

	QImage m_image;
	QPixmap m_pixmap;
	QPixmap m_scaled;

private Q_SLOTS:
	void setPixmap(const QImage& image) {
		m_image = image;
		m_pixmap = QPixmap::fromImage(image);
		m_scaled = {};

		update();
	}
};



#endif // IMAGE_BASE_H
