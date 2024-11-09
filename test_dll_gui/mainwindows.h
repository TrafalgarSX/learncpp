#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtGui/QResizeEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QCloseEvent>
#include <QScopedPointer>
#include <QSharedPointer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void connectDatabase();
    void createTrayIcon();

    void testSmartPtr();

signals:
    void doNow();

private slots:
    void on_loginButton_clicked();

    void on_shutdownButton_clicked();

    void on_restartButton_clicked();

    void on_qrCodeButton_clicked();

    void on_signUpButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H