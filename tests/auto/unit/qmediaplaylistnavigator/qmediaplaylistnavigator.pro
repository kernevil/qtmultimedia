CONFIG += testcase
TARGET = tst_qmediaplaylistnavigator

QT += multimedia-private testlib
CONFIG += no_private_qt_headers_warning

SOURCES += tst_qmediaplaylistnavigator.cpp

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
