#include "Lash.hpp"

Lash::Lash(int *argc, char ***argv)
{
#ifdef LASH_SUPPORT
    m_perform = NULL;

    m_client = lash_init(lash_extract_args(argc, argv), PACKAGE_NAME,
            LASH_Config_File, LASH_PROTOCOL(2, 0));

    if (m_client == NULL) {
        fprintf(stderr, "Failed to connect to LASH.  "
                "Session management will not occur.\n");
    } else {
        lash_event_t* event = lash_event_new_with_type(LASH_Client_Name);
        lash_event_set_string(event, "Seq24");
        lash_send_event(m_client, event);
        printf("[Connected to LASH]\n");
    }
#endif // LASH_SUPPORT
}


void
Lash::set_alsa_client_id(int id)
{
#ifdef LASH_SUPPORT
    lash_alsa_client_id(m_client, id);
#endif
}


void
Lash::start(MidiPerformance* perform)
{
#ifdef LASH_SUPPORT
    m_perform = perform;
    /* Process any LASH events every 250 msec (arbitrarily chosen interval) */
    timer.start(250);
    //FIXME lash timer slot gives error
    QObject::connect(&timer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(process_events()));
#endif // LASH_SUPPORT
}


#ifdef LASH_SUPPORT

bool
Lash::process_events()
{
    lash_event_t *ev = NULL;

    // Process events
    while ((ev = lash_get_event(m_client)) != NULL) {
        handle_event(ev);
        lash_event_destroy(ev);
    }

    return true;
}


void
Lash::handle_event(lash_event_t* ev)
{
    LASH_Event_Type type   = lash_event_get_type(ev);
    const char      *c_str = lash_event_get_string(ev);
    QString     str    = (c_str == NULL) ? "" : c_str;

    if (type == LASH_Save_File) {
        MidiFile f(str + "/kepler34.mid");
        f.write(m_perform);
        lash_send_event(m_client, lash_event_new_with_type(LASH_Save_File));
    } else if (type == LASH_Restore_File) {
        MidiFile f(str + "/kepler34.mid");
        f.parse(m_perform, 0);
        lash_send_event(m_client, lash_event_new_with_type(LASH_Restore_File));
    } else if (type == LASH_Quit) {
        m_client = NULL;
    QCoreApplication::exit(0);

    } else {
        fprintf(stderr, "Warning:  Unhandled LASH event.\n");
    }
}


void
Lash::handle_config(lash_config_t* conf)
{
    const char *key     = NULL;
    const void *val     = NULL;
    size_t     val_size = 0;

    key      = lash_config_get_key(conf);
    val      = lash_config_get_value(conf);
    val_size = lash_config_get_value_size(conf);
}


#endif // LASH_SUPPORT
