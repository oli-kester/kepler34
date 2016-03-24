#include "MainWindow.hpp"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <getopt.h>

#ifdef __WIN32__
#    include "configwin32.h"
#else
#    include "Config.hpp"
#endif

//#include "font.h"
#ifdef LASH_SUPPORT
#    include "Lash.hpp"
#endif
#include "MidiFile.hpp"
#include "OptionsFile.hpp"
#include "MidiPerformance.hpp"
#include "UserFile.hpp"

/* struct for command parsing */
static struct
        option long_options[] = {

{"help", 0, 0, 'h'},
{"showmidi", 0, 0, 's'},
{"show_keys", 0, 0, 'k'},
{"stats", 0, 0, 'S'},
{"priority", 0, 0, 'p'},
{"ignore", required_argument, 0, 'i'},
{"interaction_method", required_argument, 0, 'x'},
{"jack_transport",0, 0, 'j'},
{"jack_master",0, 0, 'J'},
{"jack_master_cond", 0, 0, 'C'},
{"jack_start_mode", required_argument, 0, 'M'},
{"jack_session_uuid", required_argument, 0, 'U'},
{"manual_alsa_ports", 0, 0, 'm'},
{"pass_sysex", 0, 0, 'P'},
{"version", 0, 0, 'V'},
{0, 0, 0, 0}

};

static const char versiontext[] = PACKAGE " " VERSION "\n";

/* some default config settings stored here */

bool global_manual_alsa_ports = true;
bool global_showmidi = false;
bool global_priority = false;
bool global_device_ignore = false;
int global_device_ignore_num = 0;
bool global_stats = false;
bool global_pass_sysex = false;
QString global_filename = "";
QString last_used_dir ="/";
QString recent_files[10];
QString config_filename = ".kepler34rc";
QString user_filename = ".kepler34usr";
bool global_print_keys = false;
interaction_method_e global_interactionmethod = e_seq24_interaction;

bool global_with_jack_transport = false;
bool global_with_jack_master = false;
bool global_with_jack_master_cond = false;
bool global_jack_start_mode = true;
QString global_jack_session_uuid = "";

user_midi_bus_definition   global_user_midi_bus_definitions[c_maxBuses];
user_instrument_definition global_user_instrument_definitions[c_max_instruments];

//font *p_font_renderer;

#ifdef LASH_SUPPORT
Lash *lash_driver = NULL;
#endif

#ifdef __WIN32__
#   define HOME "HOMEPATH"
#   define SLASH "\\"
#else
#   define HOME "HOME"
#   define SLASH "/"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*prepare global MIDI definitions*/
    for ( int i=0; i<c_maxBuses; i++ )
    {
        for ( int j=0; j<16; j++ )
            global_user_midi_bus_definitions[i].instrument[j] = -1;
    }

    for ( int i=0; i<c_max_instruments; i++ )
    {
        for ( int j=0; j<128; j++ )
            global_user_instrument_definitions[i].controllers_active[j] = false;
    }


    /* Init the lash driver (strip lash specific command line
     * arguments and connect to daemon) */
#ifdef LASH_SUPPORT
    lash_driver = new Lash(&argc, &argv);
