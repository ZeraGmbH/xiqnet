#include "xiqnetserverprivate.h"
#include "xiqnetpeer.h"

XiQNetServerPrivate::XiQNetServerPrivate(XiQNetServer *t_public) : q_ptr(t_public)
{
}

XiQNetServerPrivate::~XiQNetServerPrivate()
{
  foreach(XiQNetPeer *c, m_clients)
  {
    c->deleteLater();
  }
  m_clients.clear();
}

