QT += qml quick opengl

QMAKE_CXXFLAGS += -std=gnu++11

HEADERS += \
    importmesh.h \
    drawing.h \
    Point.h \
    Mesh.h \
    Face.h \
    Edge.h \
    Surface.h \
    Utils.h \
    Retopology.h
SOURCES += main.cpp \
    importmesh.cpp \
    drawing.cpp \
    Point.cpp \
    Mesh.cpp \
    Face.cpp \
    Edge.cpp \
    Utils.cpp \
    Surface.cpp \
    Retopology.cpp
RESOURCES += openglunderqml.qrc

target.path = /qml/ProjetFinalMaths
INSTALLS += target

OTHER_FILES += \
    qml/ProjetFinalMaths/main.qml \
    Shaders/FragmentShader.frag \
    Shaders/VertexShader.vert
