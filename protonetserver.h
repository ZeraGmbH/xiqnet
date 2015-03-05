#ifndef H2012_PROTONET_SERVER_PUB_H
#define H2012_PROTONET_SERVER_PUB_H

#include "protonet-qt_global.h"

#include <QTcpServer>

#include <QList>

class ProtoNetServerPrivate;
class ProtoNetPeer;
class ProtoNetWrapper;
namespace google
{
  namespace protobuf
  {
    class Message;
  }
}

class PROTO_NET_QTSHARED_EXPORT ProtoNetServer : public QTcpServer
{
  Q_OBJECT
public:
  explicit ProtoNetServer(QObject* qObjParent = 0);
  ~ProtoNetServer();

  QList<ProtoNetPeer*> getClientList();

  /**
   * @brief Incoming connections use the default ProtoNetWrapper for messaging
   * @param wrapper
   */
  void setDefaultWrapper(ProtoNetWrapper *wrapper);

signals:
  /**
   * @brief A new client connected
   * @param newClient
   */
  void sigClientConnected(ProtoNetPeer* newClient);

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
  ProtoNetServerPrivate *d_ptr;

  Q_DISABLE_COPY(ProtoNetServer)
  Q_DECLARE_PRIVATE(ProtoNetServer)
};

#endif // H2012_PROTONET_SERVER_H
