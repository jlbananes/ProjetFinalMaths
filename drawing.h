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
#include "importmesh.h"

class Drawing : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal z READ z WRITE setZ NOTIFY zChanged)
    Q_PROPERTY(int mode READ mode WRITE setMODE NOTIFY modeChanged)
    Q_PROPERTY(QString path READ path WRITE setPATH NOTIFY pathChanged)
    Q_PROPERTY(qreal level READ level WRITE setLEVEL NOTIFY levelChanged)
    Q_PROPERTY(qreal clickedButton READ clickedButton WRITE setClickedButton NOTIFY clickedButtonChanged)

public:
    Drawing();

    qreal t() const { return m_t; }
    qreal x() const { return m_x; }
    qreal y() const { return m_y; }
    qreal z() const { return m_z; }
    int mode() const { return m_mode; }
    QString path() const { return m_path; }
    qreal level() const { return m_level; }
    qreal clickedButton() const { return m_clickedButton; }

    void setT(qreal t);
    void setX(qreal x);
    void setY(qreal y);
    void setZ(qreal z);
    void setMODE(int mode);
    void setPATH(QString path);
    void setLEVEL(qreal level);
    void setClickedButton(qreal clickedButton);

    QOpenGLContext* m_context;
    QOpenGLFunctions_4_3_Core* m_funcs;
    //QOpenGLFunctions* m_funcs;
    //QAbstractOpenGLFunctions* m_funcs;

signals:
    void tChanged();
    void xChanged();
    void yChanged();
    void zChanged();
    void modeChanged();
    void pathChanged();
    void levelChanged();
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
    void changeFile();

    QMatrix4x4 totalRotationMatrix;
    QMatrix4x4 currentRotationMatrix;
    qreal tempXmove;
    qreal tempYmove;
    qreal tempZmove;
    qreal currentXmove;
    qreal currentYmove;
    unsigned int objectsOnScene;

    qreal m_t;
    qreal m_thread_t;
    qreal m_x, m_thread_x;
    qreal m_y, m_thread_y;
    qreal m_z, m_thread_z;
    int m_mode, m_thread_mode;
    QString m_path, m_thread_path;
    qreal m_level, m_thread_level;
    qreal m_clickedButton, m_thread_clickedButton;

    vector<QVector3D> _vertices;
    vector<QVector2D> _uvs;
    vector<QVector3D> _normals;
    vector<unsigned int> _vertexIndices;
    vector<unsigned int> _uvIndices;
    vector<unsigned int> _normalIndices;
    vector<GLfloat> _verFloat;
    vector<GLfloat> _uvsFloat;
    vector<GLfloat> _norFloat;
};

#endif // DRAWING_H
