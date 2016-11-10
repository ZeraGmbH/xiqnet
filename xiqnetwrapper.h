#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#include <QByteArray>

namespace google
{
  namespace protobuf
  {
    class Message;
  }
}


/**
 * @brief A conveniant wrapper class for custom protobuf based classes
 *
 * It has to be implemented to use the "user specific" protocol buffers
 */
class XiQNetWrapper
{
public:
  /**
   * @brief Parses a QByteArray to create a google::protobuf::Message
   * @param t_byteArray
   * @return
   */
  virtual google::protobuf::Message *byteArrayToProtobuf(QByteArray t_byteArray) =0;

  /**
   * @brief Serializes a protobuf message to QByteArray
   * @param t_protobufMessage
   * @return
   */
  virtual QByteArray protobufToByteArray(google::protobuf::Message *t_protobufMessage) =0;
};

#endif // PROTOWRAPPER_H
