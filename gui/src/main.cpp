#include "MonitorWindow.hpp"
#include <QApplication>

int main(int argc, char** argv){
    QApplication app(argc, argv);
    QString group = "239.255.0.1"; quint16 port = 30001;
    for (int i=1;i<argc;i++){
        QString a = argv[i];
        auto nexts=[&](QString& v){ if (i+1<argc) v = argv[++i]; };
        auto next=[&](quint16& v){ if (i+1<argc) v = QString(argv[++i]).toUShort(); };
        if (a=="--mgroup") nexts(group);
        else if (a=="--mport") next(port);
    }
    MonitorWindow win(group, port);
    win.show();
    return app.exec();
}
