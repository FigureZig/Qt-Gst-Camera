#include "imageprocessing.h"

// Function to apply grayscale effect to an image.
QImage ImageProcessing::applyGrayscale(const QImage& original) {
    QImage grayImage = original.copy(); // Create a copy of the original image to modify.
    const int width = grayImage.width(); // Get the width of the image.
    const int height = grayImage.height(); // Get the height of the image.
    uchar* bits = grayImage.bits(); // Access the raw image data.

    // Iterate over each pixel in the image.
    for (int y = 0; y < height; ++y) {
        uchar* row = bits + y * grayImage.bytesPerLine(); // Calculate the start of the row.
        for (int x = 0; x < width; ++x) {
            uchar* pixel = row + x * 3; // Access the pixel at (x, y).
            // Calculate the grayscale value using weighted sum method.
            const int r = pixel[0];
            const int g = pixel[1];
            const int b = pixel[2];
            const int grayValue = (r * 11 + g * 16 + b * 5) >> 5; // Weighted sum and bit-shift for performance.

            // Set the RGB values of the pixel to the grayscale value.
            pixel[0] = pixel[1] = pixel[2] = static_cast<uchar>(grayValue);
        }
    }

    return grayImage; // Return the modified image.
}

// Function to adjust the brightness of an image.
QImage ImageProcessing::adjustBrightness(const QImage& original, int brightnessValue) {
    if (brightnessValue == 0) {
        return original; // Return the original image if brightness adjustment is not needed.
    }

    QImage adjusted = original.copy(); // Copy the original image to modify.
    const auto width = adjusted.width(); // Get the width of the image.
    const auto height = adjusted.height(); // Get the height of the image.
    uchar* bits = adjusted.bits(); // Access the raw image data.
    std::array<uchar, 256> lut{}; // Lookup table for brightness adjustment.

    // Initialize the lookup table with adjusted brightness values.
    for (size_t i = 0; i < lut.size(); ++i) {
        const int adjustedValue = static_cast<int>(i) + brightnessValue; // Adjust the value.
        lut[i] = static_cast<uchar>(std::clamp(adjustedValue, 0, 255)); // Clamp the value to valid range.
    }

    // Apply the brightness adjustment to each pixel using the lookup table.
    const auto end = bits + width * height * 3;
    for (uchar* p = bits; p != end; p += 3) {
        p[0] = lut[p[0]]; // Adjust the red component.
        p[1] = lut[p[1]]; // Adjust the green component.
        p[2] = lut[p[2]]; // Adjust the blue component.
    }

    return adjusted; // Return the brightness-adjusted image.
}

// Function to apply a box blur effect to an image.
QImage ImageProcessing::applyBoxBlur(const QImage& original, int blurValue) {
    if (blurValue < 1) {
        return original; // Return the original image if no blur is applied.
    }
    QImage tempImage = original.copy(); // Copy the original image for processing.

    horizontalBlurPass(tempImage, blurValue); // Apply horizontal blur pass.
    verticalBlurPass(tempImage, blurValue); // Apply vertical blur pass.

    return tempImage; // Return the blurred image.
}

