#include "videoglwidget_gl.h"


#include <QPainter>
#include <QOpenGLShader>

VideoGLWidget_gl::VideoGLWidget_gl(QWidget *parent)
    : QOpenGLWidget(parent), frameTexture(nullptr), textTexture(nullptr) {
}

VideoGLWidget_gl::~VideoGLWidget_gl() {
    makeCurrent();
    delete frameTexture;
    delete textTexture;
    delete program;
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &texCoordVBO);
    doneCurrent();
}

void VideoGLWidget_gl::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    qDebug() << "Initializing GL";

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);  // Set clear color

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create text texture
    createTextTexture("Hello ChatGPT");
}

void VideoGLWidget_gl::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void VideoGLWidget_gl::paintGL() {
    //qDebug() << "Painting GL";
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(-1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(0, 1); glVertex2f(-1, 1);
    glEnd();

    glDisable(GL_TEXTURE_2D);

}

void VideoGLWidget_gl::updateFrame(const QImage &frame) {
    //qDebug() << "Updating frame";
    // Ensure the new frame is in the correct format
    makeCurrent();  // Ensure the OpenGL context is current

    QImage glFrame = frame.convertToFormat(QImage::Format_RGB888).mirrored();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glFrame.width(), glFrame.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, glFrame.bits());

    update();  // Schedule a repaint
}

void VideoGLWidget_gl::createTextTexture(const QString &text) {
    // Similar to your existing implementation, but create an OpenGL texture
}

void VideoGLWidget_gl::setupVertexAttribs() {
    // Generate and bind vertex and texture coordinate buffers
}

