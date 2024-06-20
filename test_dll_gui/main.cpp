#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>


int test_call_gui(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    qputenv("QT_PLUGIN_PATH", "D:/workspace/dev/vcpkg/installed/x64-windows/plugins/platforms");
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(true);
    QMainWindow w;
    w.show();

    test_call_gui(argc, argv);
    return a.exec();
}

