#include "protonetpeerprivate.h"

#include <QDataStream>
#include <QTcpSocket>


ProtoNetPeerPrivate::ProtoNetPeerPrivate(ProtoNetPeer *pPeer) :
  tcpSock(0),
  q_ptr(pPeer)
{
}

QByteArray ProtoNetPeerPrivate::readArray()
{
  if(tcpSock->bytesAvailable())
  {
    QByteArray retVal;
    QDataStream in(tcpSock);
    in.setVersion(QDataStream::Qt_4_0);
    qint32 expectedSize;
    in >> expectedSize;
    if(tcpSock->bytesAvailable()<expectedSize)
    {
      //error
      qWarning("[proto-net]Error bytes not available");
    }
    else
    {
      in >> retVal;
      //qDebug()<<"[proto-net]Receiving message:"<<QString(retVal.toBase64());
    }
    return retVal;
  }
  else
    return QByteArray();
}

void ProtoNetPeerPrivate::sendArray(const QByteArray &bA)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_0);
  out << (qint32)0;
  //qDebug()<<"[proto-net] Sending message:"<<QString(bA.toBase64());
  out << bA;
  out.device()->seek(0);
  out << (qint32)(block.size() - sizeof(qint32));

  if(!tcpSock->write(block))
  {
    qDebug()<<"[proto-net] Failed to send message";
  }
}
