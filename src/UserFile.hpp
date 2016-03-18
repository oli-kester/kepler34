#pragma once

#include "Perform.hpp"
#include "ConfigFile.hpp"
#include <string>

class UserFile  : public ConfigFile
{

 public:

    UserFile(QString a_name );
    ~UserFile( );

    bool parse( Perform *a_perf );
    bool write( Perform *a_perf );
};


