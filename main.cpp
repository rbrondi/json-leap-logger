/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

//#include <iostream>
//#include <cstring>
//#include "leaplogger.h"
//#include "Leap.h"


#include <iostream>
#include <cstdio>

#include <QApplication>
#include <QFileDialog>

#include "mainwindow.h"

int main(int argc, char** argv) {
    //cout << "Starting application. version (" << ARTH_VERSION_MAJOR << "." << ARTH_VERSION_MINOR << ")" << endl;

    //disable output stream buffering
    std::setbuf(stdout, nullptr);

    QApplication app(argc, argv);
    app.setApplicationName("AIAS - Bike prj");

//    QString assetFolder = QFileDialog::getExistingDirectory(nullptr, "Select asset folder", "/home/hara/development/haptics/arth/lib/assets");
//    if(assetFolder.isEmpty())
//        exit(0);

    MainWindow mainWin;
    mainWin.show();
//    mainWin.showMaximized();

    return app.exec();
}
