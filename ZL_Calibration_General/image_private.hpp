#pragma once

#include "image.h"

#include <atomic>
#include <utility>

#include <QtGlobal>
#include <QtGui/QImage>

#include <boost/safe_numerics/safe_integer.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace core {
inline int depthForFormat(Image::Format format) noexcept {
    switch (format) {
    case Image::bayer8_rggb:
    case Image::bayer8_grbg:
    case Image::bayer8_bggr:
    case Image::bayer8_gbrg:
    case Image::yuv8_uyvy:
    case Image::yuv8_yuy2:
    case Image::yuv8_yvyu:
    case Image::grayscale8:
        return 8;
    case Image::bayer10_rggb:
    case Image::bayer10_grbg:
    case Image::bayer10_bggr:
    case Image::bayer10_gbrg:
    case Image::bayer12_rggb:
    case Image::bayer12_grbg:
    case Image::bayer12_bggr:
    case Image::bayer12_gbrg:
    case Image::bayer14_rggb:
    case Image::bayer14_grbg:
    case Image::bayer14_bggr:
    case Image::bayer14_gbrg:
    case Image::bayer16_rggb:
    case Image::bayer16_grbg:
    case Image::bayer16_bggr:
    case Image::bayer16_gbrg:
        return 16;
    case Image::rgb24:
    case Image::bgr24:
        return 24;
    case Image::rgba32:
    case Image::bgra32:
    case Image::argb32:
    case Image::abgr32:
        return 32;
    default:
        return 0;
    }
}

inline int bitPlaneCountForFormat(Image::Format format) noexcept {
    switch (format) {
    case Image::bayer8_rggb:
    case Image::bayer8_grbg:
    case Image::bayer8_bggr:
    case Image::bayer8_gbrg:
    case Image::yuv8_uyvy:
    case Image::yuv8_yuy2:
    case Image::yuv8_yvyu:
    case Image::grayscale8:
        return 8;
    case Image::bayer10_rggb:
    case Image::bayer10_grbg:
    case Image::bayer10_bggr:
    case Image::bayer10_gbrg:
        return 10;
    case Image::bayer12_rggb:
    case Image::bayer12_grbg:
    case Image::bayer12_bggr:
    case Image::bayer12_gbrg:
        return 12;
    case Image::bayer14_rggb:
    case Image::bayer14_grbg:
    case Image::bayer14_bggr:
    case Image::bayer14_gbrg:
        return 14;
    case Image::bayer16_rggb:
    case Image::bayer16_grbg:
    case Image::bayer16_bggr:
    case Image::bayer16_gbrg:
        return 16;
    case Image::rgb24:
    case Image::bgr24:
        return 24;
    case Image::rgba32:
    case Image::bgra32:
    case Image::argb32:
    case Image::abgr32:
        return 32;
    default:
        return 0;
    }
}

inline Image::Format formatFromQImageFormat(QImage::Format format) noexcept {
    switch (format) {
    case QImage::Format_RGB32:
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        return Image::argb32;
#else
        return Image::bgra32;
#endif
    case QImage::Format_ARGB32:
        return Image::argb32;
    case QImage::Format_RGB888:
        return Image::rgb24;
    case QImage::Format_RGBX8888:
    case QImage::Format_RGBA8888:
        return Image::rgba32;
    case QImage::Format_Grayscale8:
        return Image::grayscale8;
    case QImage::Format_BGR888:
        return Image::bgr24;
    default:
        return Image::invalid;
    }
}

inline QImage::Format qImageFormatFromFormat(Image::Format format) noexcept {
    switch (format) {
    case Image::rgb24:
        return QImage::Format_RGB888;
    case Image::bgr24:
        return QImage::Format_BGR888;
    case Image::rgba32:
        return QImage::Format_RGBA8888;
    case Image::bgra32:
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        return QImage::Format_Invalid;
#else
        return QImage::Format_RGB32;
#endif
    case Image::argb32:
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
        return QImage::Format_RGB32;
#else
        return QImage::Format_ARGB32;
#endif
    case Image::grayscale8:
        return QImage::Format_Grayscale8;
    default:
        return QImage::Format_Invalid;
    }
}

struct ImageSizeParams {
    qsizetype nBytes{0};
    int depth{0};
    int bpc{0};
    int bpl{0};
    bool isContinuous{true};
};

