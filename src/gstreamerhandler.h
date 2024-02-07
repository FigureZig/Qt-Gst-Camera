#pragma once

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <QObject>
#include <QImage>
#include <QDebug>
#include <memory>

class GStreamerHandler : public QObject {
    Q_OBJECT

public:
    explicit GStreamerHandler(QObject* parent = nullptr);
    ~GStreamerHandler() override;

    void startPipeline();
    void stopPipeline();

signals:
    void newFrame(QImage frame);

private:
    std::unique_ptr<GstElement, decltype(&gst_object_unref)> m_pipeline{ nullptr, gst_object_unref };
    GstElement* m_sink = nullptr;

    static GstFlowReturn newFrameCallback(GstAppSink* appsink, gpointer user_data);
    static QImage convertToQImage(GstSample* sample);
};