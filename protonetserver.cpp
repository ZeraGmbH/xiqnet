#include "protonetserver.h"
#include "protonetserverprivate.h"

#include "protonetpeer.h"

QList<ProtoNetPeer *> ProtoNetServer::getClientList()
{
  Q_D(ProtoNetServer);
  return d->clients;
}

void ProtoNetServer::setDefaultWrapper(ProtoNetWrapper *wrapper)
{
  if(wrapper)
  {
    Q_D(ProtoNetServer);
    d->defaultWrapper = wrapper;
  }
}

void ProtoNetServer::broadcastMessage(google::protobuf::Message *pMessage)
{
  Q_D(ProtoNetServer);
  foreach(ProtoNetPeer* c,d->clients)
  {
    c->sendMessage(pMessage);
  }
}

void ProtoNetServer::startServer(quint16 uPort)
{
  /// @todo change default port
  this->listen(QHostAddress::Any, uPort);
  qDebug()<<"[proto-net]Server Started on port:" << uPort;
}

ProtoNetServer::ProtoNetServer(QObject *qObjParent) :
  QTcpServer(qObjParent),
  d_ptr(new ProtoNetServerPrivate(this))
{
}

ProtoNetServer::~ProtoNetServer()
{
  delete d_ptr;
}

void ProtoNetServer::incomingConnection(qintptr sockDesc)
{
  Q_D(ProtoNetServer);
  qDebug()<<"[proto-net]Client connected";

  ProtoNetPeer *client = new ProtoNetPeer(sockDesc, this);
  if(d->defaultWrapper)
  {
    client->setWrapper(d->defaultWrapper);
  }
  d->clients.append(client);
  connect(client, &ProtoNetPeer::sigConnectionClosed, this, &ProtoNetServer::clientDisconnectedSRV);
  sigClientConnected(client);
}

void ProtoNetServer::clientDisconnectedSRV()
{
  if(QObject::sender()!=0)
  {
    ProtoNetPeer* client = qobject_cast<ProtoNetPeer*>(QObject::sender());
    if(client)
    {
      Q_D(ProtoNetServer);
      d->clients.removeAll(client);
      //this should be only done if no one uses the client anymore
      client->deleteLater();
    }
  }
}
