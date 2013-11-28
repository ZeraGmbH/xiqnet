#ifndef PROTOPEER_H
#define PROTOPEER_H

#include "protonet-qt_global.h"

//#include "protopeerprivate.h"

#include <QObject>
#include <QHostAddress>

class ProtoNetPeerPrivate;
class ProtoNetWrapper;
namespace google
{
  namespace protobuf
  {
    class Message;
  }
}

class QTcpSocket;

class PROTO_NET_QTSHARED_EXPORT ProtoNetPeer : public QObject
{
  Q_OBJECT
public:
  explicit ProtoNetPeer(QObject *qObjParent = 0);
  ProtoNetPeer(quint32 socketDescriptor, QObject *qObjParent = 0);
  ~ProtoNetPeer();

  QHostAddress getIpAddress();
  quint16 getPort();
  bool isConnected();


  const QUuid &getIdentityUuid();
  void setIdentityUuid(const QUuid &identity);


  ProtoNetWrapper *getWrapper();
  void setWrapper(ProtoNetWrapper *value);

signals:
  void sigConnectionEstablished();
  void sigConnectionClosed();
  void sigMessageReceived(google::protobuf::Message *pMessage);
  void sigSocketError(QAbstractSocket::SocketError socketError);

public slots:
  void sendMessage(google::protobuf::Message *pMessage);
  void startConnection(QHostAddress ipAddress, quint16 port);
  void stopConnection();

protected slots:
  void onReadyRead();

private:
  ProtoNetPeerPrivate *d_ptr;

  Q_DISABLE_COPY(ProtoNetPeer)
  Q_DECLARE_PRIVATE(ProtoNetPeer)
};

#endif // PROTOPEER_H
