/* reads/writes user preferences to .kepler34rc file */

#pragma once

#include "Perform.hpp"
#include "configfile.h"

class optionsfile : public configfile
{

 public:

    optionsfile(const QString& a_name);

    bool parse( perform *a_perf );
    bool write( perform *a_perf );

};


