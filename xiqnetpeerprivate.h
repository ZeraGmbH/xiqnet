#ifndef PROTOPEERPRIVATE_H
#define PROTOPEERPRIVATE_H

#include "xiqnetwrapper.h"

#include <QUuid>

class XiQNetPeer;

class QTcpSocket;

/**
 * @brief XiQNetPeer PIMPL
 */
class XiQNetPeerPrivate
{
private:
  explicit XiQNetPeerPrivate(XiQNetPeer *t_publicPeer);

  /**
   * @brief readClient
   * @return
   */
  QByteArray readArray();

  /**
   * @brief sendByteArray
   * @param[in] t_byteArray Data that will be sent
   */
  void sendArray(const QByteArray &t_byteArray) const;

  int m_peerId = -1;

  /**
   * @brief when a tcp message is split in multiple fragments this will ensure that all fragments are complete
   * expected size in bytes
   */
  qint32 m_socketBufferExpectedSize = 0;
  QTcpSocket *m_tcpSock = 0;
  XiQNetWrapper *m_wrapper = 0;
  XiQNetPeer *q_ptr = 0;

  friend class XiQNetPeer;
};

#endif // PROTOPEERPRIVATE_H
