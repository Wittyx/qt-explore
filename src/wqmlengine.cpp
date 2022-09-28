#include "wqmlengine.h"
#include "todolist.h"
#include "todomodel.h"


WQMLEngine::WQMLEngine()
{

}

void WQMLEngine::createQmlWindow(QString filename){
    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "ToDoList",
        QStringLiteral("ToDoList should not be created in QML"));

    ToDoList toDoList;

    QQmlEngine engine;
    QQmlComponent component(&engine,QUrl(QStringLiteral("qrc:/ToDoList.qml")));
    engine.rootContext()->setContextProperty(QStringLiteral("toDoList"), &toDoList);
    QObject *object = component.create();

//    if (engine.rootObjects().isEmpty())
//        return -1;

}
