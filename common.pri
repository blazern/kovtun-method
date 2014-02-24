QMAKE_CXXFLAGS += -std=c++11

QT += qml quick widgets

SOURCES += \
    $$PWD/main/cppsrc/Executer.cpp \
    $$PWD/main/cppsrc/ClosedContour.cpp \
    $$PWD/main/cppsrc/Painter.cpp \
    $$PWD/main/cppsrc/ExecuterQmlInterface.cpp \
    $$PWD/main/cppsrc/RectangleToolKit.cpp \
    $$PWD/main/cppsrc/MyQRectF.cpp \
    $$PWD/main/cppsrc/ColorsDictionary.cpp \
    $$PWD/main/cppsrc/ExecuterFileLogger.cpp \
    $$PWD/main/cppsrc/UnitsToolKit.cpp \
    $$PWD/main/cppsrc/ExecuterProgressWatcher.cpp \
    $$PWD/main/cppsrc/ClosedContourParser.cpp \
    $$PWD/main/cppsrc/ClosedContourSetterQmlInterface.cpp

HEADERS += \
    $$PWD/main/cppsrc/Executer.h \
    $$PWD/main/cppsrc/ClosedContour.h \
    $$PWD/main/cppsrc/Painter.h \
    $$PWD/main/cppsrc/ExecuterQmlInterface.h \
    $$PWD/main/cppsrc/RectangleToolKit.h \
    $$PWD/main/cppsrc/MyQRectF.h \
    $$PWD/main/cppsrc/ColorsDictionary.h \
    $$PWD/main/cppsrc/ExecuterListener.h \
    $$PWD/main/cppsrc/ExecuterFileLogger.h \
    $$PWD/main/cppsrc/NotSingleShape.h \
    $$PWD/main/cppsrc/UnitsToolKit.h \
    $$PWD/main/cppsrc/ExecuterProgressWatcher.h \
    $$PWD/main/cppsrc/ClosedContourParser.h \
    $$PWD/main/cppsrc/ClosedContourSetterQmlInterface.h
