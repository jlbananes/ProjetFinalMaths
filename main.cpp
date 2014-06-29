#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "drawing.h"
#include "Retopology.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    //Data sceneData;
    qmlRegisterType<Drawing>("OpenGLUnderQML", 1, 0, "Drawing");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/ProjetFinalMaths/main.qml"));
    view.showMaximized();

    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    return app.exec();
}
