#ifndef PROTOPEERPRIVATE_H
#define PROTOPEERPRIVATE_H

#include "xiqnetwrapper.h"

#include <QUuid>

class XiQNetPeer;

class QTcpSocket;

class XiQNetPeerPrivate
{
private:
  explicit XiQNetPeerPrivate(XiQNetPeer *pPeer);

  /**
   * @brief readClient
   * @return
   */
  QByteArray readArray();

  /**
   * @brief sendByteArray
   * @param[in] bA Data that will be sent
   */
  void sendArray(const QByteArray &bA);

  int peerId;

  qint32 socketBufferExpectedSize;

  /**
   * @brief tcpSock
   */
  QTcpSocket* tcpSock;

  /**
   * @brief q_ptr see topic D'Pointer
   */
  XiQNetPeer *q_ptr;

  XiQNetWrapper *wrapper;

  Q_DECLARE_PUBLIC(XiQNetPeer)
};

#endif // PROTOPEERPRIVATE_H
