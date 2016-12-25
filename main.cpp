#include <QApplication>
#include "LoginPanel.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    LoginPanel lp;

    lp.show();

    app.exec();

    return 0;
}
