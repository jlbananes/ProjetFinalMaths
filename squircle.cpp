#include "squircle.h"

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>

#define highp
#define mediump
#define lowp

Squircle::Squircle()
    : m_program(0)
    , m_t(0)
    , m_thread_t(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    totalRotationMatrix.setToIdentity();
    currentRotationMatrix.setToIdentity();
    tempXmove = 0;
    tempYmove = 0;
}

void Squircle::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void Squircle::setX(qreal x)
{
    if(x==m_x)
        return;
    m_x = x;
    emit xChanged();
    if (window())
        window()->update();
}

void Squircle::setY(qreal y)
{
    if(y==m_y)
        return;
    m_y = y;
    emit yChanged();
    if (window())
        window()->update();
}

void Squircle::setClickedButton(qreal c)
{
    if (c == m_clickedButton)
        return;
    m_clickedButton = c;
    emit clickedButtonChanged();
    if (window())
        window()->update();
}

QMatrix4x4 Squircle::rotateCamera(QVector3D cameraPosition, QVector3D cameraUpDirection, int nbPixelsX, int nbPixelsY)
{
    QMatrix4x4 rotationMatrix;
    rotationMatrix.setToIdentity();

    float Yangle = atan2(nbPixelsX/20.,cameraPosition.length());

    float c = cos(Yangle);  float s = sin(Yangle);  float C = 1-c;
    float x = cameraUpDirection.x();
    float y = cameraUpDirection.y();
    float z = cameraUpDirection.z();

    float xs = x*s;   float ys = y*s;   float zs = z*s;
    float xC = x*C;   float yC = y*C;   float zC = z*C;
    float xyC = x*yC; float yzC = y*zC; float zxC = z*xC;

    // matrice de rotation autour de l'axe Y de l'écran (vertical)
    QMatrix4x4 rotationMatrixY = QMatrix4x4(x*xC+c,    xyC-zs, zxC+ys,     0.0,
                                 xyC+zs,    y*yC+c, yzC-xs,     0.0,
                                 zxC-ys,    yzC+xs, z*zC+c ,    0.0,
                                 0.0,       0.0,    0.0,        1.0);


    /*c = cos(θ); s = sin(θ); C = 1-c
    xs = x*s;   ys = y*s;   zs = z*s
    xC = x*C;   yC = y*C;   zC = z*C
    xyC = x*yC; yzC = y*zC; zxC = z*xC
    [ x*xC+c   xyC-zs   zxC+ys ]
    [ xyC+zs   y*yC+c   yzC-xs ]
    [ zxC-ys   yzC+xs   z*zC+c ]*/


    float Xangle = atan2(nbPixelsY/20.,cameraPosition.length());
    c = cos(Xangle);  s = sin(Xangle);  C = 1-c;
    QVector3D horizontalAxis = QVector3D::crossProduct(cameraPosition,cameraUpDirection);
    /*x = horizontalAxis.x();
    y = horizontalAxis.y();
    z = horizontalAxis.z();*/
    x = -1; y=0; z=0;
    xs = x*s;    ys = y*s;    zs = z*s;
    xC = x*C;    yC = y*C;    zC = z*C;
    xyC = x*yC;  yzC = y*zC;  zxC = z*xC;

    // matrice de rotation autour de l'axe X de l'écran (horizontal)
    QMatrix4x4 rotationMatrixX = QMatrix4x4(x*xC+c,    xyC-zs, zxC+ys,     0.0,
                                 xyC+zs,    y*yC+c, yzC-xs,     0.0,
                                 zxC-ys,    yzC+xs, z*zC+c ,    0.0,
                                 0.0,       0.0,    0.0,        1.0);

    return rotationMatrix * rotationMatrixY * rotationMatrixX;
}

