QMAKE_CXXFLAGS += -std=c++11

QT += qml quick widgets

SOURCES += \
    $$PWD/main/cppsrc/KovtunMethodExecuter.cpp \
    $$PWD/main/cppsrc/Contour.cpp \
    $$PWD/main/cppsrc/KovtunMethodPainter.cpp \
    $$PWD/main/cppsrc/KovtunMethodExecuterQmlInterface.cpp \
    $$PWD/main/cppsrc/RectangleToolKit.cpp

HEADERS += \
    $$PWD/main/cppsrc/KovtunMethodExecuter.h \
    $$PWD/main/cppsrc/Contour.h \
    $$PWD/main/cppsrc/KovtunMethodPainter.h \
    $$PWD/main/cppsrc/KovtunMethodExecuterQmlInterface.h \
    $$PWD/main/cppsrc/RectangleToolKit.h
