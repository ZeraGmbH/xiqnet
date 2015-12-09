#include "xiqnetpeerprivate.h"

#include <QDataStream>
#include <QTcpSocket>


XiQNetPeerPrivate::XiQNetPeerPrivate(XiQNetPeer *t_publicPeer) :  q_ptr(t_publicPeer)
{
}

QByteArray XiQNetPeerPrivate::readArray()
{
  // it is at least required to read the expected size
  if(Q_LIKELY(m_tcpSock->bytesAvailable()>=4)) // 4 == sizeof(qint32)
  {
    QDataStream in(m_tcpSock);
    in.setVersion(QDataStream::Qt_4_0);

    if(m_socketBufferExpectedSize==0)
    {
      in >> m_socketBufferExpectedSize;
    }
    if(m_tcpSock->bytesAvailable() >= m_socketBufferExpectedSize)
    {
      QByteArray retVal(m_socketBufferExpectedSize, '\0');
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

      return retVal;
    }
    else
    {
      // need to wait for missing data
      return QByteArray();
    }
  }
  else
    return QByteArray();
}

void XiQNetPeerPrivate::sendArray(const QByteArray &t_byteArray) const
{
  Q_ASSERT(m_tcpSock != 0);
  Q_ASSERT(m_tcpSock->isOpen());

  QByteArray block(t_byteArray.size() + 4, '\0'); // 4 == sizeof(qint32)
  QDataStream out(&block, QIODevice::WriteOnly);

  out.device()->seek(0);
  out.setVersion(QDataStream::Qt_4_0);
  out << static_cast<qint32>(0);
  out << t_byteArray;
  out.device()->seek(0);
  out << static_cast<qint32>(block.size() - sizeof(qint32));

  if(m_tcpSock->write(block)<block.size())
  {
    qDebug() << "[xiqnet-qt] could not send all data the network is congested";
  }
}
