#ifndef PROTOWRAPPER_H
#define PROTOWRAPPER_H

#include <QByteArray>
#include <google/protobuf/message.h>


class ProtoNetWrapper
{
public:
  virtual google::protobuf::Message *byteArrayToProtobuf(QByteArray bA) =0;
  virtual QByteArray protobufToByteArray(google::protobuf::Message *pMessage) =0;
};

#endif // PROTOWRAPPER_H
