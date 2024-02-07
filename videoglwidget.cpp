#include "videoglwidget.h"
#include <QPainter>

VideoGLWidget::VideoGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void VideoGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1); // Set default clear color

    createTextTexture("Hello ChatGPT"); // Create the text texture
}

void VideoGLWidget::createTextTexture(const QString &text) {
    // Render the text to a QImage
    QImage textImage(256, 256, QImage::Format_RGBA8888);
    textImage.fill(Qt::transparent); // Start with a transparent background
    QPainter painter(&textImage);
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 24));
    painter.drawText(textImage.rect(), Qt::AlignCenter, text);
    painter.end();

    // Convert the QImage to an OpenGL texture
    //if (textTexture) delete textTexture;
    //textTexture = new QOpenGLTexture(textImage);
}


void VideoGLWidget::resizeGL(int w, int h) {
    // Update projection matrix and other size related settings:
    glViewport(0, 0, w, h);
}

void VideoGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
    if (!currentFrame.isNull()) {
        // Draw the frame
        QImage scaledFrame = currentFrame.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        int x = (this->width() - scaledFrame.width()) / 2;
        int y = (this->height() - scaledFrame.height()) / 2;
        painter.drawImage(QPoint(x, y), scaledFrame);
    }
    // Set the painter for drawing the text
    painter.setPen(Qt::white); // Set text color
    painter.setFont(QFont("Arial", 20)); // Set font and size

    // Draw the text
    painter.drawText(rect(), Qt::AlignCenter, "Hello ChatGPT");

    painter.end();
}

void VideoGLWidget::updateFrame(const QImage &frame) {
    currentFrame = frame;
    update(); // Trigger paint event
}
