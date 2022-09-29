#include <QApplication>
#include <QGuiApplication>
#include <mainwindow.h>

int main(int argc, char* argv[]) {
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

	MainWindow* window = new MainWindow();
	window->show();

	return app.exec();
}
