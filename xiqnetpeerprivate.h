#ifndef PROTOPEERPRIVATE_H
#define PROTOPEERPRIVATE_H

#include "xiqnetwrapper.h"

#include <QUuid>

class XiQNetPeer;

class QTcpSocket;

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
   * @param[in] bA Data that will be sent
   */
  void sendArray(const QByteArray &t_byteArray);

  int peerId = -1;
  qint32 socketBufferExpectedSize = 0;
  /**
   * @brief tcpSock
   */
  QTcpSocket* tcpSock = 0;
  /**
   * @brief q_ptr see topic D'Pointer
   */
  XiQNetPeer *q_ptr = 0;
  XiQNetWrapper *wrapper = 0;

  Q_DECLARE_PUBLIC(XiQNetPeer)
};

#endif // PROTOPEERPRIVATE_H
