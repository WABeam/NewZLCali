#pragma once

#include "image_private.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace core {
namespace image_conversion {
inline int opencvTypeForFormat(Image::Format format) noexcept {
    switch (format) {
    case Image::bayer8_rggb:
    case Image::bayer8_grbg:
    case Image::bayer8_bggr:
    case Image::bayer8_gbrg:
    case Image::grayscale8:
        return CV_8UC1;
    case Image::yuv8_uyvy:
    case Image::yuv8_yuy2:
    case Image::yuv8_yvyu:
        return CV_8UC2;
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
        return CV_16UC1;
    case Image::rgb24:
    case Image::bgr24:
        return CV_8UC3;
    case Image::rgba32:
    case Image::bgra32:
    case Image::argb32:
    case Image::abgr32:
        return CV_8UC4;
    default:
        return -1;
    }
}

inline cv::Mat createMat(ImagePrivate* p) {
    return p ? cv::Mat(p->height(), p->width(),
                       opencvTypeForFormat(p->format()), p->bits(),
                       p->bytesPerLine())
             : cv::Mat();
}

inline void cvtColor(const cv::Mat& src, cv::Mat& dst, int code) {
    cv::cvtColor(src, dst, code);
}

inline void cvtColor(const cv::Mat& src, cv::Mat& dst, int code, int ddepth,
                     double alpha) {
    cv::Mat tmp;
    cvtColor(src, tmp, code);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void argbToRGB(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {1, 0, 2, 1, 3, 2};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 3);
}

inline void argbToBGR(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {1, 2, 2, 1, 3, 0};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 3);
}

inline void argbToRGBA(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {0, 3, 1, 0, 2, 1, 3, 2};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 4);
}

inline void argbToBGRA(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {0, 3, 1, 2, 2, 1, 3, 0};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 4);
}

inline void argbToGrayscale(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat tmp;
    argbToBGRA(src, tmp);
    cv::cvtColor(tmp, dst, cv::COLOR_BGRA2GRAY);
}

inline void abgrToRGB(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {1, 2, 2, 1, 3, 0};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 3);
}

inline void abgrToBGR(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {1, 0, 2, 1, 3, 2};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 3);
}

inline void abgrToRGBA(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {0, 3, 1, 2, 2, 1, 3, 0};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 4);
}

inline void abgrToBGRA(const cv::Mat& src, cv::Mat& dst) {
    constexpr const int fromTo[] = {0, 3, 1, 0, 2, 1, 3, 2};
    cv::mixChannels(&src, 1, &dst, 1, &fromTo[0], 4);
}

inline void abgrToGrayscale(const cv::Mat& src, cv::Mat& dst) {
    cv::Mat tmp;
    abgrToBGRA(src, tmp);
    cv::cvtColor(tmp, dst, cv::COLOR_BGRA2GRAY);
}

