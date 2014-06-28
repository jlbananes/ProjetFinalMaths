#include "drawing.h"

#define highp
#define mediump
#define lowp

using namespace Utils;

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

    importMesh iMesh;
    if(iMesh.import("D:\\Workspace\\ProjetFinalMaths\\Assets\\sphere.obj",
                 _vertices, _uvs, _normals,
                 _vertexIndices, _uvIndices, _normalIndices))
    {
        cout << "lecture ok" << endl;
    }
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

// chargement des données du mesh dans les tableaux utilisés par la fonction paint()
/*void Drawing::LoadMeshData(Mesh* mesh, GLfloat[] &_vertices, GLfloat[] &_normals, GLfloat[] &_texCoords, GLuint[] &_indexes)
{
    for (unsigned int i=0; i<mesh->getSurfaces().size(); i++)
    {
        for (unsigned int j=0; j<mesh->getSurfaces()[i]->getFaces().size(); j++)
        {
            return;
        }
    }
}*/

void Drawing::paint()
{
    unsigned int i = 0;
    unsigned int el = 0;
    unsigned int id = 0;
    unsigned int concombre = _vertices.size()*4;
    GLfloat vect3[_vertices.size()*3];
    for(vector<QVector3D>::iterator it = _vertices.begin(); it != _vertices.end(); ++it)
    {
        QVector3D v = *it;
        vect3[i++] = v.x();
        vect3[i++] = v.y();
        vect3[i++] = v.z();
        el+=3;
    }
    i = 0;
    GLuint idex3[_vertexIndices.size()];
    for(vector<unsigned int>::iterator it = _vertexIndices.begin(); it != _vertexIndices.end(); ++it)
    {
        idex3[i++] = (*it)-1;
        id++;
    }
    /*
    cout << el << endl;
    cout << id << endl;
    for(unsigned int i = 0; i < el; i+=3)
    {
        cout << vect3[i] << " " << vect3[i+1] << " " << vect3[i+2] << endl;
    }
    cout << endl;
    for(unsigned int i = 0; i < id; i+=3)
    {
        cout << idex3[i] << " " << idex3[i+1] << " " << idex3[i+2] << endl;
    }
    //*/
    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 10);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    GLint time;

    glGetIntegerv(GL_TIMESTAMP, &time);
    static const GLfloat lightpos[] = {.5, 1., 1., 0.};

    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;

    //Resize
    pMatrix.setToIdentity();
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    if (m_x!=currentXmove || m_y!=currentYmove)
    {
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
    GLfloat light_position[4] = {70.0, 70.0, 70.0, 0.1};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,1);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    //glLightfv(GL_LIGHT0, GL_POSITION, lightpos);y
    glClearColor(0, 1, 1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (!m_program)
    {
        /* D:\\Workspace //*/
        m_program = new QOpenGLShaderProgram();
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/VertexShader");
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/FragmentShader");
        m_program->bindAttributeLocation("gl_Vertex", 0);
        m_program->link();

        connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()), this, SLOT(cleanup()), Qt::DirectConnection);
    }
    m_program->bind();
    m_program->setUniformValue("t", (float)m_thread_t);
    m_program->setUniformValue("mvpMatrix", pMatrix * vMatrix * mMatrix);


    // tableaux de données du mesh
    GLuint nbOfIndexes = 24;
    //GLfloat* vertices = new GLfloat[nbOfIndexes*3];
    //GLfloat* colors = new GLfloat[nbOfIndexes*4];
    //GLfloat* normals = new GLfloat[nbOfIndexes*3];
    //GLfloat* texCoords = new GLfloat[nbOfIndexes*2];
    //GLushort* indexes = new GLushort[nbOfIndexes];

    /*/ tableau des vertices distincts (position, couleur et normale unique)
    for (unsigned int i=0; i<nbOfIndexes*3; i=i+3)
    {
         vertices[i] = -1.0;
         vertices[i+1] = 1.0;
         vertices[i+2] = -1.0;
    }
    //*/
    /*/ tableau des couleurs
    GLfloat alpha = 0.8;
    for (unsigned int i=0; i<nbOfIndexes*4; i=i+4)
    {
         colors[i] = 1.0;
         colors[i+1] = 1.0;
         colors[i+2] = 1.0;
         colors[i+3] = alpha;
    }

    // tableau des normales
    for (unsigned int i=0; i<nbOfIndexes*3; i=i+3)
    {
         normals[i] = 1.0;
         normals[i+1] = 1.0;
         normals[i+2] = 1.0;
    }

    // tableau des coordonnées de texture
    for (unsigned int i=0; i<nbOfIndexes*2; i=i+2)
    {
         texCoords[i] = 0.5;
         texCoords[i+1] = 0.5;
    }

    // tableau des indices
    for (unsigned int i=0; i<nbOfIndexes; i++)
    {
         indexes[i] = i;
    }
    //*/
    // tableaux de données statiques
    //*
    GLfloat vertices[] =
    {
        1.000000, -1.000000, -1.000000,
        1.000000, -1.000000, 1.000000,
        -1.000000, -1.000000, 1.000000,
        -1.000000, -1.000000, -1.000000,
        1.000000, 1.000000, -0.999999,
        0.999999, 1.000000, 1.000001,
        -1.000000, 1.000000, 1.000000,
        -1.000000, 1.000000, -1.000000

        //-1.0,-1.0,-1.0, // vertex 0
        //-1.0,-1.0, 1.0, // vertex 1
        //-1.0, 1.0, 1.0, // vertex 2
        //-1.0, 1.0,-1.0, // vertex 3
        //1.0,-1.0, 1.0,  // vertex 4
        //1.0,-1.0,-1.0,  // vertex 5
        //1.0, 1.0,-1.0,  // vertex 6
        //1.0, 1.0, 1.0,  // vertex 7

        /*
        -1.0,-1.0,-1.0, // vertex 0     // face (x=-1)
        -1.0,-1.0, 1.0, // vertex 1
        -1.0, 1.0, 1.0, // vertex 2
        -1.0, 1.0,-1.0, // vertex 3

        1.0,-1.0, 1.0,  // vertex 4     //face (y=-1)
        -1.0,-1.0,-1.0, // vertex 0
        1.0,-1.0,-1.0,  // vertex 5
        -1.0,-1.0, 1.0, // vertex 1

        1.0, 1.0,-1.0,  // vertex 6     //face (z=-1)
        -1.0,-1.0,-1.0, // vertex 0
        -1.0, 1.0,-1.0, // vertex 3
        1.0,-1.0,-1.0,  // vertex 5

        1.0, 1.0, 1.0,  // vertex 7     //face (x=1)
        1.0,-1.0,-1.0,  // vertex 5
        1.0, 1.0,-1.0,  // vertex 6
        1.0,-1.0, 1.0,  // vertex 4

        1.0, 1.0, 1.0,  // vertex 7     //face (y=1)
        1.0, 1.0,-1.0,  // vertex 6
        -1.0, 1.0,-1.0, // vertex 3
        -1.0, 1.0, 1.0, // vertex 2

        -1.0, 1.0, 1.0, // vertex 2     //face (z=1)
        -1.0,-1.0, 1.0, // vertex 1
        1.0, 1.0, 1.0,  // vertex 7
        1.0,-1.0, 1.0,  // vertex 4
        //*/
    };
    //*/
    GLfloat alpha = 1.0;
    GLfloat colors[_vertices.size()*4];
    for(unsigned int i = 0; i < _vertices.size()*4; i+=4)
    {
        colors[i] = 1.0;
        colors[i+1] = 0.0;
        colors[i+2] = 0.0;
        colors[i+3] = alpha;
    }
    /*
    GLfloat alpha = 0.8;
    GLfloat colors[] =
    {
        1.0,0.0,0.0,alpha, // couleur 0 (rouge)
        1.0,0.0,0.0,alpha,
        1.0,0.0,0.0,alpha,
        1.0,0.0,0.0,alpha,

        0.0,0.0,1.0,alpha, // couleur 1 (bleu)
        0.0,0.0,1.0,alpha,
        0.0,0.0,1.0,alpha,
        0.0,0.0,1.0,alpha,

        0.0,1.0,0.0,alpha, // couleur 2 (vert)
        0.0,1.0,0.0,alpha,
        0.0,1.0,0.0,alpha,
        0.0,1.0,0.0,alpha,

        1.0,0.0,0.0,alpha, // couleur 0 (rouge)
        1.0,0.0,0.0,alpha,
        1.0,0.0,0.0,alpha,
        1.0,0.0,0.0,alpha,

        0.0,0.0,1.0,alpha, // couleur 1 (bleu)
        0.0,0.0,1.0,alpha,
        0.0,0.0,1.0,alpha,
        0.0,0.0,1.0,alpha,

        0.0,1.0,0.0,alpha, // couleur 2 (vert)
        0.0,1.0,0.0,alpha,
        0.0,1.0,0.0,alpha,
        0.0,1.0,0.0,alpha,
    };
    //*/
    //*
    GLfloat normals[] =
    {
        //*
        -1.0,0.0,0.0,   // face (x=-1)
        -1.0,0.0,0.0,
        -1.0,0.0,0.0,
        -1.0,0.0,0.0,

        0.0,-1.0,0.0,   // face (y=-1)
        0.0,-1.0,0.0,
        0.0,-1.0,0.0,
        0.0,-1.0,0.0,

        0.0,0.0,-1.0,   // face (z=-1)
        0.0,0.0,-1.0,
        0.0,0.0,-1.0,
        0.0,0.0,-1.0,

        1.0,0.0,0.0,   // face (x=1)
        1.0,0.0,0.0,
        1.0,0.0,0.0,
        1.0,0.0,0.0,

        0.0,1.0,0.0,   // face (y=1)
        0.0,1.0,0.0,
        0.0,1.0,0.0,
        0.0,1.0,0.0,

        0.0,0.0,1.0,   // face (z=1)
        0.0,0.0,1.0,
        0.0,0.0,1.0,
        0.0,0.0,1.0,
        //*/
    };
    //*/
    //*
    GLfloat texCoords[] =
    {
        0.0,0.33,   // face (x=-1)
        0.25,0.33,
        0.25,0.67,
        0.0,0.67,

        0.5,0.33,   // face (y=-1)
        0.25,0.0,
        0.5,0.0,
        0.25,0.33,

        0.75,0.67,  // face (z=-1)
        1.0,0.33,
        1.0,0.67,
        0.75,0.33,

        0.5,0.67,   // face (x=1)
        0.75,0.33,
        0.75,0.67,
        0.5,0.33,

        0.5,0.67,   // face (y=1)
        0.5,1.0,
        0.25,1.0,
        0.25,0.67,

        0.25,0.67,  // face (z=1)
        0.25,0.33,
        0.5,0.67,
        0.5,0.33,
    };
    //*/
    //*
    GLuint indexes[] =
    {
        0, 1, 2,
        4, 7, 6,
        0, 4, 5,
        1, 5, 6,
        2, 6, 7,
        4, 0, 3,
        3, 0, 2,
        5, 4, 6,
        1, 0, 5,
        2, 1, 6,
        3, 2, 7,
        7, 4, 3
        /*
        0,1,2,      // face (x=-1)
        0,2,3,
        4,5,6,      // face (y=-1)
        4,7,5,
        8,9,10,     // face (z=-1)
        8,11,9,
        12,13,14,   // face (x=1)
        13,12,15,
        16,17,18,   // face (y=1)
        16,18,19,
        20,21,22,   // face (z=1)
        22,21,23
        //*/
    };
    //*/

    m_program->bind();
    // création du VAO
    m_vao0 = new QOpenGLVertexArrayObject(this);
    m_vao0->create();
    m_vao0->bind();

    // initialisation des VBO
    QOpenGLBuffer m_vertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_colorBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_texCoordBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    QOpenGLBuffer m_normalBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);

    // initialisation de l'Index Buffer Object
    QOpenGLBuffer m_indexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    // Index Buffer Object
    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_indexBuffer.bind();
    m_indexBuffer.allocate( &idex3, id * sizeof( GLuint) );

    // VBO pour les vertices
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate( &vect3, el * sizeof(GLfloat) );
    m_program->enableAttributeArray("vertex");
    m_program->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
    m_vertexBuffer.release();

    // VBO pour les couleurs
    m_colorBuffer.create();
    m_colorBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_colorBuffer.bind();
    m_colorBuffer.allocate( &colors, concombre * sizeof(GLfloat));
    m_program->enableAttributeArray("in_color");
    m_program->setAttributeBuffer("in_color", GL_FLOAT, 0, 4);
    m_colorBuffer.release();
    //*/

    /*/ VBO pour les normales
    m_normalBuffer.create();
    m_normalBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_normalBuffer.bind();
    m_normalBuffer.allocate( &normals, 24 * 3 * sizeof(GLfloat));
    m_program->enableAttributeArray("in_normal");
    m_program->setAttributeBuffer("in_normal", GL_FLOAT, 0, 3);
    m_normalBuffer.release();
    //*/
    /*/
    //QPixmap texture = QPixmap(QString("..\\..\\ProjetFinalMaths\\Textures\\texture_caisse_recadree.jpg"));
    //GLuint texture = m_context->functions()->glActiveTexture(GL_TEXTURE0);
    GLuint texID = m_context->bindTexture(QPixmap(QString("..\\..\\ProjetFinalMaths\\Textures\\texture_caisse_recadree.jpg")));
    //textures[0] = texID;
    glBindTexture(GL_TEXTURE_2D,texID);*/
    //*/

    /*/ Prepare texture
    QOpenGLTexture *texture = new QOpenGLTexture(QImage("..\\..\\ProjetFinalMaths\\Textures\\texture_caisse_recadree.jpg").mirrored());
    //texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    //texture->setMagnificationFilter(QOpenGLTexture::Linear);
    // Render with texture
    texture->bind(0);
    glBindTexture(GL_TEXTURE_2D,0);
    //*/
    /*/ VBO pour les coordonnées de texture
    m_texCoordBuffer.create();
    m_texCoordBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_texCoordBuffer.bind();
    m_texCoordBuffer.allocate( &texCoords, 24 * 4 * sizeof(GLfloat));
    m_program->enableAttributeArray("in_texCoord");
    m_program->setAttributeBuffer("in_texCoord", GL_FLOAT, 0, 4);
    m_texCoordBuffer.release();
    //*/
    m_vao0->release();


    m_vao0->bind();
    glDrawElements(GL_TRIANGLES,id,GL_UNSIGNED_INT,0);
    //glDrawElements(GL_LINE_LOOP,36,GL_UNSIGNED_SHORT,0);
    m_vao0->release();
    m_program->release();
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
