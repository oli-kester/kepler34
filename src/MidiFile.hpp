#pragma once

#include "MidiPerformance.hpp"

#include <fstream>
#include <list>
#include <vector>
#include <QString>

class MidiFile
{

 private:
    
    int m_pos;
    const QString m_name;

    /* holds our data */
	std::vector<unsigned char> m_d;
    
    list<unsigned char> m_l;

    unsigned long read_long();
    unsigned short read_short();
    unsigned char read_byte();
    unsigned long read_var();

    void write_long( unsigned long );
    void write_short( unsigned short );
    void write_byte( unsigned char );

 public:

    MidiFile(const QString&);
 
    ~MidiFile();

    bool parse( MidiPerformance *a_perf, int a_screen_set );
    bool write( MidiPerformance *a_perf );

};


