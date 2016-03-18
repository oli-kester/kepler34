#include "ConfigFile.hpp"
#include <iostream>

ConfigFile::ConfigFile(const QString &a_name) :
    m_pos(0),
    m_name(a_name)
{
}

ConfigFile::~ConfigFile( )
{
}

void
ConfigFile::next_data_line( ifstream *a_file)
{
    a_file->getline( m_line, sizeof(m_line) );
    while (( m_line[0] == '#' || m_line[0] == ' ' || m_line[0] == 0 ) &&
           !a_file->eof() )
    {
        a_file->getline( m_line, sizeof(m_line) );
    }
}

void
ConfigFile::line_after( ifstream *a_file, string a_tag)
{
    /* run to start */
    a_file->clear();
    a_file->seekg( 0, ios::beg );

    a_file->getline( m_line, sizeof(m_line) );
    while ( strncmp( m_line, a_tag.c_str(), a_tag.length()) != 0  &&
            !a_file->eof() )
    {
        a_file->getline( m_line, sizeof(m_line) );
    }
    next_data_line( a_file );
}

