#ifndef H2012_PROTONET_SERVER_PRIVATE_H
#define H2012_PROTONET_SERVER_PRIVATE_H

#include <QList>


class ProtoNetPeer;
class ProtoNetServer;
class ProtoNetWrapper;

class ProtoNetServerPrivate
{
protected:
  ProtoNetServerPrivate(ProtoNetServer *parent);
  ~ProtoNetServerPrivate();
  QList<ProtoNetPeer*> clients;
  ProtoNetWrapper *defaultWrapper;
  ProtoNetServer *q_ptr;


private:
  Q_DISABLE_COPY(ProtoNetServerPrivate)
  Q_DECLARE_PUBLIC(ProtoNetServer)
};

#endif // H2012_PROTONET_SERVER_PRIVATE_H
