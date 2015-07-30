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

QList<XiQNetPeer *> XiQNetServer::getClientList() const
{
  return d_ptr->m_clients;
}

void XiQNetServer::setDefaultWrapper(XiQNetWrapper *t_wrapper)
{
  if(t_wrapper)
  {
    d_ptr->m_defaultWrapper = t_wrapper;
  }
}

void XiQNetServer::broadcastMessage(google::protobuf::Message *t_message) const
{
  foreach(XiQNetPeer *c,d_ptr->m_clients)
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
  if(QObject::sender()!=0) /// @todo redesign to not rely on QObject::sender
  {
    XiQNetPeer *client = qobject_cast<XiQNetPeer*>(QObject::sender());
    if(client)
    {
      d_ptr->m_clients.removeAll(client);
      ///@note use deletelater to execute other signal slot connections connected to the XiQNetPeer::sigConnectionClosed signal
      client->deleteLater();
    }
  }
}

void XiQNetServer::incomingConnection(qintptr t_socketDescriptor)
{
  qDebug()<<"[xiqnet-qt]Client connected";

  XiQNetPeer *client = new XiQNetPeer(t_socketDescriptor, this);
  if(d_ptr->m_defaultWrapper)
  {
    client->setWrapper(d_ptr->m_defaultWrapper);
  }
  d_ptr->m_clients.append(client);
  connect(client, &XiQNetPeer::sigConnectionClosed, this, &XiQNetServer::clientDisconnectedSRV);
  emit sigClientConnected(client);
}