void Squircle::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        // Connect the beforeRendering signal to our paint function.
        // Since this call is executed on the rendering thread it must be
        // a Qt::DirectConnection
        connect(win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);

        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void Squircle::paint()
{
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 5);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    GLint time;

    glGetIntegerv(GL_TIMESTAMP, &time);
    static const GLfloat lightpos[] = {.5, 1., 1., 0.};
    GLfloat vertices[] =
    {
        // face x=-1
        -1.0,-1.0,-1.0, // triangle 1 : begin
        -1.0,-1.0, 1.0,
        -1.0, 1.0, 1.0, // triangle 1 : end

        -1.0,-1.0,-1.0,
        -1.0, 1.0, 1.0,
        -1.0, 1.0,-1.0,

        // face y=-1
        1.0,-1.0, 1.0,
        -1.0,-1.0,-1.0,
        1.0,-1.0,-1.0,

        1.0,-1.0, 1.0,
        -1.0,-1.0, 1.0,
        -1.0,-1.0,-1.0,

        // face z=-1
        1.0, 1.0,-1.0,
        -1.0,-1.0,-1.0,
        -1.0, 1.0,-1.0,

        1.0, 1.0,-1.0,
        1.0,-1.0,-1.0,
        -1.0,-1.0,-1.0,

        // face x=1
        1.0, 1.0, 1.0,
        1.0,-1.0,-1.0,
        1.0, 1.0,-1.0,

        1.0,-1.0,-1.0,
        1.0, 1.0, 1.0,
        1.0,-1.0, 1.0,

        // face y=1
        1.0, 1.0, 1.0,
        1.0, 1.0,-1.0,
        -1.0, 1.0,-1.0,

        1.0, 1.0, 1.0,
        -1.0, 1.0,-1.0,
        -1.0, 1.0, 1.0,

        // face z=1
        -1.0, 1.0, 1.0,
        -1.0,-1.0, 1.0,
        1.0,-1.0, 1.0,

        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        1.0,-1.0, 1.0,

    };

    GLfloat colors[] =
    {
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,

        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,

        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,

        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,

        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,

        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,

        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,

        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,

        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,

        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,

        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,

        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0
    };

    //GLfloat normals[12] = {0};

    /*int j = 0;
    for(int i = 0; i < 12*3; i+=9)
    {
        QVector3D p1 = QVector3D(values[i], values[i+1], values[i+2]);
        QVector3D p2 = QVector3D(values[i+3], values[i+4], values[i+5]);
        QVector3D p3 = QVector3D(values[i+6], values[i+7], values[i+8]);

        QVector3D n = getNormal(p1, p2, p3);
        normals[j++] = (GLfloat)n.x();
        normals[j++] = (GLfloat)n.y();
        normals[j++] = (GLfloat)n.z();
    }*/

    if (!m_program)
    {
        /* D:\\Workspace */
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "..\\..\\ProjetFinalMaths\\Shaders\\VertexShader.vert");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "..\\..\\ProjetFinalMaths\\Shaders\\FragmentShader.frag");
        m_program->bindAttributeLocation("gl_Vertex", 0);
        m_program->link();

        connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()), this, SLOT(cleanup()), Qt::DirectConnection);
    }

    m_program->bind();

    m_program->enableAttributeArray(0);

    //Resize
    pMatrix.setToIdentity();
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    //QMatrix4x4 newRotationMatrix;
    if (m_x!=currentXmove || m_y!=currentYmove)
    {
        printf("clic !!\n");
        currentRotationMatrix = rotateCamera(cameraPosition, cameraUpDirection, m_x, m_y);
        currentXmove = m_x;
        currentYmove = m_y;
    }
    else //if (clic relaché)
    {
        totalRotationMatrix = totalRotationMatrix * currentRotationMatrix;
        currentRotationMatrix.setToIdentity();
    }
    vMatrix = vMatrix * totalRotationMatrix * currentRotationMatrix;

    qreal ratio = window()->devicePixelRatio();
    int w = int(ratio * window()->width());
    int h = int(ratio * window()->height());
    pMatrix.perspective(60.0, (double) w / (double) h, 0.001, 1000);

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION|GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDisable(GL_BLEND);

    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    glClearColor(0., 0., 0., 1.);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    m_program->setUniformValue("t", (float)m_thread_t);
    m_program->setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);
    //m_program->setUniformValue("color", QVector4D(1.0, 1.0, 0.0, 1.0));

    //m_program->setAttributeArray("normals", normals, 3);
    //m_program->enableAttributeArray("normals");

    m_program->setAttributeArray("vertex", vertices, 3);
    m_program->enableAttributeArray("vertex");

    m_program->setAttributeArray("in_color", colors, 3);
    m_program->enableAttributeArray("in_color");

    glDrawArrays(GL_TRIANGLES, 0, 12*3);

    //printf("x = %f y = %f", m_x, m_y);

    //m_program->disableAttributeArray("normals");
    m_program->disableAttributeArray("in_color");
    m_program->disableAttributeArray("vertex");
    m_program->release();
    //*/
}

/*QVector3D Squircle::getNormal(QVector3D &p1, QVector3D &p2, QVector3D &p3)
{
    QVector3D U = QVector3D(p2.x()-p1.x(), p2.y()-p1.y(), p2.z()-p1.z());
    QVector3D V = QVector3D(p3.x()-p1.x(), p3.y()-p1.y(), p3.z()-p1.z());

    QVector3D Normal;
    Normal.setX((U.y()*V.z()) - (U.z()*V.y()));
    Normal.setY((U.z()*V.x()) - (U.x()*V.z()));
    Normal.setZ((U.x()*V.y()) - (U.y()*V.z()));

    return Normal;
}*/

void Squircle::cleanup()
{
    if (m_program) {
        delete m_program;
        m_program = 0;
    }
}

void Squircle::meuh()
{
    return;
}

void Squircle::sync()
{
    m_thread_t = m_t;
    m_thread_x = m_x;
    m_thread_y = m_y;
    m_thread_clickedButton = m_clickedButton;
}
