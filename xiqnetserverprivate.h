#ifndef H2012_xiqnet_SERVER_PRIVATE_H
#define H2012_xiqnet_SERVER_PRIVATE_H

#include <QList>


class XiQNetPeer;
class XiQNetServer;
class XiQNetWrapper;

class XiQNetServerPrivate
{
  XiQNetServerPrivate(XiQNetServer *t_public);
  ~XiQNetServerPrivate();

  QList<XiQNetPeer*> m_clients;
  XiQNetWrapper *m_defaultWrapper = 0;
  XiQNetServer *q_ptr = 0;

  friend class XiQNetServer;
};

#endif // H2012_xiqnet_SERVER_PRIVATE_H
