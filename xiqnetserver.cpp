#include "xiqnetserver.h"
#include "xiqnetserverprivate.h"

#include "xiqnetpeer.h"



XiQNetServer::XiQNetServer(QObject *t_parent) :
  QTcpServer(t_parent),
  d_ptr(new XiQNetServerPrivate(this))
{
}

XiQNetServer::~XiQNetServer()
{
  delete d_ptr;
}

QList<XiQNetPeer *> XiQNetServer::getClientList()
{
  Q_D(XiQNetServer);
  return d->clients;
}

void XiQNetServer::setDefaultWrapper(XiQNetWrapper *t_wrapper)
{
  if(t_wrapper)
  {
    Q_D(XiQNetServer);
    d->defaultWrapper = t_wrapper;
  }
}

void XiQNetServer::broadcastMessage(google::protobuf::Message *t_message)
{
  Q_D(XiQNetServer);
  foreach(XiQNetPeer* c,d->clients)
  {
    c->sendMessage(t_message);
  }
}

void XiQNetServer::startServer(quint16 t_port)
{
  this->listen(QHostAddress::Any, t_port);
  qDebug()<<"[xiqnet-qt]Server Started on port:" << t_port;
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

void XiQNetServer::incomingConnection(qintptr t_socketDescriptor)
{
  Q_D(XiQNetServer);
  qDebug()<<"[xiqnet-qt]Client connected";

  XiQNetPeer *client = new XiQNetPeer(t_socketDescriptor, this);
  if(d->defaultWrapper)
  {
    client->setWrapper(d->defaultWrapper);
  }
  d->clients.append(client);
  connect(client, &XiQNetPeer::sigConnectionClosed, this, &XiQNetServer::clientDisconnectedSRV);
  sigClientConnected(client);
}
