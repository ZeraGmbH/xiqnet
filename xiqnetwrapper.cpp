#include "xiqnetwrapper.h"
#include <netmessages.pb.h>

QByteArray XiQNetWrapper::protobufToByteArray(const google::protobuf::Message &t_protobufMessage)
{
  return QByteArray(t_protobufMessage.SerializeAsString().c_str(), t_protobufMessage.ByteSizeLong());
}
