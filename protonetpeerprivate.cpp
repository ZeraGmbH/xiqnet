#include "protonetpeerprivate.h"

#include <QDataStream>
#include <QTcpSocket>


ProtoNetPeerPrivate::ProtoNetPeerPrivate(ProtoNetPeer *pPeer) :
  socketBufferExpectedSize(0),
  tcpSock(0),
  q_ptr(pPeer)
{

}

QByteArray ProtoNetPeerPrivate::readArray()
{
  // it is at least required to read the expected size
  if(tcpSock->bytesAvailable()>=static_cast<qint64>(sizeof(qint32)))
  {
    QByteArray retVal;
    QDataStream in(tcpSock);
    in.setVersion(QDataStream::Qt_4_0);

    if(socketBufferExpectedSize==0)
    {
      in >> socketBufferExpectedSize;
    }
    if(tcpSock->bytesAvailable()<socketBufferExpectedSize)
    {
      // need to wait for missing data
    }
    else
    {
      in >> retVal;
      if(retVal.isEmpty() || retVal.size()>socketBufferExpectedSize)
      {
        qWarning() << "[proto-net] Bytes not available:" << QString("%1/%2, remaining: %3")
                      .arg(retVal.size())
                      .arg(socketBufferExpectedSize)
                      .arg(tcpSock->bytesAvailable());
      }
      socketBufferExpectedSize=0;
    }
    return retVal;
  }
  else
    return QByteArray();
}

void ProtoNetPeerPrivate::sendArray(const QByteArray &bA)
{
  if(!(tcpSock && tcpSock->isOpen()))
  {
    qDebug()<<"[proto-net] Failed to send message";
  }
  else
  {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (qint32)0;

    //qDebug()<<"[proto-net] Sending message:"<<QString(bA.toBase64());
    out << bA;
    out.device()->seek(0);
    out << (qint32)(block.size() - sizeof(qint32));

    tcpSock->write(block);
  }

}
