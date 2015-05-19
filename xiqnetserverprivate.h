#ifndef H2012_protonet_SERVER_PRIVATE_H
#define H2012_protonet_SERVER_PRIVATE_H

#include <QList>


class XiQNetPeer;
class XiQNetServer;
class XiQNetWrapper;

class XiQNetServerPrivate
{
protected:
  XiQNetServerPrivate(XiQNetServer *parent);
  ~XiQNetServerPrivate();
  QList<XiQNetPeer*> clients;
  XiQNetWrapper *defaultWrapper;
  XiQNetServer *q_ptr;


private:
  Q_DISABLE_COPY(XiQNetServerPrivate)
  Q_DECLARE_PUBLIC(XiQNetServer)
};

#endif // H2012_protonet_SERVER_PRIVATE_H
