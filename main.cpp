#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "squircle.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/ProjetFinalMaths/main.qml"));
    view.show();

    QObject::connect((QObject*)view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    // Bijour !

    // Bijour 2 !

    // Encore un bijour \o/

    return app.exec();
}
