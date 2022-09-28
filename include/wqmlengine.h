#ifndef WQMLENGINE_H
#define WQMLENGINE_H
#include <QUrl>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlContext>

class WQMLEngine
{
public:
    WQMLEngine();
    void createQmlWindow(QString filename);
};

#endif // WQMLENGINE_H
