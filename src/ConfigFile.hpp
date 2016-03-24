#pragma once

#include "MidiPerformance.hpp"
#include <fstream>
#include <string>
#include <list>

class ConfigFile
{

 protected:

    int m_pos;
    QString m_name;

    /* holds our data */
    unsigned char *m_d;

    list<unsigned char> m_l;

    char m_line[1024];

    bool m_done;

    void next_data_line( ifstream *a_file);
    void line_after( ifstream *a_file, string a_tag);

 public:

    ConfigFile(const QString& a_name);
    virtual ~ConfigFile();

    virtual bool parse( MidiPerformance *a_perf ) = 0;
    virtual bool write( MidiPerformance *a_perf ) = 0;

};


