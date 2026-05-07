QT += core widgets qml quick quickwidgets
CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
    PairModel.cpp \
    main.cpp \
    SyncEngine.cpp \
    FileWatcher.cpp \
    DebounceManager.cpp \
    SyncWorker.cpp \
    TrayApp.cpp \
    ConfigDialog.cpp

HEADERS += \
    PairModel.h \
    SyncEngine.h \
    FileWatcher.h \
    DebounceManager.h \
    SyncWorker.h \
    TrayApp.h \
    config.h \
    ConfigDialog.h

RESOURCES += resources.qrc
