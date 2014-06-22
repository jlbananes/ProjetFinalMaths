#ifndef DRAWING_H
#define DRAWING_H

#include <math.h>
#include <iostream>

#include <QtQuick/qquickwindow.h>
#include <QtQuick/QQuickItem>
#include <QtCore/qhash.h>
#include <QtCore/qpair.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <QtOpenGL>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/qopengl.h>

#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions>
#include <QGLContext>

#include "Utils.h"

class Drawing : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal clickedButton READ clickedButton WRITE setClickedButton NOTIFY clickedButtonChanged)

public:
    Drawing();

    qreal t() const { return m_t; }
    qreal x() const { return m_x; }
    qreal y() const { return m_y; }
    qreal clickedButton() const { return m_clickedButton; }

    void setT(qreal t);
    void setX(qreal x);
    void setY(qreal y);
    void setClickedButton(qreal clickedButton);

    QOpenGLContext* m_context;
    QOpenGLFunctions_4_3_Core* m_funcs;
    //QOpenGLFunctions* m_funcs;
    //QAbstractOpenGLFunctions* m_funcs;

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

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    QOpenGLVertexArrayObject* m_vao0;
    QOpenGLShaderProgram *m_program;
    QMatrix4x4 pMatrix;
    QMatrix4x4 rotateCamera(QVector3D,QVector3D, int, int);
    void LoadMeshData(Mesh*);

    QMatrix4x4 totalRotationMatrix;
    QMatrix4x4 currentRotationMatrix;
    qreal tempXmove;
    qreal tempYmove;
    qreal currentXmove;
    qreal currentYmove;

    qreal m_t;
    qreal m_thread_t;
    qreal m_x, m_thread_x;
    qreal m_y, m_thread_y;
    qreal m_clickedButton, m_thread_clickedButton;

};

#endif // DRAWING_H