inline void argbToRGB(const cv::Mat& src, cv::Mat& dst, int ddepth,
                      double alpha) {
    cv::Mat tmp;
    argbToRGB(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void argbToBGR(const cv::Mat& src, cv::Mat& dst, int ddepth,
                      double alpha) {
    cv::Mat tmp;
    argbToBGR(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void argbToRGBA(const cv::Mat& src, cv::Mat& dst, int ddepth,
                       double alpha) {
    cv::Mat tmp;
    argbToRGBA(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void argbToBGRA(const cv::Mat& src, cv::Mat& dst, int ddepth,
                       double alpha) {
    cv::Mat tmp;
    argbToBGRA(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void argbToGrayscale(const cv::Mat& src, cv::Mat& dst, int ddepth,
                            double alpha) {
    cv::Mat tmp;
    argbToGrayscale(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void abgrToRGB(const cv::Mat& src, cv::Mat& dst, int ddepth,
                      double alpha) {
    cv::Mat tmp;
    abgrToRGB(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void abgrToBGR(const cv::Mat& src, cv::Mat& dst, int ddepth,
                      double alpha) {
    cv::Mat tmp;
    abgrToBGR(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void abgrToRGBA(const cv::Mat& src, cv::Mat& dst, int ddepth,
                       double alpha) {
    cv::Mat tmp;
    abgrToRGBA(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void abgrToBGRA(const cv::Mat& src, cv::Mat& dst, int ddepth,
                       double alpha) {
    cv::Mat tmp;
    abgrToBGRA(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void abgrToGrayscale(const cv::Mat& src, cv::Mat& dst, int ddepth,
                            double alpha) {
    cv::Mat tmp;
    abgrToGrayscale(src, tmp);
    tmp.convertTo(dst, ddepth, alpha);
}

inline void toRGB24(const cv::Mat& src, Image::Format format, cv::Mat& dst) {
    switch (format) {
    case Image::bayer8_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGB);
        break;
    case Image::bayer10_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGB, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGB, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGB, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGB, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGB);
        break;
    case Image::bayer10_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGB, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGB, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGB, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGB, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGB);
        break;
    case Image::bayer10_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGB, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGB, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGB, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGB, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGB);
        break;
    case Image::bayer10_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGB, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGB, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGB, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGB, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::yuv8_uyvy:
        cvtColor(src, dst, cv::COLOR_YUV2RGB_UYVY);
        break;
    case Image::yuv8_yuy2:
        cvtColor(src, dst, cv::COLOR_YUV2RGB_YUY2);
        break;
    case Image::yuv8_yvyu:
        cvtColor(src, dst, cv::COLOR_YUV2RGB_YVYU);
        break;
    case Image::rgb24:
        src.copyTo(dst);
        break;
    case Image::bgr24:
        cvtColor(src, dst, cv::COLOR_BGR2RGB);
        break;
    case Image::rgba32:
        cvtColor(src, dst, cv::COLOR_RGBA2RGB);
        break;
    case Image::bgra32:
        cvtColor(src, dst, cv::COLOR_BGRA2RGB);
        break;
    case Image::argb32:
        argbToRGB(src, dst);
        break;
    case Image::abgr32:
        abgrToRGB(src, dst);
        break;
    case Image::grayscale8:
        cvtColor(src, dst, cv::COLOR_GRAY2RGB);
        break;
    default:
        break;
    }
}

inline void toBGR24(const cv::Mat& src, Image::Format format, cv::Mat& dst) {
    switch (format) {
    case Image::bayer8_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGR);
        break;
    case Image::bayer10_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGR, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGR, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGR, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGR, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGR);
        break;
    case Image::bayer10_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGR, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGR, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGR, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGR, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGR);
        break;
    case Image::bayer10_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGR, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGR, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGR, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGR, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGR);
        break;
    case Image::bayer10_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGR, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGR, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGR, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGR, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::yuv8_uyvy:
        cvtColor(src, dst, cv::COLOR_YUV2BGR_UYVY);
        break;
    case Image::yuv8_yuy2:
        cvtColor(src, dst, cv::COLOR_YUV2BGR_YUY2);
        break;
    case Image::yuv8_yvyu:
        cvtColor(src, dst, cv::COLOR_YUV2BGR_YVYU);
        break;
    case Image::rgb24:
        cvtColor(src, dst, cv::COLOR_RGB2BGR);
        break;
    case Image::bgr24:
        src.copyTo(dst);
        break;
    case Image::rgba32:
        cvtColor(src, dst, cv::COLOR_RGBA2BGR);
        break;
    case Image::bgra32:
        cvtColor(src, dst, cv::COLOR_BGRA2BGR);
        break;
    case Image::argb32:
        argbToBGR(src, dst);
        break;
    case Image::abgr32:
        abgrToBGR(src, dst);
        break;
    case Image::grayscale8:
        cvtColor(src, dst, cv::COLOR_GRAY2BGR);
        break;
    default:
        break;
    }
}

inline void toRGBA32(const cv::Mat& src, Image::Format format, cv::Mat& dst) {
    switch (format) {
    case Image::bayer8_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGBA);
        break;
    case Image::bayer10_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGBA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGBA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGBA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2RGBA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGBA);
        break;
    case Image::bayer10_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGBA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGBA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGBA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2RGBA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGBA);
        break;
    case Image::bayer10_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGBA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGBA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGBA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2RGBA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGBA);
        break;
    case Image::bayer10_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGBA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGBA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGBA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2RGBA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::yuv8_uyvy:
        cvtColor(src, dst, cv::COLOR_YUV2RGBA_UYVY);
        break;
    case Image::yuv8_yuy2:
        cvtColor(src, dst, cv::COLOR_YUV2RGBA_YUY2);
        break;
    case Image::yuv8_yvyu:
        cvtColor(src, dst, cv::COLOR_YUV2RGBA_YVYU);
        break;
    case Image::rgb24:
        cvtColor(src, dst, cv::COLOR_RGB2RGBA);
        break;
    case Image::bgr24:
        cvtColor(src, dst, cv::COLOR_BGR2RGBA);
        break;
    case Image::rgba32:
        src.copyTo(dst);
        break;
    case Image::bgra32:
        cvtColor(src, dst, cv::COLOR_BGRA2RGBA);
        break;
    case Image::argb32:
        argbToRGBA(src, dst);
        break;
    case Image::abgr32:
        abgrToRGBA(src, dst);
        break;
    case Image::grayscale8:
        cvtColor(src, dst, cv::COLOR_GRAY2RGBA);
        break;
    default:
        break;
    }
}

inline void toBGRA32(const cv::Mat& src, Image::Format format, cv::Mat& dst) {
    switch (format) {
    case Image::bayer8_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGRA);
        break;
    case Image::bayer10_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGRA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGRA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGRA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2BGRA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGRA);
        break;
    case Image::bayer10_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGRA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGRA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGRA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2BGRA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGRA);
        break;
    case Image::bayer10_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGRA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGRA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGRA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2BGRA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGRA);
        break;
    case Image::bayer10_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGRA, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGRA, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGRA, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2BGRA, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::yuv8_uyvy:
        cvtColor(src, dst, cv::COLOR_YUV2BGRA_UYVY);
        break;
    case Image::yuv8_yuy2:
        cvtColor(src, dst, cv::COLOR_YUV2BGRA_YUY2);
        break;
    case Image::yuv8_yvyu:
        cvtColor(src, dst, cv::COLOR_YUV2BGRA_YVYU);
        break;
    case Image::rgb24:
        cvtColor(src, dst, cv::COLOR_RGB2BGRA);
        break;
    case Image::bgr24:
        cvtColor(src, dst, cv::COLOR_BGR2BGRA);
        break;
    case Image::rgba32:
        cvtColor(src, dst, cv::COLOR_RGBA2BGRA);
        break;
    case Image::bgra32:
        src.copyTo(dst);
        break;
    case Image::argb32:
        argbToBGRA(src, dst);
        break;
    case Image::abgr32:
        abgrToBGRA(src, dst);
        break;
    case Image::grayscale8:
        cvtColor(src, dst, cv::COLOR_GRAY2BGRA);
        break;
    default:
        break;
    }
}

inline void toARGB32(const cv::Mat& src, Image::Format format, cv::Mat& dst) {
    cv::Mat rgba;
    toRGBA32(src, format, rgba);
    if (!rgba.empty()) {
        constexpr const int fromTo[] = {0, 1, 1, 2, 2, 3, 3, 0};
        cv::mixChannels(&rgba, 1, &dst, 1, &fromTo[0], 4);
    }
}

inline void toABGR32(const cv::Mat& src, Image::Format format, cv::Mat& dst) {
    cv::Mat bgra;
    toBGRA32(src, format, bgra);
    if (!bgra.empty()) {
        constexpr const int fromTo[] = {0, 1, 1, 2, 2, 3, 3, 0};
        cv::mixChannels(&bgra, 1, &dst, 1, &fromTo[0], 4);
    }
}

inline void toGrayscale8(const cv::Mat& src, Image::Format format,
                         cv::Mat& dst) {
    switch (format) {
    case Image::bayer8_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2GRAY);
        break;
    case Image::bayer10_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2GRAY, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2GRAY, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2GRAY, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_rggb:
        cvtColor(src, dst, cv::COLOR_BayerBG2GRAY, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2GRAY);
        break;
    case Image::bayer10_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2GRAY, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2GRAY, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2GRAY, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_grbg:
        cvtColor(src, dst, cv::COLOR_BayerGB2GRAY, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2GRAY);
        break;
    case Image::bayer10_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2GRAY, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2GRAY, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2GRAY, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_bggr:
        cvtColor(src, dst, cv::COLOR_BayerRG2GRAY, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::bayer8_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2GRAY);
        break;
    case Image::bayer10_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2GRAY, CV_8U, 1.0 / (1 << 2));
        break;
    case Image::bayer12_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2GRAY, CV_8U, 1.0 / (1 << 4));
        break;
    case Image::bayer14_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2GRAY, CV_8U, 1.0 / (1 << 6));
        break;
    case Image::bayer16_gbrg:
        cvtColor(src, dst, cv::COLOR_BayerGR2GRAY, CV_8U, 1.0 / (1 << 8));
        break;
    case Image::yuv8_uyvy:
        cvtColor(src, dst, cv::COLOR_YUV2GRAY_UYVY);
        break;
    case Image::yuv8_yuy2:
        cvtColor(src, dst, cv::COLOR_YUV2GRAY_YUY2);
        break;
    case Image::yuv8_yvyu:
        cvtColor(src, dst, cv::COLOR_YUV2GRAY_YVYU);
        break;
    case Image::rgb24:
        cvtColor(src, dst, cv::COLOR_RGB2GRAY);
        break;
    case Image::bgr24:
        cvtColor(src, dst, cv::COLOR_BGR2GRAY);
        break;
    case Image::rgba32:
        cvtColor(src, dst, cv::COLOR_RGBA2GRAY);
        break;
    case Image::bgra32:
        cvtColor(src, dst, cv::COLOR_BGRA2GRAY);
        break;
    case Image::argb32:
        argbToGrayscale(src, dst);
        break;
    case Image::abgr32:
        abgrToGrayscale(src, dst);
        break;
    case Image::grayscale8:
        src.copyTo(dst);
        break;
    default:
        break;
    }
}

using Converter = void (*)(const cv::Mat&, Image::Format, cv::Mat& dst);

inline Converter getConverter(Image::Format from, Image::Format to) noexcept {
    Q_UNUSED(from)

    switch (to) {
    case Image::rgb24:
        return &toRGB24;
    case Image::bgr24:
        return &toBGR24;
    case Image::rgba32:
        return &toRGBA32;
    case Image::bgra32:
        return &toBGRA32;
    case Image::argb32:
        return &toARGB32;
    case Image::abgr32:
        return &toABGR32;
    case Image::grayscale8:
        return &toGrayscale8;
    default:
        return nullptr;
    }
}
} // namespace image_conversion
} // namespace core
