#ifndef CUBE_H
#define CUBE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

class Cube : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged);
public:
    Cube();
    qreal t() const { return m_t; }
    void setT(qreal t);

signals:
    void tChanged();
};

#endif // CUBE_H
