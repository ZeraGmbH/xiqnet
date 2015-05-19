#ifndef PROTOPEER_H
#define PROTOPEER_H

#include "xiqnet_global.h"

#include <QObject>
#include <QString>
#include <QAbstractSocket>

class XiQNetPeerPrivate;
class XiQNetWrapper;
namespace google
{
  namespace protobuf
  {
    class Message;
  }
}

class QTcpSocket;

class XIQNET_QTSHARED_EXPORT XiQNetPeer : public QObject
{
  Q_OBJECT
public:
  explicit XiQNetPeer(QObject *qObjParent = 0);
  XiQNetPeer(qintptr socketDescriptor, QObject *qObjParent = 0);
  ~XiQNetPeer();

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
   * @brief Returns a reference to the used XiQNetWrapper
   * @return
   */
  XiQNetWrapper *getWrapper();
  /**
   * @brief Sets the XiQNetWrapper for this peer
   * @param value
   */
  void setWrapper(XiQNetWrapper *value);

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
   * @brief Translates incoming messages to google::protobuf::Message via the XiQNetWrapper
   */
  void onReadyRead();

private:
  /**
   * @brief PIMPL pointer
   */
  XiQNetPeerPrivate *d_ptr;

  Q_DISABLE_COPY(XiQNetPeer)
  Q_DECLARE_PRIVATE(XiQNetPeer)
};

#endif // PROTOPEER_H
