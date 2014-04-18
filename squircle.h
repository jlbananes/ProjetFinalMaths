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
    Q_PROPERTY(qreal clickedButton READ clickedButton WRITE setClickedButton NOTIFY clickedButtonChanged)

public:
    Squircle();

    qreal t() const { return m_t; }
    qreal x() const { return m_x; }
    qreal y() const { return m_y; }
    qreal clickedButton() const { return m_clickedButton; }

    void setT(qreal t);
    void setX(qreal x);
    void setY(qreal y);
    void setClickedButton(qreal clickedButton);

signals:
    void tChanged();
    void xChanged();
    void yChanged();
    void clickedButtonChanged();

public slots:
    //void render(double currentTime);
    void paint();
    void cleanup();
    void sync();
    void meuh();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    QOpenGLShaderProgram *m_program;
    QMatrix4x4 pMatrix;
    QMatrix4x4 rotateCamera(QVector3D,QVector3D, int, int);

    QMatrix4x4 totalRotationMatrix;
    QMatrix4x4 currentRotationMatrix;
    qreal tempXmove;
    qreal tempYmove;
    qreal currentXmove;
    qreal currentYmove;

    //QVector3D getNormal(QVector3D &p1, QVector3D &p2, QVector3D &p3);
    qreal m_t;
    qreal m_thread_t;
    qreal m_x, m_thread_x;
    qreal m_y, m_thread_y;
    qreal m_clickedButton, m_thread_clickedButton;

};

#endif // SQUIRCLE_H
