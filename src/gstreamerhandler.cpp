#include "gstreamerhandler.h"

// Constructor: Initializes GStreamer and sets up the object within a Qt parent-child hierarchy.
GStreamerHandler::GStreamerHandler(QObject* parent) : QObject(parent) {
    gst_init(nullptr, nullptr); 
}

// Destructor: Ensures the GStreamer pipeline is stopped properly to free resources.
GStreamerHandler::~GStreamerHandler() {
    stopPipeline(); 
}

// startPipeline: Configures and starts the GStreamer pipeline for video processing.
void GStreamerHandler::startPipeline() {
    // Pipeline configuration string.
    const char* pipelineStr = "mfvideosrc device-index=0 ! videoconvert ! video/x-raw,format=RGB ! appsink name=sink";
    GError* error = nullptr;
    // Creates the pipeline from the configuration string and checks for errors.
    m_pipeline.reset(gst_parse_launch(pipelineStr, &error));
    if (!m_pipeline) {
        qDebug() << "Failed to create pipeline: " << error->message;
        g_error_free(error); // Frees the error object if pipeline creation failed.
        return;
    }

    // Retrieves the appsink element by its name to configure it and connect signals.
    m_sink = gst_bin_get_by_name(GST_BIN(m_pipeline.get()), "sink");
    if (!m_sink) {
        qDebug() << "Failed to find sink element"; // Error handling if the sink element is not found.
        return;
    }

    // Configure the appsink to emit signals for new samples and operate asynchronously.
    g_object_set(m_sink, "emit-signals", TRUE, "sync", FALSE, nullptr);
    // Connects the "new-sample" signal of the appsink to the newFrameCallback function.
    g_signal_connect(m_sink, "new-sample", G_CALLBACK(+[](GstAppSink* sink, gpointer data) -> GstFlowReturn {
        auto* handler = static_cast<GStreamerHandler*>(data); // Casts the user data back to a GStreamerHandler pointer.
        return handler->newFrameCallback(sink, handler); // Calls the member function to handle the new sample.
    }), this);

    // Sets the pipeline state to PLAYING, starting the video capture and processing.
    gst_element_set_state(m_pipeline.get(), GST_STATE_PLAYING);
}

// stopPipeline: Stops the GStreamer pipeline, transitioning it to the NULL state.
void GStreamerHandler::stopPipeline() {
    if (m_pipeline) {
        gst_element_set_state(m_pipeline.get(), GST_STATE_NULL); // Sets the pipeline state to NULL, stopping it.
    }
}

// Callback function called by GStreamer when a new frame is available in the appsink.
GstFlowReturn GStreamerHandler::newFrameCallback(GstAppSink* appsink, gpointer data) {
    auto* handler = static_cast<GStreamerHandler*>(data); // Casts the gpointer back to the GStreamerHandler instance.
    auto* sample = gst_app_sink_pull_sample(appsink); // Pulls the sample from the appsink.
    if (!sample) {
        return GST_FLOW_ERROR; // Returns an error if no sample is available.
    }

    // If a sample is available, convert it to QImage and emit the newFrame signal.
    if (sample) {
        QImage frame = convertToQImage(sample); // Converts the GstSample to QImage.
        gst_sample_unref(sample); // Unreferences the sample after processing.

        emit handler->newFrame(frame); // Emits the newFrame signal with the QImage.
    }

    return GST_FLOW_OK; // Indicates successful processing of the sample.
}

// Converts a GstSample to a QImage for use in Qt applications.
QImage GStreamerHandler::convertToQImage(GstSample* sample) {
    auto* buffer = gst_sample_get_buffer(sample); // Retrieves the buffer from the sample.
    GstMapInfo map;
    // Maps the buffer to access its data.
    if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
        auto* caps = gst_sample_get_caps(sample); // Retrieves the capabilities of the sample.
        auto* structure = gst_caps_get_structure(caps, 0); // Retrieves the structure from the caps.

        int width, height;
        // Extracts the width and height of the video frame from the caps structure.
        gst_structure_get_int(structure, "width", &width);
        gst_structure_get_int(structure, "height", &height);

        // Creates a QImage from the buffer data with the specified width, height, and format.
        QImage image(map.data, width, height, QImage::Format_RGB888);
        gst_buffer_unmap(buffer, &map); // Unmaps the buffer after accessing its data.

        return image; // Returns the constructed QImage.
    }
    return QImage(); // Returns an empty QImage if the buffer could not be mapped.
}
