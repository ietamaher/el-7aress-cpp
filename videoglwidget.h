#ifndef VIDEOGLWIDGET_H
#define VIDEOGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>

class VideoGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit VideoGLWidget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public slots:
    void updateFrame(const QImage &frame);

private:
    QImage currentFrame;
    GLuint textTexture; // OpenGL texture ID
    void createTextTexture(const QString &text);
    QOpenGLShaderProgram *shaderProgram;
    QOpenGLBuffer vertexBuffer, texCoordBuffer;




};

#endif // VIDEOGLWIDGET_H




