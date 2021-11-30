#include "Game.hpp"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game w;
    w.setAttribute(Qt::WA_DeleteOnClose);
    w.show();
    return a.exec();
}
