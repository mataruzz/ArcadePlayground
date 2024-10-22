QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += release

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Tetris/tetrisboard.cpp \
    Tetris/tetrispiece.cpp \
    Tetris/tetriswindow.cpp \
    TicTacToe/tictactoeboard.cpp \
    TicTacToe/tictactoewindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Tetris/tetrisboard.h \
    Tetris/tetrispiece.h \
    Tetris/tetriswindow.h \
    TicTacToe/tictactoeboard.h \
    TicTacToe/tictactoewindow.h \
    mainwindow.h

FORMS += \
    TicTacToe/board_form.ui \
    TicTacToe/tictactoe_copy.ui \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