inline ImageSizeParams calculateImageSizeParams(int width, int height,
                                                Image::Format format,
                                                int bytesPerLine = -1) {
    using SafeSize = boost::safe_numerics::safe<qsizetype>;
    using SafeInt = boost::safe_numerics::safe<int>;

    if (width < 0 || height < 0) {
        throw BadImage("Image width or height is negative.");
    }

    if (width == 0 || height == 0 || format == Image::invalid) {
        return ImageSizeParams();
    }

    const int depth = depthForFormat(format);
    const int bpc = bitPlaneCountForFormat(format);

    if (depth == 0 || bpc == 0) {
        throw BadImage("Unsupported image format.");
    }

    const auto [bpl, isContinuous] = [width, depth, bytesPerLine]() {
        const int perfectBpl = SafeInt(width) * (depth >> 3);
        if (bytesPerLine > 0 && bytesPerLine < perfectBpl) {
            throw BadImage("Bytes per line is too small.");
        }
        return std::pair((std::max)(bytesPerLine, perfectBpl),
                         bytesPerLine <= 0 || bytesPerLine == perfectBpl);
    }();

    const qsizetype nBytes = height * SafeSize(bpl);

    return ImageSizeParams(nBytes, depth, bpc, bpl, isContinuous);
}

class ImagePrivate {
public:
    using Format = Image::Format;
    using enum Image::Format;

    ImagePrivate() noexcept = default;
    virtual ~ImagePrivate() = default;

    ImagePrivate(const ImagePrivate&) = delete;
    ImagePrivate(ImagePrivate&&) = delete;
    ImagePrivate& operator=(const ImagePrivate&) = delete;
    ImagePrivate& operator=(ImagePrivate&&) = delete;

    virtual uchar* bits() noexcept = 0;
    virtual const uchar* bits() const noexcept = 0;
    virtual qsizetype sizeInBytes() const noexcept = 0;
    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;
    virtual int depth() const noexcept = 0;
    virtual int bitPlaneCount() const noexcept = 0;
    virtual int bytesPerLine() const noexcept = 0;
    virtual Format format() const noexcept = 0;
    virtual bool isContinuous() const noexcept = 0;

    virtual QImage toQImage() const {
        QImage qimage(
                bits(), width(), height(), bytesPerLine(),
                qImageFormatFromFormat(format()),
                [](void* info) noexcept {
                    static_cast<const ImagePrivate*>(info)->dref();
                },
                static_cast<void*>(const_cast<ImagePrivate*>(this)));

        if (!qimage.isNull()) {
            ref();
        }

        return qimage;
    }

    void ref() const noexcept {
        m_rc.fetch_add(1, std::memory_order_relaxed);
    }

    void dref() const noexcept {
        if (m_rc.fetch_sub(1, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete this;
        }
    }

private:
    mutable std::atomic_int_least32_t m_rc;
};

class ImageData : public ImagePrivate {
public:
    ImageData() noexcept :
            m_ptr(nullptr), m_nBytes(0), m_width(0), m_height(0), m_depth(0),
            m_bpc(0), m_bpl(0), m_format(invalid) {}

    ImageData(int width, int height, Format format,
              const ImageSizeParams& params) :
            m_ptr(new uchar[params.nBytes]()),
            m_nBytes(params.nBytes), m_width(width), m_height(height),
            m_depth(params.depth), m_bpc(params.bpc), m_bpl(params.bpl),
            m_format(format) {}

    virtual ~ImageData() {
        delete[] m_ptr;
    }

    virtual uchar* bits() noexcept override {
        return m_ptr;
    }

    virtual const uchar* bits() const noexcept override {
        return m_ptr;
    }

    virtual qsizetype sizeInBytes() const noexcept override {
        return m_nBytes;
    }

    virtual int width() const noexcept override {
        return m_width;
    }

    virtual int height() const noexcept override {
        return m_height;
    }

    virtual int depth() const noexcept override {
        return m_depth;
    }

    virtual int bitPlaneCount() const noexcept override {
        return m_bpc;
    }

    virtual int bytesPerLine() const noexcept override {
        return m_bpl;
    }

    virtual Format format() const noexcept override {
        return m_format;
    }

    virtual bool isContinuous() const noexcept override {
        return true;
    }

private:
    uchar* m_ptr;
    qsizetype m_nBytes;
    int m_width;
    int m_height;
    int m_depth;
    int m_bpc;
    int m_bpl;
    Format m_format;
};

class ImageUserData : public ImagePrivate {
public:
    using CleanupFunction = Image::CleanupFunction;

    ImageUserData() noexcept :
            m_ptr(nullptr), m_cleanup(nullptr), m_nBytes(0), m_width(0),
            m_height(0), m_depth(0), m_bpc(0), m_bpl(0), m_format(invalid),
            m_isContinuous(true) {}

