#pragma once

#include <QWidget>
#include <QPainter>
#include <QImage>

class VideoPlayer : public QWidget
{
	Q_OBJECT

public:
	VideoPlayer(QWidget *parent = nullptr);
	~VideoPlayer();

	void setImage(const QImage& newImage);

	QImage applyEffects(const QImage& image);

	void setGrayscale(bool enabled);
	void setBrightness(int value);
	void setBrightnessEnabled(bool enabled);
	void setBlur(int value);
	void setBlurEnabled(bool enabled);

protected:
	void paintEvent(QPaintEvent* event) override;
	void drawImage(QPainter& painter, const QImage& image, const QSize& size);

private:
	QImage image;
	bool grayscaleEnabled = false;
	int brightnessValue;
	bool brightnessEnabled = false;
	int blurValue = 0;
	bool blurEnabled = false;
};
