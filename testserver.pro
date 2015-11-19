CONFIG += 	qt debug
HEADERS       = server.h \
    dragon/core_opcode.h \
    dragon/edfa_common.h \
    dragon/edfa_opcode.h
SOURCES       = server.cpp \
                main.cpp
QT           += network

