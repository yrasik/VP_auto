#-------------------------------------------------
#
# Project created by QtCreator 2010-11-04T17:01:01
#
#-------------------------------------------------

#QT       += core
QT       += core gui
QT += widgets

#QT       += gui

TARGET = vp_auto
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app



INCLUDEPATH += $${_PRO_FILE_PWD_}/per/pcad/for_doc_includes
INCLUDEPATH += $${_PRO_FILE_PWD_}/per/pcad/for_sch


SOURCES += main.cpp \
    element.cpp \
    vp.cpp \
    do_element.cpp \
    util_element.cpp \
    sp.cpp \
    pilot.cpp \
    cmdline.c \
    per/pcad/for_doc_includes/pcad_per_for_doc_includes.cpp \
    dot_export.cpp \
    per_to_log.cpp \
    per/pcad/for_sch/pcad_per_for_sch.cpp \
    similarity.cpp \
    true_elements.cpp

HEADERS += \
    element.h \
    vp.h \
    do_element.h \
    util_element.h \
    sp.h \
    pilot.h \
    cmdline.h \
    per/pcad/for_doc_includes/pcad_per_for_doc_includes.h \
    dot_export.h \
    per_to_log.h \
    per/pcad/for_sch/pcad_per_for_sch.h \
    similarity.h \
    firm_name.h \
    text_name.h \
    text_width.h \
    true_elements.h
