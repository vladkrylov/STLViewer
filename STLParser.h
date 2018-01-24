#ifndef STLPARSER_H
#define STLPARSER_H

#include <QtCore>
#include "Model.h"

class STLParser
{
public:
    STLParser();

    Model parse(QFile& file);

private:
    enum parser_status {PARSE_OK, PARSE_FAILED};
};

#endif // STLPARSER_H
