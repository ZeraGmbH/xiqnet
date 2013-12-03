#ifndef PROTOPEERPRIVATE_H
#define PROTOPEERPRIVATE_H

#include "protonetwrapper.h"

#include <QUuid>

class ProtoNetPeer;

class QTcpSocket;

class ProtoNetPeerPrivate
{
private:
  ProtoNetPeerPrivate(ProtoNetPeer *pPeer);

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

  QUuid identityUuid;

  qint32 socketBufferExpectedSize;

  /**
   * @brief tcpSock
   */
  QTcpSocket* tcpSock;

  /**
   * @brief q_ptr see topic D'Pointer
   */
  ProtoNetPeer *q_ptr;

  ProtoNetWrapper *wrapper;

  Q_DECLARE_PUBLIC(ProtoNetPeer)
};

#endif // PROTOPEERPRIVATE_H
