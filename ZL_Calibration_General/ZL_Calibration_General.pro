QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    image.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CameraControllerView.h \
    CameraOutput.h \
    ImageItem.h \
    Image_base.h \
    exception.hpp \
    global.hpp \
    image.h \
    image_conversion.hpp \
    image_private.hpp \
    mainwindow.h \
    ChannelViewerWidget.h

FORMS += \
    mainwindow.ui
INCLUDEPATH += D:\\Boost\\include\\boost-1_79
LIBS += -LD:/Boost/lib -lboost_system

INCLUDEPATH += D:\\opencv-4.5.1\\build\\include
LIBS += -LD:\\opencv-4.5.1\\build\\x64\\vc15\\lib -lopencv_world451d

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
