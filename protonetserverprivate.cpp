#include "protonetserverprivate.h"
#include "protonetpeer.h"

ProtoNetServerPrivate::ProtoNetServerPrivate(ProtoNetServer *parent) :
  defaultWrapper(0),
  q_ptr(parent)
{
}

ProtoNetServerPrivate::~ProtoNetServerPrivate()
{
  foreach(ProtoNetPeer* c, clients)
  {
    clients.removeAll(c);
    c->deleteLater();
  }
}

