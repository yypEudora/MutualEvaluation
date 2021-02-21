QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dealLogin.cpp \
    dealRegist.cpp \
    main.cpp \
    mainwindow.cpp \
    mysql.cpp \
    student/deal_stu_info.cpp \
    student/stu_main.cpp \
    teacher/deal_tc_info.cpp \
    teacher/tc_main.cpp

HEADERS += \
    dealLogin.h \
    dealRegist.h \
    mainwindow.h \
    student/dealRegist.h \
    mysql.h \
    student/deal_stu_info.h \
    student/stu_main.h \
    teacher/deal_tc_info.h \
    teacher/tc_main.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
LIBS += -L/usr/local/lib -lhiredis
