#ifndef H2012_protonet_SERVER_PUB_H
#define H2012_protonet_SERVER_PUB_H

#include "xiqnet_global.h"

#include <QTcpServer>

#include <QList>

class XiQNetServerPrivate;
class XiQNetPeer;
class XiQNetWrapper;
namespace google
{
  namespace protobuf
  {
    class Message;
  }
}

class XIQNET_QTSHARED_EXPORT XiQNetServer : public QTcpServer
{
  Q_OBJECT
public:
  explicit XiQNetServer(QObject* qObjParent = 0);
  ~XiQNetServer();

  QList<XiQNetPeer*> getClientList();

  /**
   * @brief Incoming connections use the default XiQNetWrapper for messaging
   * @param wrapper
   */
  void setDefaultWrapper(XiQNetWrapper *wrapper);

signals:
  /**
   * @brief A new client connected
   * @param newClient
   */
  void sigClientConnected(XiQNetPeer* newClient);

public slots:
  /**
   * @brief Sends the message to all peers of this server
   * @param pMessage
   */
  void broadcastMessage(google::protobuf::Message *pMessage);

  /**
   * @brief Convenient function for QTcpServer::listen()
   *
   * Prints out the server port to the debug message handler
   *
   * @param uPort
   */
  void startServer(quint16 uPort);

protected slots:
  /**
   * @brief For reference housekeeping
   */
  void clientDisconnectedSRV();

protected:
  /**
   * @brief Override of QTcpServer function
   * @param sockDesc
   */
  void incomingConnection(qintptr sockDesc) override;

private:
  /**
   * @brief PIMPL pointer
   */
  XiQNetServerPrivate *d_ptr;

  Q_DISABLE_COPY(XiQNetServer)
  Q_DECLARE_PRIVATE(XiQNetServer)
};

#endif // H2012_protonet_SERVER_H
