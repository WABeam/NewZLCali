#pragma once

#include "image_conversion.hpp"
#include "image_private.hpp"

#include "image.h"
#include <QtCore/QFile>

namespace core {
Image::Image() noexcept : m_p(nullptr) {}

Image::Image(int width, int height, Format format) :
        Image(createImagePrivate(width, height, format)) {}

Image::Image(uchar* data, int width, int height, Format format,
             int bytesPerLine, CleanupFunction cleanup) :
        Image(createImagePrivate(data, width, height, format, bytesPerLine,
                                 cleanup)) {}

Image::Image(const QSize& size, Format format) :
        Image(size.width(), size.height(), format) {}

Image::Image(uchar* data, const QSize& size, Format format, int bytesPerLine,
             CleanupFunction cleanup) :
        Image(data, size.width(), size.height(), format, bytesPerLine,
              cleanup) {}

Image::Image(const QImage& image) : Image(createImagePrivate(image)) {}

Image::Image(const QString& fileName, const char* format) : Image(QImage(fileName, format)) {}


Image::Image(ImagePrivate* p) noexcept : m_p(p) {
    if (m_p) {
        m_p->ref();
    }
}

Image::~Image() {
    if (m_p) {
        m_p->dref();
    }
}

Image::Image(const Image& other) noexcept : m_p(other.m_p) {
    if (m_p) {
        m_p->ref();
    }
}

Image::Image(Image&& other) noexcept : m_p(std::exchange(other.m_p, nullptr)) {}

bool Image::isNull() const noexcept {
    return !m_p;
}

uchar* Image::bits() noexcept {
    return m_p ? m_p->bits() : nullptr;
}

const uchar* Image::bits() const noexcept {
    return m_p ? m_p->bits() : nullptr;
}

qsizetype Image::sizeInBytes() const noexcept {
    return m_p ? m_p->sizeInBytes() : 0;
}

int Image::width() const noexcept {
    return m_p ? m_p->width() : 0;
}

int Image::height() const noexcept {
    return m_p ? m_p->height() : 0;
}

int Image::depth() const noexcept {
    return m_p ? m_p->depth() : 0;
}

int Image::bitPlaneCount() const noexcept {
    return m_p ? m_p->bitPlaneCount() : 0;
}

int Image::bytesPerLine() const noexcept {
    return m_p ? m_p->bytesPerLine() : 0;
}

Image::Format Image::format() const noexcept {
    return m_p ? m_p->format() : invalid;
}

bool Image::isContinuous() const noexcept {
    return m_p ? m_p->isContinuous() : true;
}

QImage Image::toQImage() const {
    return m_p ? m_p->toQImage() : QImage();
}

Image Image::clone() const {
    return Image(cloneImagePrivate(m_p));
}

QImage Image::makePaintable() const {
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    return convertTo(argb32).toQImage();
#else
    return convertTo(bgra32).toQImage();
#endif
}

bool Image::save(const QString& fileName, const char* format) const {
    auto qimg = toQImage();
    return qimg.isNull() ? false : qimg.save(fileName, format);
}

bool Image::saveBinary(const QString& fileName) const {
    if (isNull() || !bits()) {
        return false;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    if (isContinuous()) {
        return file.write(reinterpret_cast<const char*>(bits()),
                          sizeInBytes()) == sizeInBytes();
    } else {
        const int bpl = depth() * width();
        const uchar* data = bits();
        for (int i = 0; i < height(); ++i) {
            if (file.write(reinterpret_cast<const char*>(data), bpl) != bpl) {
                return false;
            }
            data += bytesPerLine();
        }
        return true;
    }
}

Image Image::convertTo(Image::Format format) const {
    if (isNull() || this->format() == format) {
        return *this;
    }

    auto converter = image_conversion::getConverter(this->format(), format);
    if (!converter) {
        return Image();
    }

    Image dst(width(), height(), format);

    try {
        auto srcMat = image_conversion::createMat(m_p);
        auto dstMat = image_conversion::createMat(dst.m_p);

        if (srcMat.empty() || dstMat.empty()) {
            return Image();
        }

        converter(srcMat, this->format(), dstMat);

        return dstMat.data == dst.bits() ? dst : Image();
    } catch (const cv::Exception& e) {
        throw ImageConversionError(
                std::format("Image internal, OpenCV Exception: {}", e.msg));
    }
}
} // namespace core
