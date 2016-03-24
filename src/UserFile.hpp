#pragma once

#include "MidiPerformance.hpp"
#include "ConfigFile.hpp"
#include <string>

class UserFile  : public ConfigFile
{

 public:

    UserFile(QString a_name );
    ~UserFile( );

    bool parse( MidiPerformance *a_perf );
    bool write( MidiPerformance *a_perf );
};


