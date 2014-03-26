#include "protonetpeer.h"
#include "protonetpeerprivate.h"
#include "protonetwrapper.h"
#include <QTcpSocket>
#include <QHostAddress>


ProtoNetPeer::ProtoNetPeer(QObject *qObjParent) :
  QObject(qObjParent),
  d_ptr(new ProtoNetPeerPrivate(this))
{
  Q_D(ProtoNetPeer);
  d->tcpSock=0;
  d->wrapper=0;
}

ProtoNetPeer::ProtoNetPeer(qintptr socketDescriptor, QObject *qObjParent) :
  QObject(qObjParent),
  d_ptr(new ProtoNetPeerPrivate(this))
{
  Q_D(ProtoNetPeer);
  d->tcpSock = new QTcpSocket(this);
  d->wrapper = 0;
  connect(d->tcpSock, &QTcpSocket::connected, this, &ProtoNetPeer::sigConnectionEstablished);
  connect(d->tcpSock, &QTcpSocket::readyRead, this, &ProtoNetPeer::onReadyRead);
  connect(d->tcpSock, &QTcpSocket::disconnected, this, &ProtoNetPeer::sigConnectionClosed);
  connect(d->tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));

  connect(d->tcpSock, &QTcpSocket::disconnected, this, &ProtoNetPeer::stopConnection);
  if(!d->tcpSock->setSocketDescriptor(socketDescriptor))
  {
    sigSocketError(d->tcpSock->error());
    qFatal("[protonet-qt] Error setting clients socket descriptor");
    Q_ASSERT(false);
  }
  d->tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

ProtoNetPeer::~ProtoNetPeer()
{
  delete d_ptr;
}


QString ProtoNetPeer::getIpAddress()
{
  Q_D(ProtoNetPeer);
  return d->tcpSock->peerAddress().toString();
}

quint16 ProtoNetPeer::getPort()
{
  Q_D(ProtoNetPeer);
  return d->tcpSock->peerPort();
}

bool ProtoNetPeer::isConnected()
{
  Q_D(ProtoNetPeer);
  if(d->tcpSock)
  {
    return (d->tcpSock->state()==QTcpSocket::ConnectedState || d->tcpSock->state()==QTcpSocket::BoundState);
  }
  else
  {
    return false;
  }
}
const QUuid &ProtoNetPeer::getIdentityUuid()
{
  Q_D(ProtoNetPeer);
  return d->identityUuid;
}

void ProtoNetPeer::setIdentityUuid(const QUuid &identity)
{
  Q_D(ProtoNetPeer);
  d->identityUuid = identity;
}

QTcpSocket *ProtoNetPeer::getTcpSocket()
{
  Q_D(ProtoNetPeer);
  return d->tcpSock;
}

void ProtoNetPeer::sendMessage(google::protobuf::Message *pMessage)
{
  if(isConnected())
  {
    Q_D(ProtoNetPeer);
    if(d->wrapper)
    {
      d->sendArray(d->wrapper->protobufToByteArray(pMessage));
    }
    else
    {
      qCritical() << "[protonet-qt] No protobuf wrapper set";
      Q_ASSERT(false);
    }
  }
  else
  {
    qWarning() << "[protonet-qt] Trying to send data to disconnected host";
  }
}

void ProtoNetPeer::startConnection(QString ipAddress, quint16 port)
{
  Q_D(ProtoNetPeer);
  if(d->tcpSock==0)
  {
    d->tcpSock= new QTcpSocket(this);

    connect(d->tcpSock, &QTcpSocket::connected, this, &ProtoNetPeer::sigConnectionEstablished);
    connect(d->tcpSock, &QTcpSocket::readyRead, this, &ProtoNetPeer::onReadyRead);
    connect(d->tcpSock, &QTcpSocket::disconnected, this, &ProtoNetPeer::sigConnectionClosed);
    connect(d->tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));
    connect(d->tcpSock, &QTcpSocket::disconnected, this, &ProtoNetPeer::stopConnection);
    d->tcpSock->connectToHost(ipAddress, port);
    d->tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
  }
  else
  {
    qCritical() << "[protonet-qt] Do not re-use ProtoPeer instances, delete & recreate instead";
    Q_ASSERT(false);
  }
}

void ProtoNetPeer::stopConnection()
{
  Q_D(ProtoNetPeer);
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
    qCritical() << "[protonet-qt] Tried to execute stopConnection on null pointer";
    Q_ASSERT(false);
  }
}

void ProtoNetPeer::onReadyRead()
{
  Q_D(ProtoNetPeer);
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
      delete tmpMessage;
      newMessage = d->readArray();
    }
  }
  else
  {
    qCritical() << "[protonet-qr] No protobuf wrapper set";
    Q_ASSERT(false);
  }
}
ProtoNetWrapper *ProtoNetPeer::getWrapper()
{
  Q_D(ProtoNetPeer);
  return d->wrapper;
}

void ProtoNetPeer::setWrapper(ProtoNetWrapper *value)
{
  Q_D(ProtoNetPeer);
  d->wrapper = value;
}


