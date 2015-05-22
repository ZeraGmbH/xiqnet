#include "xiqnetpeerprivate.h"

#include <QDataStream>
#include <QTcpSocket>


XiQNetPeerPrivate::XiQNetPeerPrivate(XiQNetPeer *t_publicPeer) :  q_ptr(t_publicPeer)
{
}

QByteArray XiQNetPeerPrivate::readArray()
{
  // it is at least required to read the expected size
  if(m_tcpSock->bytesAvailable()>=static_cast<qint64>(sizeof(qint32)))
  {
    QByteArray retVal;
    QDataStream in(m_tcpSock);
    in.setVersion(QDataStream::Qt_4_0);

    if(m_socketBufferExpectedSize==0)
    {
      in >> m_socketBufferExpectedSize;
    }
    if(m_tcpSock->bytesAvailable()<m_socketBufferExpectedSize)
    {
      // need to wait for missing data
    }
    else
    {
      in >> retVal;
      if(retVal.isNull() || retVal.size()>m_socketBufferExpectedSize)
      {
        qWarning() << "[xiqnet-qt] Bytes not available:"
                   << QString("%1/%2, remaining: %3")
                      .arg(retVal.size())
                      .arg(m_socketBufferExpectedSize)
                      .arg(m_tcpSock->bytesAvailable());
      }
      m_socketBufferExpectedSize=0;
    }
    return retVal;
  }
  else
    return QByteArray();
}

void XiQNetPeerPrivate::sendArray(const QByteArray &t_byteArray) const
{
  if(!(m_tcpSock && m_tcpSock->isOpen()))
  {
    qDebug()<<"[xiqnet-qt] Failed to send message";
  }
  else
  {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (qint32)0;

    //qDebug()<<"[xiqnet-qt] Sending message:"<<QString(bA.toBase64());
    out << t_byteArray;
    out.device()->seek(0);
    out << (qint32)(block.size() - sizeof(qint32));

    if(m_tcpSock->write(block)<block.size())
    {
      qDebug() << "[xiqnet-qt] could not send all data the network is congested";
    }
  }
}
