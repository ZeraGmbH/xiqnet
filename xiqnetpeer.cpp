#include "xiqnetpeer.h"
#include "xiqnetpeerprivate.h"
#include "xiqnetwrapper.h"
#include <QTcpSocket>
#include <QHostAddress>

XiQNetPeer::XiQNetPeer(QObject *t_parent) :
  QObject(t_parent),
  d_ptr(new XiQNetPeerPrivate(this))
{
}

XiQNetPeer::XiQNetPeer(qintptr t_socketDescriptor, QObject *t_parent) :
  QObject(t_parent),
  d_ptr(new XiQNetPeerPrivate(this))
{
  Q_D(XiQNetPeer);
  d->m_tcpSock = new QTcpSocket(this);
  d->m_wrapper = 0;
  connect(d->m_tcpSock, &QTcpSocket::connected, this, &XiQNetPeer::sigConnectionEstablished);
  connect(d->m_tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
  connect(d->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::sigConnectionClosed);
  connect(d->m_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));

  connect(d->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::stopConnection);
  if(!d->m_tcpSock->setSocketDescriptor(t_socketDescriptor))
  {
    sigSocketError(d->m_tcpSock->error());
    qFatal("[xiqnet-qt] Error setting clients socket descriptor");
    Q_ASSERT(false);
  }
  d->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
}

XiQNetPeer::~XiQNetPeer()
{
  delete d_ptr;
}

QString XiQNetPeer::getIpAddress() const
{
  return d_ptr->m_tcpSock->peerAddress().toString();
}

quint16 XiQNetPeer::getPort() const
{
  return d_ptr->m_tcpSock->peerPort();
}

bool XiQNetPeer::isConnected() const
{
  if(d_ptr->m_tcpSock)
  {
    return (d_ptr->m_tcpSock->state()==QTcpSocket::ConnectedState
            || d_ptr->m_tcpSock->state()==QTcpSocket::BoundState);
  }
  else
  {
    return false;
  }
}

int XiQNetPeer::getPeerId() const
{
  return d_ptr->m_peerId;
}

void XiQNetPeer::setPeerId(int t_peerId)
{
  if(t_peerId>=0)
  {
    d_ptr->m_peerId = t_peerId;
  }
}

QTcpSocket *XiQNetPeer::getTcpSocket() const
{
  return d_ptr->m_tcpSock;
}

XiQNetWrapper *XiQNetPeer::getWrapper() const
{
  return d_ptr->m_wrapper;
}

void XiQNetPeer::setWrapper(XiQNetWrapper *value)
{
  Q_D(XiQNetPeer);
  d->m_wrapper = value;
}

void XiQNetPeer::sendMessage(google::protobuf::Message *t_message) const
{
  if(isConnected())
  {
    if(d_ptr->m_wrapper)
    {
      d_ptr->sendArray(d_ptr->m_wrapper->protobufToByteArray(t_message));
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

void XiQNetPeer::startConnection(QString t_ipAddress, quint16 t_port)
{
  Q_D(XiQNetPeer);
  if(d->m_tcpSock==0)
  {
    d->m_tcpSock= new QTcpSocket(this);

    connect(d->m_tcpSock, &QTcpSocket::connected, this, &XiQNetPeer::sigConnectionEstablished);
    connect(d->m_tcpSock, &QTcpSocket::readyRead, this, &XiQNetPeer::onReadyRead);
    connect(d->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::sigConnectionClosed);
    connect(d->m_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SIGNAL(sigSocketError(QAbstractSocket::SocketError)));
    connect(d->m_tcpSock, &QTcpSocket::disconnected, this, &XiQNetPeer::stopConnection);
    d->m_tcpSock->connectToHost(t_ipAddress, t_port);
    d->m_tcpSock->setSocketOption(QAbstractSocket::KeepAliveOption, true);
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
  if(d->m_tcpSock)
  {
    //void out the wrapper
    d->m_wrapper=0;

    d->m_tcpSock->close();
    d->m_tcpSock->deleteLater();
    d->m_tcpSock=0;
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
  if(d->m_wrapper)
  {
    QByteArray newMessage;
    newMessage = d->readArray();
    while(!newMessage.isNull())
    {
      //qDebug() << "[proto-net-qt] Message received: "<<newMessage.toBase64();
      google::protobuf::Message *tmpMessage = d->m_wrapper->byteArrayToProtobuf(newMessage);
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
