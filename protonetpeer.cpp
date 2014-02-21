#include "protonetpeer.h"
#include "protonetpeerprivate.h"
#include "protonetwrapper.h"
#include <QTcpSocket>



ProtoNetPeer::ProtoNetPeer(QObject *qObjParent) :
  QObject(qObjParent),
  d_ptr(new ProtoNetPeerPrivate(this))
{
}

ProtoNetPeer::ProtoNetPeer(qintptr socketDescriptor, QObject *qObjParent) :
  QObject(qObjParent),
  d_ptr(new ProtoNetPeerPrivate(this))
{
  Q_D(ProtoNetPeer);
  d->tcpSock = new QTcpSocket(this);
  connect(d->tcpSock, &QTcpSocket::connected, this, &ProtoNetPeer::sigConnectionEstablished);
  connect(d->tcpSock, &QTcpSocket::readyRead, this, &ProtoNetPeer::onReadyRead);
  connect(d->tcpSock, &QTcpSocket::disconnected, this, &ProtoNetPeer::sigConnectionClosed);
  connect(d->tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));
  if(!d->tcpSock->setSocketDescriptor(socketDescriptor))
  {
    sigSocketError(d->tcpSock->error());
    qFatal("[proto-net] Error setting clients socket descriptor");
    Q_ASSERT(false);
  }
  d->tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

ProtoNetPeer::~ProtoNetPeer()
{
  delete d_ptr;
}


QHostAddress ProtoNetPeer::getIpAddress()
{
  Q_D(ProtoNetPeer);
  return d->tcpSock->peerAddress();
}

quint16 ProtoNetPeer::getPort()
{
  Q_D(ProtoNetPeer);
  return d->tcpSock->peerPort();
}

bool ProtoNetPeer::isConnected()
{
  Q_D(ProtoNetPeer);
  return (d->tcpSock->state()==QTcpSocket::ConnectedState);
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

void ProtoNetPeer::sendMessage(google::protobuf::Message *pMessage)
{
  Q_D(ProtoNetPeer);
  if(d->wrapper)
  {
    d->sendArray(d->wrapper->protobufToByteArray(pMessage));
  }
  else
  {
    qCritical() << "[programmers-mistake] No protobuf wrapper set";
    Q_ASSERT(false);
  }
}

void ProtoNetPeer::startConnection(QHostAddress ipAddress, quint16 port)
{
  Q_D(ProtoNetPeer);
  if(d->tcpSock==0)
  {
    d->tcpSock= new QTcpSocket(this);

    connect(d->tcpSock, &QTcpSocket::connected, this, &ProtoNetPeer::sigConnectionEstablished);
    connect(d->tcpSock, &QTcpSocket::readyRead, this, &ProtoNetPeer::onReadyRead);
    connect(d->tcpSock, &QTcpSocket::disconnected, this, &ProtoNetPeer::sigConnectionClosed);
    connect(d->tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));
    d->tcpSock->connectToHost(ipAddress, port);
    d->tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
  }
  else
  {
    qCritical() << "[error] Do not re-use ProtoPeer instances, delete & recreate instead";
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
    //qDebug() << "disconnected";
  }
  else
  {
    qCritical() << "[programmers-mistake] Tried to execute stopConnection on null pointer";
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
      google::protobuf::Message *tmpMessage = d->wrapper->byteArrayToProtobuf(newMessage);
      sigMessageReceived(tmpMessage);
      delete tmpMessage;
      newMessage = d->readArray();
    }
  }
  else
  {
    qCritical() << "[programmers-mistake] No protobuf wrapper set";
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


