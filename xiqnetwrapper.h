#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#include <QByteArray>
#include <google/protobuf/message.h>


/**
 * @brief A conveniant wrapper class
 *
 * It has to be implemented to use the "user specific" protocol buffers
 */
class XiQNetWrapper
{
public:
  /**
   * @brief Parses a QByteArray to create a google::protobuf::Message
   * @param bA
   * @return
   */
  virtual google::protobuf::Message *byteArrayToProtobuf(QByteArray bA) =0;

  /**
   * @brief Serializes a protobuf message to QByteArray
   * @param pMessage
   * @return
   */
  virtual QByteArray protobufToByteArray(google::protobuf::Message *pMessage) =0;
};

#endif // PROTOWRAPPER_H
