#include "STLParser.h"

STLParser::STLParser()
{

}

Model STLParser::parse(QFile& file)
{
    Model m;
    STLTriangle t;
    parser_status status = PARSE_OK;
    int vertexCounter = 0;
    int linesCounter = 0;

    while (!file.atEnd() && status != PARSE_FAILED) {
        QString line = file.readLine().trimmed();
        QStringList parts = line.split(' ', QString::SkipEmptyParts);
        // skip blank liness
        if (parts.length() == 0) {
            continue;
        } else {
            ++linesCounter;
        }
        // check if first non-blank line starts properly
        if (linesCounter == 0 && !line.startsWith("solid")) {
            status = PARSE_FAILED;
            break;
        }
        // set model name
        if (parts[0] == "solid") {
            if (parts.length() > 2) {
                QStringList nameParts = parts;
                nameParts.removeFirst();
                m.SetName(nameParts.join(' '));
            }
        }
        // start facet
        else if (line.startsWith("facet normal")) {
            if (parts.length() != 5) {
                status = PARSE_FAILED;
                break;
            }
            bool ok = true;
            float nx = parts[2].toFloat(&ok);
            float ny = parts[3].toFloat(&ok);
            float nz = parts[4].toFloat(&ok);
            if (!ok) {
                status = PARSE_FAILED;
                break;
            }
            t.Reset();
            t.SetNormal(nx, ny, nz);
        }
        // start outer loop
        else if (line.startsWith("outer loop")) {
            // TODO: set outer loop status
            vertexCounter = 0;
        }
        // new vertex
        else if (parts[0] == "vertex") {
            // TODO: check outer loop status
            if (parts.length() != 4) {
                status = PARSE_FAILED;
                continue;
            }
            bool ok = true;
            float vx = parts[1].toFloat(&ok);
            float vy = parts[2].toFloat(&ok);
            float vz = parts[3].toFloat(&ok);
            if (!ok) {
                status = PARSE_FAILED;
                break;
            }
            if (vertexCounter > 2) {
                status = PARSE_FAILED;
                break;
            }
            t.SetVertex(vertexCounter++, QVector3D(vx, vy, vz));
        }
        // end outer loop
        else if (parts[0] == "endloop") {
            // TODO: check outer loop status
            if (parts.length() != 1) {
                status = PARSE_FAILED;
                break;
            }
        }
        // end outer loop
        else if (parts[0] == "endfacet") {
            // TODO: check outer loop status
            if (parts.length() != 1) {
                status = PARSE_FAILED;
                break;
            }
            m.AddTriangle(t);
            t.Reset();
        }
    }

    if (status == PARSE_FAILED) {
        m.DeInitialize();
        qDebug() << "Something wrong with the model";
    }
    return m;
}
