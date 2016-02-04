/* reads/writes user preferences to .kepler34rc file */

#pragma once

#include "perform.h"
#include "configfile.h"

class optionsfile : public configfile
{

 public:

    optionsfile(const Glib::ustring& a_name);

    bool parse( perform *a_perf );
    bool write( perform *a_perf );

};


