TEMPLATE =   app
TARGET = genOctData
DEPENDPATH =   .
INCLUDEPATH =   .
QT += widgets
FORMS =   genScene.ui
RESOURCES =   gen.qrc
HEADERS =   genScene.h
SOURCES =   genScene.cpp \
  main.cpp
#config = 
unix {
  DEFINES =     HAVE_CONFIG_H
  LIBPATH =     /usr/local/lib
  LIBS =     -lCoin \
    -lSoQt
  INCLUDEPATH =     /usr/include
}
win32 {
  DEFINES =     COIN_DLL \
    SOQT_DLL \
    _WIN32
  LIBPATH =     $(COINDIR)/lib
    LIBS =       -lCoin2 \
      -lSoQt1
    INCLUDEPATH =       $(COINDIR)/include
}
CONFIG +=   release
