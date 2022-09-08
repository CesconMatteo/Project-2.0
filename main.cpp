#define TEST 0

#if TEST == 1
#include <iostream>
using namespace std;
int main() {

}
#else

#include "View/mainwidget.h"
#include "Model/model.h"
#include "Control/controller.h"

#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWidget w;
    Controller c;
    Model m;
    c.setup(&m, &w);
    w.setController(&c);
    w.show();
    app.setWindowIcon(QIcon(":/Static/icon.png"));
    return app.exec();
}
#endif
