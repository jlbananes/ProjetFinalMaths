QT += qml quick opengl

HEADERS += \
    importmesh.h \
    drawing.h
SOURCES += main.cpp \
    importmesh.cpp \
    drawing.cpp
RESOURCES += openglunderqml.qrc

target.path = /qml/ProjetFinalMaths
INSTALLS += target

OTHER_FILES += \
    qml/ProjetFinalMaths/main.qml \
    Shaders/FragmentShader.frag \
    Shaders/VertexShader.vert
