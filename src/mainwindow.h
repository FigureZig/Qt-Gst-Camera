#pragma once
#include "gstreamerhandler.h"
#include "videoplayer.h"
#include "ui_mainwindow.h"
#include "styleloader.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <memory>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private:
    void setupUI();
    void setupVideoPlayer(QVBoxLayout* mainLayout);
    void setupControls(QVBoxLayout* mainLayout);
    void setupConnections();
    void initializeVideoPlayer();

    void applyGrayscaleEffect(int state);
    void enableBrightnessAdjustment(int state);
    void adjustBrightnessEffect(int value);
    void enableBlurAdjustment(int state);
    void applyBlurEffect(int value);

    Ui::MainWindowClass ui;
    std::unique_ptr<GStreamerHandler> gstreamerHandler;
    std::unique_ptr<VideoPlayer> videoPlayer;
};
