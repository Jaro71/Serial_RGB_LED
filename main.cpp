#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setFixedSize(560, 160); // Okienko b�dzie mia�o sta�� wysoko�� i szeroko��
    w.setWindowTitle("Kontroler RGB LED");
    w.show();

    return a.exec();
}