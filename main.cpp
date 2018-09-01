#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWin;

    // Load the embedded font.
    QString fontPath = ":/fonts/Ubuntu-R.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);

    if (fontId != -1)
    {
        QFont font("Ubuntu");
        font.setPointSize(10);
        app.setFont(font);
    }

    mainWin.show();

    QString fileFromArg = QString::fromLocal8Bit(argv[1]);

    if (!fileFromArg.isEmpty()){
        mainWin.openFromArgument(fileFromArg);

    }

    return app.exec();
}
