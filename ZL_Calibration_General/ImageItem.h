#ifndef IMAGEITEM_H
#define IMAGEITEM_H
#include <QObject>
#include <QTimer>
#include <mutex>
#include <QImage>
#include <image.h>
class ImageConverter : public QObject {
    Q_OBJECT

public:
    ImageConverter() : m_timer(this) {
        m_timer.setTimerType(Qt::PreciseTimer);
        m_timer.setInterval(1000 / 60);
        m_timer.callOnTimeout(this, &ImageConverter::convert);
    }

    ~ImageConverter() {
        stop();
    }

    bool isStart() const {
        return m_timer.isActive();
    }

private:
    QTimer m_timer;
    core::Image m_image;
    std::mutex m_mutex;

Q_SIGNALS:
    void qRGB32Available(const QImage& image);

public Q_SLOTS:
    bool requestConvert(const core::Image& image) {
        if (image.isNull()) {
            return false;
        }

        std::lock_guard lock(m_mutex);
        if (m_image.isNull()) {
            m_image = image;
            return true;
        }
        return false;
    }

    void start() {
        if (!isStart()) {
            m_timer.start();
        }
    }

    void stop() {
        if (isStart()) {
            m_timer.stop();
        }
    }

private Q_SLOTS:
    void convert() {
        std::unique_lock lock(m_mutex);
        core::Image image = std::move(m_image);
        lock.unlock();

        QImage qimg = image.makePaintable();

        if (!qimg.isNull()) {
            Q_EMIT qRGB32Available(qimg);
        }
    }
};
#endif // IMAGEITEM_H
