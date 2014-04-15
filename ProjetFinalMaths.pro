QT += qml quick opengl

HEADERS += squircle.h \
    importmesh.h
SOURCES += squircle.cpp main.cpp \
    importmesh.cpp
RESOURCES += openglunderqml.qrc

target.path = /qml/ProjetFinalMaths
INSTALLS += target

OTHER_FILES += \
    FragmentShader.frag \
    VertexShader.vert \
    qml/ProjetFinalMaths/main.qml \
    Shaders/FragmentShader.frag \
    Shaders/VertexShader.vert
