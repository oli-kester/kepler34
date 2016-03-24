#pragma once

#ifdef __WIN32__
#include "configwin32.h"
#else
#include "Config.hpp"
#endif

#include "MidiPerformance.hpp"
#include "MidiFile.hpp"

#include <QString>
#include <QCoreApplication>
#include <QTimer>

#ifdef LASH_SUPPORT
#include <lash/lash.h>
#endif // LASH_SUPPORT

/* all the ifdef skeleton work is done in this class in such a way that any
 * other part of the code can use this class whether or not lash support is
 * actually built in (the functions will just do nothing) */

class Lash : public QObject
{
//   TODO get this working Q_OBJECT

private:
#ifdef LASH_SUPPORT
    MidiPerformance
    *m_perform;
    lash_client_t *m_client;

    QTimer timer;

    void handle_event(lash_event_t* conf);
    void handle_config(lash_config_t* conf);

private slots:
    bool process_events();

#endif // LASH_SUPPORT

public:
    Lash(int *argc, char ***argv);

    void set_alsa_client_id(int id);
    void start(MidiPerformance* MidiPerformance);
};


/* global lash driver, defined in kepler34.cpp and used in midibus.cpp*/
extern Lash *lash_driver;


