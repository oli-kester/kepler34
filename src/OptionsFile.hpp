/* reads/writes user preferences to .kepler34rc file */

#pragma once

#include "Perform.hpp"
#include "ConfigFile.hpp"

class PreferencesFile : public ConfigFile
{

 public:

    PreferencesFile(const QString& a_name);

    bool parse( Perform *a_perf );
    bool write( Perform *a_perf );

};


