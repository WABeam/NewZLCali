#ifndef IMAGE_H
#define IMAGE_H


#include "exception.hpp"
#include "global.hpp"

#include <cstddef>
#include <exception>
#include <functional>
#include <type_traits>
#include <utility>

#include <QtCore/QSize>
#include <QtGui/QImage>
#include <QObject>
namespace core {
class BadImage : public Exception {
public:
    BadImage(const std::string& whatArg) : Exception(whatArg) {}
    BadImage(const char* whatArg) : Exception(whatArg) {}
};

class ImageConversionError : public Exception {
public:
    ImageConversionError(const std::string& whatArg) : Exception(whatArg) {}
    ImageConversionError(const char* whatArg) : Exception(whatArg) {}
};

class ImagePrivate;

class Image {
public:
    enum class Format {
        invalid,
        // 8 bits
        // bayer
        bayer8_rggb,
        bayer8_grbg,
        bayer8_bggr,
        bayer8_gbrg,
        // yuv 4:2:2 packed
        yuv8_uyvy,
        yuv8_yuy2,
        yuv8_yvyu,
        // rgb
        rgb24,
        bgr24,
        // rgba
        rgba32,
        bgra32,
        argb32,
        abgr32,
        // gray
        grayscale8,

        // 10 bits
        // bayer
        bayer10_rggb,
        bayer10_grbg,
        bayer10_bggr,
        bayer10_gbrg,

        // 12 bits
        // bayer
        bayer12_rggb,
        bayer12_grbg,
        bayer12_bggr,
        bayer12_gbrg,

        // 14 bits
        // bayer
        bayer14_rggb,
        bayer14_grbg,
        bayer14_bggr,
        bayer14_gbrg,

        // 16 bits
        // bayer
        bayer16_rggb,
        bayer16_grbg,
        bayer16_bggr,
        bayer16_gbrg
    };

    using enum Format;
    using CleanupFunction = void (*)(uchar*) noexcept;

    Image() noexcept;
    Image(int width, int height, Format format);
    Image(const QSize& size, Format format);
    Image(uchar* data, int width, int height, Format format,
          int bytesPerLine = -1, CleanupFunction cleanup = nullptr);
    Image(uchar* data, const QSize& size, Format format, int bytesPerLine = -1,
          CleanupFunction cleanup = nullptr);
    Image(const QImage& image);
    Image(const QString& fileName, const char* format = nullptr);

    ~Image();
    Image(const Image&) noexcept;
    Image(Image&&) noexcept;

    Image& operator=(Image rhs) noexcept {
        swap(*this, rhs);
        return *this;
    }

    uchar* bits() noexcept;
    const uchar* bits() const noexcept;
    qsizetype sizeInBytes() const noexcept;
    int width() const noexcept;
    int height() const noexcept;
    int depth() const noexcept;
    int bitPlaneCount() const noexcept;
    int bytesPerLine() const noexcept;
    Format format() const noexcept;
    bool isContinuous() const noexcept;

    QImage toQImage() const;

    bool isNull() const noexcept;

    Image clone() const;
    QImage makePaintable() const;

    Image convertTo(Format format) const;

    bool save(const QString& fileName, const char* format = nullptr) const;
    bool saveBinary(const QString& fileName) const;

    QSize size() const noexcept {
        return {width(), height()};
    }

    friend void swap(Image& lhs, Image& rhs) noexcept {
        std::swap(lhs.m_p, rhs.m_p);
    }

private:
    Image(ImagePrivate* p) noexcept;

    ImagePrivate* m_p;
};
} // namespace core

REGISTER_QT_METATYPE(core::Image, core__Image)


#endif // IMAGE_H
