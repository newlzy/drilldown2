#include <QApplication>
#include "connection.h"
#include "view.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(drilldown);

    QApplication app(argc, argv);

    if (!createConnection())
        return EXIT_FAILURE;

    View view("items", "images");
    view.show();

    //Qt::NavigationModeCursorAuto:鼠标光标用于改变焦点，仅在非触摸屏设备上显示。小键盘是用来实现虚拟光标的，除非设备有模拟鼠标类型的输入设备(例如触摸板)。对于需要在触摸和非触摸设备上进行指针控制的应用程序(如web浏览器)，这是推荐的设置。

#ifdef QT_KEYPAD_NAVIGATION    QApplication::setNavigationMode(Qt::NavigationModeCursorAuto);
#endif
    return app.exec();
}
