#pragma once

#include <QImage>
#include <array>
#include <algorithm>
#include <cmath>
#include <memory>
#include <cstring>

class ImageProcessing {
public:
    static QImage applyGrayscale(const QImage& original);
    static QImage adjustBrightness(const QImage& original, int brightnessValue);
    static QImage applyBoxBlur(const QImage& original, int blurRadius);

private:
    ImageProcessing() = delete; // Prevent instantiation

    static void horizontalBlurPass(QImage& image, int blurRadius);
    static void verticalBlurPass(QImage& image, int blurRadius);
};