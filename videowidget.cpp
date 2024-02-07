#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent) : QOpenGLWidget(parent), texture(nullptr) {
    // Initialization
}

VideoWidget::~VideoWidget() {
    // Clean up
}

void VideoWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set clear color

    program = new QOpenGLShaderProgram(this);
    // Setup shaders, etc.
}

void VideoWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind and draw texture
    // You'll update this texture in a slot connected to your video feed thread
}

void VideoWidget::resizeGL(int w, int h) {
    // Handle window resizing if necessary
}
