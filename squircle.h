#ifndef SQUIRCLE_H
#define SQUIRCLE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtOpenGL>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Squircle : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)

public:
    Squircle();

    qreal t() const { return m_t; }
    qreal x() const { return m_x; }
    qreal y() const { return m_y; }
    void setT(qreal t);
    void setX(qreal x);
    void setY(qreal y);

signals:
    void tChanged();
    void xChanged();
    void yChanged();

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
    //QVector3D getNormal(QVector3D &p1, QVector3D &p2, QVector3D &p3);
    qreal m_t;
    qreal m_thread_t;
    qreal m_x, m_thread_x;
    qreal m_y, m_thread_y;
};

#endif // SQUIRCLE_H
