#pragma once

#include "Perform.hpp"
#include "configfile.h"
#include <string>

class userfile  : public configfile
{

 public:

    userfile(QString a_name );
    ~userfile( );

    bool parse( perform *a_perf );
    bool write( perform *a_perf );
};


