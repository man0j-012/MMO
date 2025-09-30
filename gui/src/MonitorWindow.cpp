#include "MonitorWindow.hpp"
#include <QVBoxLayout>
#include <QHostAddress>
#include <QNetworkInterface>

MonitorWindow::MonitorWindow(const QString& group, quint16 port, QWidget* parent)
    : QMainWindow(parent), sock_(new QUdpSocket(this)), lbl_(new QLabel("Waiting...", this))
{
    auto central = new QWidget(this);
    auto layout = new QVBoxLayout(central);
    layout->addWidget(lbl_);
    setCentralWidget(central);
    resize(360,120);

    sock_->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    sock_->joinMulticastGroup(QHostAddress(group));
    connect(sock_, &QUdpSocket::readyRead, this, &MonitorWindow::onReadyRead);
}

void MonitorWindow::onReadyRead() {
    while (sock_->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(sock_->pendingDatagramSize()));
        sock_->readDatagram(datagram.data(), datagram.size());
        lbl_->setText(QString::fromUtf8(datagram));
    }
}
