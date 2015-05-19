#include "xiqnetserver.h"
#include "xiqnetserverprivate.h"

#include "xiqnetpeer.h"

QList<XiQNetPeer *> XiQNetServer::getClientList()
{
  Q_D(XiQNetServer);
  return d->clients;
}

void XiQNetServer::setDefaultWrapper(XiQNetWrapper *wrapper)
{
  if(wrapper)
  {
    Q_D(XiQNetServer);
    d->defaultWrapper = wrapper;
  }
}

void XiQNetServer::broadcastMessage(google::protobuf::Message *pMessage)
{
  Q_D(XiQNetServer);
  foreach(XiQNetPeer* c,d->clients)
  {
    c->sendMessage(pMessage);
  }
}

void XiQNetServer::startServer(quint16 uPort)
{
  this->listen(QHostAddress::Any, uPort);
  qDebug()<<"[protonet-qt]Server Started on port:" << uPort;
}

XiQNetServer::XiQNetServer(QObject *qObjParent) :
  QTcpServer(qObjParent),
  d_ptr(new XiQNetServerPrivate(this))
{
}

XiQNetServer::~XiQNetServer()
{
  delete d_ptr;
}

void XiQNetServer::incomingConnection(qintptr sockDesc)
{
  Q_D(XiQNetServer);
  qDebug()<<"[protonet-qt]Client connected";

  XiQNetPeer *client = new XiQNetPeer(sockDesc, this);
  if(d->defaultWrapper)
  {
    client->setWrapper(d->defaultWrapper);
  }
  d->clients.append(client);
  connect(client, &XiQNetPeer::sigConnectionClosed, this, &XiQNetServer::clientDisconnectedSRV);
  sigClientConnected(client);
}

void XiQNetServer::clientDisconnectedSRV()
{
  if(QObject::sender()!=0)
  {
    XiQNetPeer* client = qobject_cast<XiQNetPeer*>(QObject::sender());
    if(client)
    {
      Q_D(XiQNetServer);
      d->clients.removeAll(client);
      //this should be only done if no one uses the client anymore
      client->deleteLater();
    }
  }
}
