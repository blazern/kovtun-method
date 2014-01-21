QMAKE_CXXFLAGS += -std=c++11

QT += qml quick widgets

SOURCES += \
    $$PWD/main/cppsrc/KovtunMethodExecuter.cpp \
    $$PWD/main/cppsrc/ClosedContour.cpp \
    $$PWD/main/cppsrc/KovtunMethodPainter.cpp \
    $$PWD/main/cppsrc/KovtunMethodExecuterQmlInterface.cpp \
    $$PWD/main/cppsrc/RectangleToolKit.cpp \
    $$PWD/main/cppsrc/KovtunQRectF.cpp \
    $$PWD/main/cppsrc/ColorsDictionary.cpp \
    $$PWD/main/cppsrc/KovtunMethodExecuterFileLogger.cpp

HEADERS += \
    $$PWD/main/cppsrc/KovtunMethodExecuter.h \
    $$PWD/main/cppsrc/ClosedContour.h \
    $$PWD/main/cppsrc/KovtunMethodPainter.h \
    $$PWD/main/cppsrc/KovtunMethodExecuterQmlInterface.h \
    $$PWD/main/cppsrc/RectangleToolKit.h \
    $$PWD/main/cppsrc/KovtunQRectF.h \
    $$PWD/main/cppsrc/ColorsDictionary.h \
    $$PWD/main/cppsrc/KovtunMethodExecuterListener.h \
    $$PWD/main/cppsrc/KovtunMethodExecuterFileLogger.h
