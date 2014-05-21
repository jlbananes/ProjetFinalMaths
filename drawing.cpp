#include "drawing.h"

#define highp
#define mediump
#define lowp

Drawing::Drawing()
    : m_program(0)
    , m_t(0)
    , m_thread_t(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    totalRotationMatrix.setToIdentity();
    currentRotationMatrix.setToIdentity();
    tempXmove = 0;
    tempYmove = 0;

    // Create an OpenGL context
    m_context = new QOpenGLContext;
    m_context->create();

    // Make the context current on this window
    //m_context->makeCurrent( this );
}

void Drawing::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void Drawing::setX(qreal x)
{
    if(x==m_x)
        return;
    m_x = x;
    emit xChanged();
    if (window())
        window()->update();
}

void Drawing::setY(qreal y)
{
    if(y==m_y)
        return;
    m_y = y;
    emit yChanged();
    if (window())
        window()->update();
}

void Drawing::setClickedButton(qreal c)
{
    if (c == m_clickedButton)
        return;
    m_clickedButton = c;
    emit clickedButtonChanged();
    if (window())
        window()->update();
}

QMatrix4x4 Drawing::rotateCamera(QVector3D cameraPosition, QVector3D cameraUpDirection, int nbPixelsX, int nbPixelsY)
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
    QVector3D horizontalAxis = QVector3D(1.0,.0,.0); //::crossProduct(cameraPosition,cameraUpDirection);
    x = horizontalAxis.x();
    y = horizontalAxis.y();
    z = horizontalAxis.z();
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

void Drawing::handleWindowChanged(QQuickWindow *win)
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

