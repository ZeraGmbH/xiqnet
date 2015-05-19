#include "xiqnetserverprivate.h"
#include "xiqnetpeer.h"

XiQNetServerPrivate::XiQNetServerPrivate(XiQNetServer *parent) :
  defaultWrapper(0),
  q_ptr(parent)
{
}

XiQNetServerPrivate::~XiQNetServerPrivate()
{
  foreach(XiQNetPeer* c, clients)
  {
    clients.removeAll(c);
    c->deleteLater();
  }
}

