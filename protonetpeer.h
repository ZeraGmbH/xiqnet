#ifndef PROTOPEER_H
#define PROTOPEER_H

#include "protonet-qt_global.h"

#include <QObject>
#include <QString>
#include <QAbstractSocket>

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
  ProtoNetPeer(qintptr socketDescriptor, QObject *qObjParent = 0);
  ~ProtoNetPeer();

  QString getIpAddress();
  quint16 getPort();
  bool isConnected();


  int getPeerId();
  void setPeerId(int peerId);

  /**
   * @brief Allows access to the QTcpSocket in case of non protobuf transmission
   * @return QTcpSocket used by this instance
   */
  QTcpSocket *getTcpSocket();


  /**
   * @brief Returns a reference to the used ProtoNetWrapper
   * @return
   */
  ProtoNetWrapper *getWrapper();
  /**
   * @brief Sets the ProtoNetWrapper for this peer
   * @param value
   */
  void setWrapper(ProtoNetWrapper *value);

signals:
  /**
   * @brief Emitted if successful connected
   */
  void sigConnectionEstablished();
  /**
   * @brief Emitted when the connection is interrupted
   */
  void sigConnectionClosed();
  /**
   * @brief Emitted when incoming messages arrive
   * @param pMessage
   * @note Do not use the pMessage parameter with Qt::QueuedConnection as it may be deleted before the slot is called
   */
  void sigMessageReceived(google::protobuf::Message *pMessage);
  /**
   * @brief Emitted on socket failure
   * @param socketError
   */
  void sigSocketError(QAbstractSocket::SocketError socketError);

public slots:
  /**
   * @brief Transmits the message
   * @param pMessage
   */
  void sendMessage(google::protobuf::Message *pMessage);
  /**
   * @brief Starts the connection in case of a dedicated host connection
   *
   * Called after Signal/Slot setup
   * @param ipAddress
   * @param port
   */
  void startConnection(QString ipAddress, quint16 port);
  /**
   * @brief Closes the connection
   */
  void stopConnection();

protected slots:
  /**
   * @brief Translates incoming messages to google::protobuf::Message via the ProtoNetWrapper
   */
  void onReadyRead();

private:
  /**
   * @brief PIMPL pointer
   */
  ProtoNetPeerPrivate *d_ptr;

  Q_DISABLE_COPY(ProtoNetPeer)
  Q_DECLARE_PRIVATE(ProtoNetPeer)
};

#endif // PROTOPEER_H
