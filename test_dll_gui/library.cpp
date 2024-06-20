#include <iostream>
#include <thread>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int test_call_gui(int argc, char *argv[]) {

  std::thread th = std::thread([&argc, argv]() {
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(true);
    QMainWindow w;
    w.show();
    return a.exec();
  });

  th.detach();

  return 0;
}