    ImageUserData(uchar* data, int width, int height, Format format,
                  int bytesPerLine, CleanupFunction cleanup,
                  const ImageSizeParams& params) noexcept :
            m_ptr(data),
            m_cleanup(cleanup), m_nBytes(params.nBytes), m_width(width),
            m_height(height), m_depth(params.depth), m_bpc(params.bpc),
            m_bpl(params.bpl), m_format(format),
            m_isContinuous(params.isContinuous) {}

    virtual ~ImageUserData() {
        if (m_cleanup) {
            m_cleanup(m_ptr);
        }
    }

    virtual uchar* bits() noexcept override {
        return m_ptr;
    }

    virtual const uchar* bits() const noexcept override {
        return m_ptr;
    }

    virtual qsizetype sizeInBytes() const noexcept override {
        return m_nBytes;
    }

    virtual int width() const noexcept override {
        return m_width;
    }

    virtual int height() const noexcept override {
        return m_height;
    }

    virtual int depth() const noexcept override {
        return m_depth;
    }

    virtual int bitPlaneCount() const noexcept override {
        return m_bpc;
    }

    virtual int bytesPerLine() const noexcept override {
        return m_bpl;
    }

    virtual Format format() const noexcept override {
        return m_format;
    }

    virtual bool isContinuous() const noexcept override {
        return m_isContinuous;
    }

private:
    uchar* m_ptr;
    CleanupFunction m_cleanup;
    qsizetype m_nBytes;
    int m_width;
    int m_height;
    int m_depth;
    int m_bpc;
    int m_bpl;
    Format m_format;
    bool m_isContinuous;
};

class QImageHolder : public ImagePrivate {
public:
    QImageHolder() noexcept : m_isContinuous(true) {}

    QImageHolder(const QImage& image, Format format) : QImageHolder() {
        m_format = format;
        m_isContinuous = image.width() * image.depth() == image.bytesPerLine();
        m_image = image;
    }

    virtual uchar* bits() noexcept {
        return m_image.bits();
    }

    virtual const uchar* bits() const noexcept {
        return m_image.bits();
    }

    virtual qsizetype sizeInBytes() const noexcept {
        return m_image.sizeInBytes();
    }

    virtual int width() const noexcept {
        return m_image.width();
    }

    virtual int height() const noexcept {
        return m_image.height();
    }

    virtual int depth() const noexcept {
        return m_image.depth();
    }

    virtual int bitPlaneCount() const noexcept {
        return m_image.bitPlaneCount();
    }

    virtual int bytesPerLine() const noexcept {
        return m_image.bytesPerLine();
    }

    virtual Format format() const noexcept {
        return m_format;
    }

    virtual bool isContinuous() const noexcept {
        return m_isContinuous;
    }

    virtual QImage toQImage() const noexcept override {
        return m_image;
    }

private:
    QImage m_image;
    Format m_format;
    bool m_isContinuous;
};

inline ImagePrivate* createImagePrivate(int width, int height,
                                        Image::Format format) {
    const auto params = calculateImageSizeParams(width, height, format);
    return params.nBytes > 0 ? new ImageData(width, height, format, params)
                             : nullptr;
}

inline ImagePrivate* createImagePrivate(uchar* data, int width, int height,
                                        Image::Format format, int bpl,
                                        Image::CleanupFunction cleanup) {
    const auto params = calculateImageSizeParams(width, height, format, bpl);
    return params.nBytes > 0 ? new ImageUserData(data, width, height, format,
                                                 bpl, cleanup, params)
                             : nullptr;
}

inline ImagePrivate* createImagePrivate(const QImage& image) {
    const auto format = formatFromQImageFormat(image.format());
    return format == Image::invalid ? nullptr : new QImageHolder(image, format);
}

inline ImagePrivate* cloneImagePrivate(ImagePrivate* p) {
    if (!p) {
        return nullptr;
    }

    std::unique_ptr<ImagePrivate> replica(
            createImagePrivate(p->width(), p->height(), p->format()));
    if (replica) {
        if (p->isContinuous()) {
            std::copy_n(p->bits(), p->sizeInBytes(), replica->bits());
        } else {
            const uchar* src = p->bits();
            uchar* dst = replica->bits();
            const int srcStep = p->bytesPerLine();
            const int dstStep = replica->bytesPerLine();

            Q_ASSERT(srcStep > dstStep);

            for (int i = 0; i < p->height(); ++i) {
                std::copy_n(src, dstStep, dst);
                src += srcStep;
                dst += dstStep;
            }
        }
    }

    return replica.release();
}

inline ImagePrivate* convertImagePrivate(ImagePrivate* p, Image::Format format);
} // namespace core
