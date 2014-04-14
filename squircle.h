#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtOpenGL>
#include <math.h>
#include <GL/gl.h>
#include <GL/glaux.h>
#include <GL/glu.h>

class Squircle : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    Squircle();

    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();

public slots:
    //void render(double currentTime);
    void paint();
    void cleanup();
    void sync();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    QOpenGLShaderProgram *m_program;
    QMatrix4x4 pMatrix;
    QVector3D getNormal(QVector3D &p1, QVector3D &p2, QVector3D &p3);
    qreal m_t;
    qreal m_thread_t;
};



#endif // SQUIRCLE_H
