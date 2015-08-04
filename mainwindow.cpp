#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

#include "leaplogger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
    mLogger(nullptr),
    mConfig()
{
    ui->setupUi(this);

	this->connect(this->ui->btnToggleRecording, &QPushButton::toggled, this, &MainWindow::toggleRecording);
	this->connect(this->ui->btnBrowse, &QPushButton::clicked, this, &MainWindow::selectDestination);
}

MainWindow::~MainWindow()
{
    delete ui;
	if(mLogger)
		delete mLogger;
}


void MainWindow::selectDestination(){
	QString fn = QFileDialog::getSaveFileName(this, tr("Destinazione"), "", tr("Json (*.json *.js)"));
    if(!fn.endsWith(".json") || !!fn.endsWith(".js"))
        fn.append(".json");

	QFile f(fn);

	if(!f.open(QIODevice::WriteOnly)){
		QMessageBox notWritable;
		notWritable.setText("File non scrivibile");
		notWritable.setInformativeText("Impossibile scrivere sul file selezionato");
		notWritable.exec();
		return;
	}



	this->ui->destination->setText(fn);
}

void MainWindow::toggleRecording(bool checked){
	if(checked)
		this->startRecording();
	else
		this->stopRecording();
}

void MainWindow::startRecording(){
	if(this->ui->destination->text().isEmpty()){
		QMessageBox warning;
		warning.setText("Specificare destinazione.");
		warning.setIcon(QMessageBox::Warning);
		warning.exec();
		bool oldState = this->ui->btnToggleRecording->blockSignals(true);
		this->ui->btnToggleRecording->setChecked(false);
		this->ui->btnToggleRecording->blockSignals(oldState);

		return;
	}

	this->ui->btnToggleRecording->setText("Stop recording");

    this->mConfig.logTimestamp(this->ui->chkTimestamp->isChecked());
    this->mConfig.logEnd(this->ui->chkEnd->isChecked());
    this->mConfig.logStart(this->ui->chkStart->isChecked());
    this->mConfig.logName(this->ui->chkNome->isChecked());
    this->mConfig.logDirection(this->ui->chkDirection->isChecked());
    this->mConfig.logFingerDimensions(this->ui->chkFingerDimensions->isChecked());
    this->mConfig.logArm(this->ui->chkArm->isChecked());

    this->ui->groupConfig->setEnabled(false);

    this->mLogger = new LeapLogger(this->ui->destination->text(), this->mConfig);
    this->setStatusBar("Connecting", 255, 255, 100);
    connect(this->mLogger, &LeapLogger::connected, this, &MainWindow::leapConnected);
    connect(this->mLogger, &LeapLogger::disconnected, this, &MainWindow::leapDisconnected);
    this->mLogger->connect2Leap();
}

void MainWindow::setStatusBar(const QString& text, int r, int g, int b){
    QPalette palette = this->ui->statusbar->palette(); // get current palette
    palette.setColor(QPalette::Background, QColor(r,g,b)); // modify palette
    this->ui->statusbar->setPalette(palette); // apply modified palette
    this->ui->statusbar->setAutoFillBackground(true); // tell widget to fill its background itself
    this->ui->statusbar->showMessage(text);
}


void MainWindow::leapConnected(){
    this->setStatusBar("Connected", 100, 255, 100);
}
void MainWindow::leapDisconnected(){
    this->setStatusBar("Disconnected", 255, 100, 100);
}

void MainWindow::stopRecording(){
	this->ui->btnToggleRecording->setText("Start recording");

	delete this->mLogger;
	this->mLogger = nullptr;

    this->ui->groupConfig->setEnabled(true);
    this->ui->statusbar->setPalette(this->ui->statusbar->style()->standardPalette()); // apply modified palette
    this->ui->statusbar->setAutoFillBackground(false); // tell widget to fill its background itself
    this->ui->statusbar->clearMessage();
}
