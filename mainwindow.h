#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loggerconfig.h"

namespace Ui {
class MainWindow;
}

class LeapLogger;

class MainWindow : public QMainWindow
{
    Q_OBJECT
	Ui::MainWindow *ui;
	LeapLogger* mLogger;
    LoggerConfig mConfig;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
	void startRecording();
	void stopRecording();

    void setStatusBar(const QString& text, int r, int g, int b);

private slots:
	void toggleRecording(bool checked);
	void selectDestination();

    void leapConnected();
    void leapDisconnected();



};

#endif // MAINWINDOW_H
