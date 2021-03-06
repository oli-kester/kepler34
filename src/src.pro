#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T14:41:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kepler34
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
    UserFile.cpp \
    MidiPerformance.cpp \
    BeatIndicator.cpp \
    EditKeys.cpp \
    EditTimeBar.cpp \
    EditNoteRoll.cpp \
    SongTimeBar.cpp \
    SongSequenceNames.cpp \
    SongSequenceGrid.cpp \
    PreferencesFile.cpp \
    KeplerStyle.cpp \
    EditEventValues.cpp \
    EditEventTriggers.cpp \
    AboutDialog.cpp

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
    MidiPerformance.hpp \
    BeatIndicator.hpp \
    EditKeys.hpp \
    EditTimeBar.hpp \
    EditNoteRoll.hpp \
    SongTimeBar.hpp \
    SongSequenceNames.hpp \
    SongSequenceGrid.hpp \
    PreferencesFile.hpp \
    KeplerStyle.hpp \
    EditEventValues.hpp \
    EditEventTriggers.hpp \
    AboutDialog.hpp \
    seq24Rect.hpp

FORMS    += \
    MainWindow.ui \
    LiveFrame.ui \
    SongFrame.ui \
    EditFrame.ui \
    PreferencesDialog.ui \
    AboutDialog.ui

RESOURCES += \
    kepler34.qrc

unix:!macx: LIBS += -lasound -llash -ljack -lrt

# This is where lash is stored on certain Linux distros,
# so we must check here too
INCLUDEPATH += /usr/include/lash-1.0


