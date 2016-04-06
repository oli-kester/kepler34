#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T14:41:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kepler
TEMPLATE = app


SOURCES +=\
    Main.cpp \
    MainWindow.cpp \
    LiveFrame.cpp \
    SongFrame.cpp \
    EditFrame.cpp \
    PreferencesDialog.cpp \
    MidiSequence.cpp \
    MidiEvent.cpp \
    Mutex.cpp \
    MidiBus.cpp \
    Lash.cpp \
    ConfigFile.cpp \
    MidiFile.cpp \
    OptionsFile.cpp \
    UserFile.cpp \
    MidiPerformance.cpp \
    BeatIndicator.cpp \
    EditKeys.cpp \
    EditTimeBar.cpp \
    EditNoteRoll.cpp

HEADERS  += \
    MainWindow.hpp \
    LiveFrame.hpp \
    SongFrame.hpp \
    EditFrame.hpp \
    PreferencesDialog.hpp \
    MidiSequence.hpp \
    MidiTrigger.hpp \
    MidiEvent.hpp \
    Globals.hpp \
    MidiBus.hpp \
    Mutex.hpp \
    Lash.hpp \
    UserFile.hpp \
    ConfigFile.hpp \
    Config.hpp \
    MidiFile.hpp \
    OptionsFile.hpp \
    MidiPerformance.hpp \
    BeatIndicator.hpp \
    EditKeys.hpp \
    EditTimeBar.hpp \
    EditNoteRoll.hpp

FORMS    += \
    MainWindow.ui \
    LiveFrame.ui \
    SongFrame.ui \
    EditFrame.ui \
    PreferencesDialog.ui

RESOURCES += \
    kepler34.qrc

unix:!macx: LIBS += -lasound -llash -ljack -lrt
