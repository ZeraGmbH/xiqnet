######################################################################
# Automatically generated by qmake (2.01a) Tue Nov 12 10:08:53 2013
######################################################################

TEMPLATE = lib

exists( ../../project-paths.pri ) {
  include(../../project-paths.pri)
}



VERSION = 0.0.1

TARGET = proto-net-qt

QT += network

DEFINES += PROTONETQT_LIBRARY

unix:LIBS += -lprotobuf

android:LIBS+= -L/home/samuel/tmp/android-protobuf/android_libs/protobuf/obj/local/armeabi-v7a/ -lprotobuf
android:INCLUDEPATH+="/home/samuel/tmp/android-protobuf/android_libs/protobuf/jni/src/"

# Input
PUBLIC_HEADERS = \
                 protonet-qt_global.h \
                 protonetpeer.h \
                 protonetserver.h \
                 protonetwrapper.h

HEADERS += \
           $$PUBLIC_HEADERS \
           protonetpeerprivate.h \
           protonetserverprivate.h

SOURCES += \
    protonetpeer.cpp \
    protonetpeerprivate.cpp \
    protonetserver.cpp \
    protonetserverprivate.cpp


target.path = /usr/lib

header_files.files = $$PUBLIC_HEADERS
header_files.path = /usr/include
INSTALLS += header_files
INSTALLS += target

QMAKE_CXXFLAGS += -Wall -Wfloat-equal -std=c++0x