#endif

    /* the main performance object */
    /* lash must be initialized here because mastermidibus uses the global
     * lash_driver variable*/
    MidiPerformance p;

    /* config DIR */
    QString config_dir = QString(getenv(HOME)) + SLASH + ".config" + SLASH + "kepler" + SLASH;

    /* make sure .config directory exists. If not, create it */
    QDir m_qdir;
    m_qdir.setPath(config_dir);

    if (!m_qdir.exists()) {
        printf( "No config directory detected. Creating ... [%s]\n", config_dir.toAscii().constData());
        m_qdir.mkdir(config_dir);
    }

    /* read user preferences files */
    if ( getenv( HOME ) != NULL )
    {
        QString home( getenv( HOME ));
        last_used_dir = home;
        QString total_file = config_dir + config_filename;
        QFile m_qfile(total_file);
        if (m_qfile.exists())
        {
            printf( "Reading [%s]\n", total_file.toAscii().constData());

            PreferencesFile options( total_file );
            UserFile user( total_file );

            if ( !options.parse( &p ) || !user.parse( &p )){
                printf( "Error Reading [%s]\n", total_file.toAscii().constData());
            }
        }
    }
    else
        printf( "Error calling getenv( \"%s\" )\n", HOME );


    /* parse parameters */
    int c;

    while (true) {

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "C:hi:jJmM:pPsSU:Vx:", long_options,
                        &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c){

        case '?':
        case 'h':

            printf( "Usage: seq24 [OPTIONS] [FILENAME]\n\n" );
            printf( "Options:\n" );
            printf( "   -h, --help: show this message\n" );
            printf( "   -v, --version: show program version information\n" );
            printf( "   -m, --manual_alsa_ports: seq24 won't attach alsa ports\n" );
            printf( "   -s, --showmidi: dumps incoming midi events to screen\n" );
            printf( "   -p, --priority: runs higher priority with FIFO scheduler (must be root)\n" );
            printf( "   -P, --pass_sysex: passes any incoming sysex messages to all outputs \n" );
            printf( "   -i, --ignore <number>: ignore ALSA device\n" );
            printf( "   -k, --show_keys: prints pressed key value\n" );
            printf( "   -x, --interaction_method <number>: see .seq24rc for methods to use\n" );
            printf( "   -j, --jack_transport: seq24 will sync to jack transport\n" );
            printf( "   -J, --jack_master: seq24 will try to be jack master\n" );
            printf( "   -C, --jack_master_cond: jack master will fail if there is already a master\n" );
            printf( "   -M, --jack_start_mode <mode>: when seq24 is synced to jack, the following play\n" );
            printf( "                          modes are available (0 = live mode)\n");
            printf( "                                              (1 = song mode) (default)\n" );
            printf( "   -S, --stats: show statistics\n" );
            printf( "   -U, --jack_session_uuid <uuid>: set uuid for jack session\n" );
            printf( "\n\n\n" );

            return EXIT_SUCCESS;
            break;

        case 'S':
            global_stats = true;
            break;

        case 's':
            global_showmidi = true;
            break;

        case 'p':
            global_priority = true;
            break;

        case 'P':
            global_pass_sysex = true;
            break;

        case 'k':
            global_print_keys = true;
            break;

        case 'j':
            global_with_jack_transport = true;
            break;

        case 'J':
            global_with_jack_master = true;
            break;

        case 'C':
            global_with_jack_master_cond = true;
            break;

        case 'M':
            if (atoi( optarg ) > 0) {
                global_jack_start_mode = true;
            }
            else {
                global_jack_start_mode = false;
            }
            break;

        case 'm':
            global_manual_alsa_ports = true;
            break;

        case 'i':
            /* ignore alsa device */
            global_device_ignore = true;
            global_device_ignore_num = atoi( optarg );
            break;

        case 'V':
            printf("%s", versiontext);
            return EXIT_SUCCESS;
            break;

        case 'U':
            global_jack_session_uuid = QString(optarg);
            break;

        case 'x':
            global_interactionmethod = (interaction_method_e)atoi(optarg);
            break;

        default:
            break;
        }

    } /* end while */

    p.init();
    p.launch_input_thread();
    p.launch_output_thread();
    p.init_jack();

    MainWindow w;
    w.show();

    if (optind < argc)
    {
        QFile m_qfile(argv[optind]);
        if (m_qfile.exists())
        {
//            TODO restore this
//            seq24_window.open_file(argv[optind]);

        }
        else
            printf("File not found: %s\n", argv[optind]);
    }

    /* connect to lash daemon and poll events*/
#ifdef LASH_SUPPORT
    lash_driver->start(&p);
#endif

    /* main window loop */
    int exit_status = a.exec();

    /* now quitting */
    p.deinit_jack();

    if ( getenv( HOME ) != NULL )
    {
        string home( getenv( HOME ));
        QString total_file = config_dir + config_filename;
        printf( "Writing [%s]\n", total_file.toAscii().constData());

        PreferencesFile options( total_file );

        if (!options.write( &p))
            printf( "Error writing [%s]\n", total_file.toAscii().constData());
    }
    else
    {
        printf( "Error calling getenv( \"%s\" )\n", HOME );
    }

#ifdef LASH_SUPPORT
    delete lash_driver;
#endif

    return exit_status;
}
