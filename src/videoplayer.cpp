#include "videoplayer.h"
#include "imageprocessing.h"

// Constructor for the VideoPlayer. Initializes the QWidget with the provided parent.
VideoPlayer::VideoPlayer(QWidget* parent)
    : QWidget(parent) {
}

// Default destructor.
VideoPlayer::~VideoPlayer() = default;

// Sets the current image to be displayed and triggers a UI update.
void VideoPlayer::setImage(const QImage& newImage) {
    image = newImage; // Update the current image with the new image.
    update(); // Request a repaint of the widget.
}

// Custom paint event handler to draw the image on the VideoPlayer widget.
void VideoPlayer::paintEvent(QPaintEvent* event) {
    QPainter painter(this); // Create a QPainter to draw on the widget.
    QSize fixedSize(800, 600); // Specify a fixed size for the image.
    QImage processedImage = applyEffects(image); // Apply visual effects to the image.
    drawImage(painter, processedImage, fixedSize); // Draw the processed image.
}

// Draws an image on the widget, scaling it to a specified size.
void VideoPlayer::drawImage(QPainter& painter, const QImage& image, const QSize& size) {
    if (!image.isNull()) {
        // Scale the image to fit within the specified size, maintaining aspect ratio.
        QImage scaledImage = image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawImage(QPoint(0, 0), scaledImage); // Draw the scaled image at the widget's origin.
    }
    else {
        // Fill the widget with a cyan color if there's no image to draw.
        painter.fillRect(rect(), Qt::cyan);
    }
}

// Applies enabled visual effects to the image.
QImage VideoPlayer::applyEffects(const QImage& image) {
    QImage processedImage = image; // Start with the original image.
    if (grayscaleEnabled) {
        // Apply grayscale effect if enabled.
        processedImage = ImageProcessing::applyGrayscale(processedImage);
    }
    if (brightnessEnabled) {
        // Adjust brightness if enabled.
        processedImage = ImageProcessing::adjustBrightness(processedImage, brightnessValue);
    }
    if (blurEnabled) {
        // Apply blur effect if enabled.
        processedImage = ImageProcessing::applyBoxBlur(processedImage, blurValue);
    }
    return processedImage; // Return the image with all applied effects.
}

// Enables or disables the grayscale effect and triggers a UI update.
void VideoPlayer::setGrayscale(bool enabled) {
    grayscaleEnabled = enabled;
    update(); // Request a repaint to apply changes.
}

// Sets the brightness level and triggers a UI update.
void VideoPlayer::setBrightness(int value) {
    brightnessValue = value;
    update(); // Request a repaint to apply changes.
}

// Enables or disables brightness adjustment and triggers a UI update.
void VideoPlayer::setBrightnessEnabled(bool enabled) {
    brightnessEnabled = enabled;
    brightnessValue = 0;
    update(); // Request a repaint to apply changes.
}

// Sets the blur level and triggers a UI update.
void VideoPlayer::setBlur(int value) {
    blurValue = value;
    update(); // Request a repaint to apply changes.
}

// Enables or disables the blur effect and triggers a UI update.
void VideoPlayer::setBlurEnabled(bool enabled) {
    blurEnabled = enabled;
    update(); // Request a repaint to apply changes.
}