void Drawing::paint()
{

    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 5);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    GLint time;

    glGetIntegerv(GL_TIMESTAMP, &time);
    static const GLfloat lightpos[] = {.5, 1., 1., 0.};

    // tableau des vertices distincts
    GLfloat vertices[] =
    {
        -1.0,-1.0,-1.0, // vertex 0
        -1.0,-1.0, 1.0, // vertex 1
        -1.0, 1.0, 1.0, // vertex 2
        -1.0, 1.0,-1.0, // vertex 3
        1.0,-1.0, 1.0,  // vertex 4
        1.0,-1.0,-1.0,  // vertex 5
        1.0, 1.0,-1.0,  // vertex 6
        1.0, 1.0, 1.0,  // vertex 7

        -1.0,-1.0,-1.0, // couleur 0
        -1.0,-1.0, 1.0, // couleur 1
        -1.0, 1.0, 1.0, // couleur 2
        -1.0, 1.0,-1.0, // couleur 3
        1.0,-1.0, 1.0,  // couleur 4
        1.0,-1.0,-1.0,  // couleur 5
        1.0, 1.0,-1.0,  // couleur 6
        1.0, 1.0, 1.0,  // couleur 7
    };

    GLint indexes[] =
    {
        0,1,2,  // facette 0 (x=-1)
        0,2,3,  // facette 1
        4,0,5,  // facette 2 (y=-1)
        4,1,0,  // facette 3
        6,0,3,  // facette 4 (z=-1)
        6,5,0,  // facette 5
        7,5,6,  // facette 6 (x=1)
        5,7,4,  // facette 7
        7,6,3,  // facette 8 (y=1)
        7,3,2,  // facette 9
        2,1,7,  // facette 10 (z=1)
        7,1,4   // facette 11
    };

    /*GLfloat colors[] =
    {
        1.0, 0.0, 0.0,  // couleur 0 (rouge)
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,

        0.0, 1.0, 0.0,  // couleur 1 (vert)
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,

        0.0, 0.0, 1.0,  // couleur 2 (bleu)
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,

        0.0, 1.0, 1.0,  // couleur 3 (cyan)
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,
        0.0, 1.0, 1.0,

        1.0, 0.0, 1.0,  // couleur 4 (magenta)
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,

        1.0, 1.0, 0.0,  // couleur 5 (jaune)
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
    };*/

    /*GLint colorsIndex[] =
    {
        0,0,0,  // facette 0 (x=-1)
        0,0,0,  // facette 1
        1,1,1,  // facette 2 (y=-1)
        1,1,1,  // facette 3
        2,2,2,  // facette 4 (z=-1)
        2,2,2,  // facette 5
        3,3,3,  // facette 6 (x=1)
        3,3,3,  // facette 7
        4,4,4,  // facette 8 (y=1)
        4,4,4,  // facette 9
        5,5,5,  // facette 10 (z=1)
        5,5,5   // facette 11
    };*/

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

    //Resize
    pMatrix.setToIdentity();
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    //QMatrix4x4 newRotationMatrix;
    if (m_x!=currentXmove || m_y!=currentYmove)
    {
        //printf("clic !!\n");
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

    //glOrtho(-5.,5.,-5.,5.,.1,10.);
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
    //m_program->enableAttributeArray(0);
    m_program->setUniformValue("t", (float)m_thread_t);
    m_program->setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);

    // création du VAO
    QOpenGLVertexArrayObject* m_vao0 = new QOpenGLVertexArrayObject(this);
    m_vao0->create();
    m_vao0->bind();

    // initialisation des VBO
    QOpenGLBuffer m_positionBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    //QOpenGLBuffer m_colorBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    // initialisation des IBO
    QOpenGLBuffer m_positionIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    //QOpenGLBuffer m_colorIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);*/

    // VBO pour les vertices
    m_positionBuffer.create();
    m_positionBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_positionBuffer.bind();

    m_positionIndexBuffer.create();
    m_positionIndexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_positionIndexBuffer.bind();
    m_positionIndexBuffer.allocate( &indexes, 12 * 3 * sizeof( GLfloat ) );

    m_positionBuffer.allocate( &vertices, 16 * 3 * sizeof(GLfloat));
    m_program->enableAttributeArray("vertex");
    m_program->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
    m_program->enableAttributeArray("in_color");
    m_program->setAttributeBuffer("in_color", GL_FLOAT, 8 * 3 * sizeof(GLfloat), 3);

    /*m_program->enableAttributeArray("vertex");
    m_program->setAttributeBuffer("vertex", GL_FLOAT ,8 * 3 * sizeof( GLfloat ), 3);
    m_program->disableAttributeArray("vertex");*/
    //m_positionIndexBuffer.release();

    // VBO pour les couleurs
    /*m_colorBuffer.create();
    m_colorBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_colorBuffer.bind();
    m_colorBuffer.allocate( &colors, 12 * 3 * 3 * sizeof( GLfloat ) );
    m_program->enableAttributeArray("in_color");
    m_program->setAttributeBuffer("in_color", GL_FLOAT ,0, 3);*/
   // m_colorBuffer.release();

    /*m_colorIndexBuffer.create();
    //m_colorIndexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_colorIndexBuffer.bind();
    m_colorIndexBuffer.allocate( &colorsIndex, 12 * 3 * sizeof( GLfloat ) );
    m_program->enableAttributeArray("in_color");
    m_program->setAttributeBuffer("in_color", GL_FLOAT ,6 * 3 * sizeof( GLfloat ), 3);
    m_program->disableAttributeArray("in_color");*/

    //m_program->setUniformValue("color", QVector4D(1.0, 1.0, 0.0, 1.0));
    //m_program->setAttributeArray("normals", normals, 3);
    //m_program->enableAttributeArray("normals");

    m_vao0->bind();
    //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_SHORT, 0);
    //glDrawArrays(GL_TRIANGLES,0,36);

    glDrawElements(GL_TRIANGLES,12*3,GL_UNSIGNED_INT,0);
    //glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

    m_vao0->release();
    m_program->release();

    //m_vao0->release();

}

void Drawing::cleanup()
{
    if (m_program) {
        delete m_program;
        m_program = 0;
    }
}

void Drawing::sync()
{
    m_thread_t = m_t;
    m_thread_x = m_x;
    m_thread_y = m_y;
    m_thread_clickedButton = m_clickedButton;
}
