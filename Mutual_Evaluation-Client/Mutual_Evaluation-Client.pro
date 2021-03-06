QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common/common.cpp \
    common/des.c \
    login.cpp \
    main.cpp \
    student/stu_info.cpp \
    student/stu_login_instance.cpp \
    student/stu_mainwindow.cpp \
    student/stu_pwd.cpp \
    teacher/tc_info.cpp \
    teacher/tc_login_instance.cpp \
    teacher/tc_mainwindow.cpp \
    teacher/tc_pwd.cpp \
    titlewg.cpp \
    zhujiao/zj_mainwindow.cpp

HEADERS += \
    common/common.h \
    common/des.h \
    login.h \
    student/stu_info.h \
    student/stu_login_instance.h \
    student/stu_mainwindow.h \
    student/stu_pwd.h \
    teacher/tc_info.h \
    teacher/tc_login_instance.h \
    teacher/tc_mainwindow.h \
    teacher/tc_pwd.h \
    titlewg.h \
    zhujiao/zj_mainwindow.h

FORMS += \
    login.ui \
    student/stu_info.ui \
    student/stu_mainwindow.ui \
    student/stu_pwd.ui \
    teacher/tc_info.ui \
    teacher/tc_mainwindow.ui \
    teacher/tc_pwd.ui \
    titlewg.ui \
    zhujiao/zj_mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
LIBS += -L/usr/local/lib -lhiredis
