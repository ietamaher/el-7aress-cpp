#ifndef VIDEOGLWIDGET_GL_H
#define VIDEOGLWIDGET_GL_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QImage>

class VideoGLWidget_gl : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit VideoGLWidget_gl(QWidget *parent = nullptr);
    ~VideoGLWidget_gl();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

public slots:
    void updateFrame(const QImage &frame);

private:
    void createTextTexture(const QString &text);
    void setupVertexAttribs();

    QImage currentFrame;
    QOpenGLTexture *frameTexture = nullptr;
    QOpenGLTexture *textTexture = nullptr;
    QOpenGLShaderProgram *program = nullptr;
    GLuint vertexVBO, texCoordVBO;
    GLuint textureID;
};


#endif // VIDEOGLWIDGET_GL_H