// Apply horizontal blur to an image.
void ImageProcessing::horizontalBlurPass(QImage& image, int blurRadius) {
    const int width = image.width(); // Image width.
    const int height = image.height(); // Image height.
    const int bytesPerLine = image.bytesPerLine(); // Bytes per line in the image data.
    uchar* data = image.bits(); // Raw image data.

    QImage tempImage = image.copy(); // Copy of the image for reading during blur.
    const uchar* tempData = tempImage.constBits(); // Constant raw data of the copied image.

    const int windowSize = 2 * blurRadius + 1; // Size of the blur window.

    // Iterate over each row.
    for (int y = 0; y < height; ++y) {
        int sumRed = 0, sumGreen = 0, sumBlue = 0; // Sum of RGB components in the window.

        // Pre-compute the sum for the initial window.
        for (int kx = -blurRadius; kx <= blurRadius; ++kx) {
            int pixelX = std::clamp(kx, 0, width - 1); // Clamp the x-coordinate to the image bounds.
            const uchar* pixel = tempData + y * bytesPerLine + pixelX * 3; // Access the pixel.
            sumRed += pixel[0]; // Add the red component to the sum.
            sumGreen += pixel[1]; // Add the green component to the sum.
            sumBlue += pixel[2]; // Add the blue component to the sum.
        }

        // Apply the blur effect across the row.
        for (int x = 0; x < width; ++x) {
            // Adjust window sums for the current position.
            if (x > 0) {
                int exitPixelX = std::max(x - blurRadius - 1, 0); // X-coordinate of the pixel exiting the window.
                int enterPixelX = std::min(x + blurRadius, width - 1); // X-coordinate of the pixel entering the window.
                const uchar* exitPixel = tempData + y * bytesPerLine + exitPixelX * 3; // Exiting pixel.
                const uchar* enterPixel = tempData + y * bytesPerLine + enterPixelX * 3; // Entering pixel.

                // Update sums by removing the exit pixel's contribution and adding the enter pixel's.
                sumRed += enterPixel[0] - exitPixel[0];
                sumGreen += enterPixel[1] - exitPixel[1];
                sumBlue += enterPixel[2] - exitPixel[2];
            }

            // Compute the average for the current window and set the pixel's color.
            uchar* pixel = data + y * bytesPerLine + x * 3;
            pixel[0] = static_cast<uchar>(sumRed / windowSize);
            pixel[1] = static_cast<uchar>(sumGreen / windowSize);
            pixel[2] = static_cast<uchar>(sumBlue / windowSize);
        }
    }
}

// Apply vertical blur to an image.
void ImageProcessing::verticalBlurPass(QImage& image, int blurRadius) {
    const int width = image.width(); // Image width.
    const int height = image.height(); // Image height.
    const int bytesPerLine = image.bytesPerLine(); // Bytes per line in the image data.
    uchar* data = image.bits(); // Raw image data.

    QImage tempImage = image.copy(); // Copy of the image for reading during blur.
    const uchar* tempData = tempImage.constBits(); // Constant raw data of the copied image.

    const int windowSize = 2 * blurRadius + 1; // Size of the blur window.

    // Iterate over each column.
    for (int x = 0; x < width; ++x) {
        int sumRed = 0, sumGreen = 0, sumBlue = 0; // Sum of RGB components in the window.

        // Pre-compute the sum for the initial window.
        for (int ky = -blurRadius; ky <= blurRadius; ++ky) {
            int pixelY = std::clamp(ky, 0, height - 1); // Clamp the y-coordinate to the image bounds.
            const uchar* pixel = tempData + pixelY * bytesPerLine + x * 3; // Access the pixel.
            sumRed += pixel[0]; // Add the red component to the sum.
            sumGreen += pixel[1]; // Add the green component to the sum.
            sumBlue += pixel[2]; // Add the blue component to the sum.
        }

        // Apply the blur effect down the column.
        for (int y = 0; y < height; ++y) {
            // Adjust window sums for the current position.
            if (y > 0) {
                int exitPixelY = std::max(y - blurRadius - 1, 0); // Y-coordinate of the pixel exiting the window.
                int enterPixelY = std::min(y + blurRadius, height - 1); // Y-coordinate of the pixel entering the window.
                const uchar* exitPixel = tempData + exitPixelY * bytesPerLine + x * 3; // Exiting pixel.
                const uchar* enterPixel = tempData + enterPixelY * bytesPerLine + x * 3; // Entering pixel.

                // Update sums by removing the exit pixel's contribution and adding the enter pixel's.
                sumRed += enterPixel[0] - exitPixel[0];
                sumGreen += enterPixel[1] - exitPixel[1];
                sumBlue += enterPixel[2] - exitPixel[2];
            }

            // Compute the average for the current window and set the pixel's color.
            uchar* pixel = data + y * bytesPerLine + x * 3;
            pixel[0] = static_cast<uchar>(sumRed / windowSize);
            pixel[1] = static_cast<uchar>(sumGreen / windowSize);
            pixel[2] = static_cast<uchar>(sumBlue / windowSize);
        }
    }
}
