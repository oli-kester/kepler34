/* reads/writes user preferences to .kepler34rc file */

#pragma once

#include "MidiPerformance.hpp"
#include "ConfigFile.hpp"

#include "QKeyEvent"
#include "QDebug"

class PreferencesFile : public ConfigFile
{

 public:

    PreferencesFile(const QString& a_name);

    bool parse( MidiPerformance *a_perf );
    bool write( MidiPerformance *a_perf );

};


