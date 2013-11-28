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

  void setDefaultWrapper(ProtoNetWrapper *wrapper);

signals:
  void sigClientConnected(ProtoNetPeer* newClient);

public slots:
  void broadcastMessage(google::protobuf::Message *pMessage);
  void startServer(quint16 uPort);

protected slots:
  void clientDisconnectedSRV();

protected:
  void incomingConnection(qintptr sockDesc) override;

private:
  ProtoNetServerPrivate *d_ptr;

  Q_DISABLE_COPY(ProtoNetServer)
  Q_DECLARE_PRIVATE(ProtoNetServer)
};

#endif // H2012_PROTONET_SERVER_H
