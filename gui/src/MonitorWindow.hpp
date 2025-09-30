#pragma once
#include <QMainWindow>
#include <QUdpSocket>
#include <QLabel>
#include <QTimer>

class MonitorWindow : public QMainWindow {
    Q_OBJECT
public:
    MonitorWindow(const QString& group, quint16 port, QWidget* parent=nullptr);
private slots:
    void onReadyRead();
private:
    QUdpSocket* sock_;
    QLabel* lbl_;
};
