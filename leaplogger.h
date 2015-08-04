#ifndef LEAPLOGGER_H
#define LEAPLOGGER_H

#include <Leap.h>

#include <QJsonObject>
#include <QFile>
#include <QString>
#include <QObject>

#include "loggerconfig.h"

class QTextStream;

class LeapLogger : public QObject, Leap::Listener
{
    Q_OBJECT

    static const std::string fingerNames[];
    static const std::string boneNames[];
    static const std::string stateNames[];

    QFile mF;
    const LoggerConfig& mConfig;

    int mCounter;
    Leap::Controller mController;
    QTextStream* mOut;

    QJsonObject serializeVector(const Leap::Vector v);

public:
    LeapLogger(const QString& fn, const LoggerConfig& config);
    ~LeapLogger();

	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
	virtual void onDeviceChange(const Leap::Controller&);
	virtual void onServiceConnect(const Leap::Controller&);
	virtual void onServiceDisconnect(const Leap::Controller&);

    void connect2Leap();

signals:
    void connected();
    void disconnected();
};

#endif // LEAPLOGGER_H
