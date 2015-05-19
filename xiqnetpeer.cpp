#include "xiqnetpeer.h"
#include "xiqnetpeerprivate.h"
#include "xiqnetwrapper.h"
#include <QTcpSocket>
#include <QHostAddress>


XiQNetPeer::XiQNetPeer(QObject *qObjParent) :
  QObject(qObjParent),
  d_ptr(new XiQNetPeerPrivate(this))
{
  Q_D(XiQNetPeer);
  d->tcpSock=0;
  d->wrapper=0;
}

XiQNetPeer::XiQNetPeer(qintptr socketDescriptor, QObject *qObjParent) :
  QObject(qObjParent),
  d_ptr(new XiQNetPeerPrivate(this))
{
  Q_D(XiQNetPeer);
  d->tcpSock = new QTcpSocket(this);
  d->wrapper = 0;
  connect(d->tcpSock, &QTcpSocket::connected, this, &XiQNetPeer::sigConnectionEstablished);
  connect(d->tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
  connect(d->tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::sigConnectionClosed);
  connect(d->tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));

  connect(d->tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::stopConnection);
  if(!d->tcpSock->setSocketDescriptor(socketDescriptor))
  {
    sigSocketError(d->tcpSock->error());
    qFatal("[xiqnet-qt] Error setting clients socket descriptor");
    Q_ASSERT(false);
  }
  d->tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

XiQNetPeer::~XiQNetPeer()
{
  delete d_ptr;
}


QString XiQNetPeer::getIpAddress()
{
  Q_D(XiQNetPeer);
  return d->tcpSock->peerAddress().toString();
}

quint16 XiQNetPeer::getPort()
{
  Q_D(XiQNetPeer);
  return d->tcpSock->peerPort();
}

bool XiQNetPeer::isConnected()
{
  Q_D(XiQNetPeer);
  if(d->tcpSock)
  {
    return (d->tcpSock->state()==QTcpSocket::ConnectedState || d->tcpSock->state()==QTcpSocket::BoundState);
  }
  else
  {
    return false;
  }
}

int XiQNetPeer::getPeerId()
{
  Q_D(XiQNetPeer);
  return d->peerId;
}

void XiQNetPeer::setPeerId(int peerId)
{
  Q_D(XiQNetPeer);
  if(peerId>=0)
  {
    d->peerId = peerId;
  }
}

QTcpSocket *XiQNetPeer::getTcpSocket()
{
  Q_D(XiQNetPeer);
  return d->tcpSock;
}

void XiQNetPeer::sendMessage(google::protobuf::Message *pMessage)
{
  if(isConnected())
  {
    Q_D(XiQNetPeer);
    if(d->wrapper)
    {
      d->sendArray(d->wrapper->protobufToByteArray(pMessage));
    }
    else
    {
      qCritical() << "[xiqnet-qt] No protobuf wrapper set";
      Q_ASSERT(false);
    }
  }
  else
  {
    qWarning() << "[xiqnet-qt] Trying to send data to disconnected host:" << getIpAddress();
  }
}

void XiQNetPeer::startConnection(QString ipAddress, quint16 port)
{
  Q_D(XiQNetPeer);
  if(d->tcpSock==0)
  {
    d->tcpSock= new QTcpSocket(this);

    connect(d->tcpSock, &QTcpSocket::connected, this, &XiQNetPeer::sigConnectionEstablished);
    connect(d->tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
    connect(d->tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::sigConnectionClosed);
    connect(d->tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));
    connect(d->tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::stopConnection);
    d->tcpSock->connectToHost(ipAddress, port);
    d->tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
  }
  else
  {
    qCritical() << "[xiqnet-qt] Do not re-use ProtoPeer instances, delete & recreate instead";
    Q_ASSERT(false);
  }
}

void XiQNetPeer::stopConnection()
{
  Q_D(XiQNetPeer);
  if(d->tcpSock)
  {
    //void out the wrapper
    d->wrapper=0;

    d->tcpSock->close();
    d->tcpSock->deleteLater();
    d->tcpSock=0;
    //qDebug() << "disconnected";
  }
  else
  {
    qCritical() << "[xiqnet-qt] Tried to execute stopConnection on null pointer";
    Q_ASSERT(false);
  }
}

void XiQNetPeer::onReadyRead()
{
  Q_D(XiQNetPeer);
  if(d->wrapper)
  {
    QByteArray newMessage;
    newMessage = d->readArray();
    while(!newMessage.isNull())
    {
      //qDebug() << "[proto-net-qt] Message received: "<<newMessage.toBase64();
      google::protobuf::Message *tmpMessage = d->wrapper->byteArrayToProtobuf(newMessage);
      sigMessageReceived(tmpMessage);
      //assuming that only direct connections are used
      //delete tmpMessage;
      newMessage = d->readArray();
    }
  }
  else
  {
    qCritical() << "[protonet-qr] No protobuf wrapper set";
    Q_ASSERT(false);
  }
}
XiQNetWrapper *XiQNetPeer::getWrapper()
{
  Q_D(XiQNetPeer);
  return d->wrapper;
}

void XiQNetPeer::setWrapper(XiQNetWrapper *value)
{
  Q_D(XiQNetPeer);
  d->wrapper = value;
}


