#include "mainwindow.h"
#include "styleloader.h"

// Constructor: Sets up the main window, video player, and GStreamer handler.
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    gstreamerHandler(std::make_unique<GStreamerHandler>(this)), // Initialize GStreamer handler with the MainWindow as parent.
    videoPlayer(std::make_unique<VideoPlayer>()) { // Initialize the video player.
    setupUI(); // Setup the user interface elements.
    setupConnections(); // Connect UI signals to slots.
    initializeVideoPlayer(); // Initialize the video player with GStreamer.
}

// Destructor: Default implementation suffices since smart pointers handle cleanup.
MainWindow::~MainWindow() = default;

// setupUI: Constructs and arranges UI components.
void MainWindow::setupUI() {
    ui.setupUi(this); // Setup UI from designer file.
    setWindowTitle(tr("Demo")); // Set the window title.
    setStyleSheet(StyleLoader::Load(":/qdarkstyle/style.qss")); // Apply a stylesheet for styling the UI.
    setFixedSize(QSize(800, 800)); // Set a fixed size for the window.
    QIcon appIcon(":icon/icon.png"); // Load an application icon.
    setWindowIcon(appIcon); // Set the loaded icon as the window icon.

    // Setup the central widget and its layout.
    auto* centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(centralWidget);

    // Add video player and control elements to the layout.
    setupVideoPlayer(mainLayout);
    setupControls(mainLayout);

    centralWidget->setLayout(mainLayout); // Set the layout for the central widget.
    setCentralWidget(centralWidget); // Make the central widget the main window's central widget.
}

// setupVideoPlayer: Adds the video player widget to the main layout.
void MainWindow::setupVideoPlayer(QVBoxLayout* mainLayout) {
    mainLayout->addWidget(videoPlayer.get(), 1); // Add the video player to the layout with a stretch factor.
}

// setupControls: Creates and adds UI controls for video effects to the layout.
void MainWindow::setupControls(QVBoxLayout* mainLayout) {
    auto* controlsLayout = new QVBoxLayout(); // Layout for the controls.
    // Add various controls for video effects to the layout.
    controlsLayout->addWidget(ui.grayscaleCheckBox);
    controlsLayout->addWidget(ui.enableBrightness);
    controlsLayout->addWidget(ui.brightnessSlider);
    controlsLayout->addWidget(ui.enableBlur);
    controlsLayout->addWidget(ui.blurSlider);

    // Initially disable sliders until their corresponding checkboxes are checked.
    ui.brightnessSlider->setEnabled(false);
    ui.blurSlider->setEnabled(false);
    // Add a spacer item for aesthetic spacing in the UI.
    controlsLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    mainLayout->addLayout(controlsLayout, 0); // Add the controls layout to the main layout with no stretch factor.
}

// setupConnections: Connects UI elements' signals to the corresponding slots.
void MainWindow::setupConnections() {
    // Connect state changes of checkboxes to enable/disable sliders and apply effects.
    connect(ui.enableBrightness, &QCheckBox::stateChanged, this, [this](int state) {
        ui.brightnessSlider->setEnabled(state == Qt::Checked);
    });
    connect(ui.enableBlur, &QCheckBox::stateChanged, this, [this](int state) {
        ui.blurSlider->setEnabled(state == Qt::Checked);
    });

    // Connect checkboxes and sliders to functions that apply video effects.
    connect(ui.grayscaleCheckBox, &QCheckBox::stateChanged, this, &MainWindow::applyGrayscaleEffect);
    connect(ui.enableBrightness, &QCheckBox::stateChanged, this, &MainWindow::enableBrightnessAdjustment);
    connect(ui.brightnessSlider, &QSlider::valueChanged, this, &MainWindow::adjustBrightnessEffect);
    connect(ui.enableBlur, &QCheckBox::stateChanged, this, &MainWindow::enableBlurAdjustment);
    connect(ui.blurSlider, &QSlider::valueChanged, this, &MainWindow::applyBlurEffect);
}

// initializeVideoPlayer: Connects the GStreamer handler to the video player for frame updates.
void MainWindow::initializeVideoPlayer() {
    // Connect the newFrame signal from GStreamer to the setImage slot of the video player.
    connect(gstreamerHandler.get(), &GStreamerHandler::newFrame, videoPlayer.get(), &VideoPlayer::setImage);
    gstreamerHandler->startPipeline(); // Start the GStreamer pipeline for video streaming.
}

// Function implementations for enabling/disabling video effects based on UI interactions.
void MainWindow::applyGrayscaleEffect(int state) {
    if (videoPlayer)
        videoPlayer->setGrayscale(state == Qt::Checked); // Apply or remove grayscale effect based on checkbox state.
}

void MainWindow::enableBrightnessAdjustment(int state) {
    if (ui.brightnessSlider)
        ui.brightnessSlider->setEnabled(state == Qt::Checked); // Enable/disable brightness slider based on checkbox state.
    if (videoPlayer)
        videoPlayer->setBrightnessEnabled(state == Qt::Checked); // Enable/disable brightness effect in the video player.
}

void MainWindow::adjustBrightnessEffect(int value) {
    if (videoPlayer)
        videoPlayer->setBrightness(value); // Adjust video brightness based on slider value.
}

void MainWindow::enableBlurAdjustment(int state) {
    if (ui.blurSlider)
        ui.blurSlider->setEnabled(state == Qt::Checked); // Enable/disable blur slider based on checkbox state.
    if (videoPlayer)
        videoPlayer->setBlurEnabled(state == Qt::Checked); // Enable/disable blur effect in the video player.
}

void MainWindow::applyBlurEffect(int value) {
    if (videoPlayer)
        videoPlayer->setBlur(value); // Apply blur effect based on slider value.
}
