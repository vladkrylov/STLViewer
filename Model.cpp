#include <limits>
#include "Model.h"

// ################################################
// ######### STLTriangle class desciption #########
// ################################################

STLTriangle::STLTriangle()
{
    Reset();
}

void STLTriangle::Reset()
{
    n = QVector3D(0.f, 0.f, 0.f);
    for(int i=0; i<3; ++i) {
        v[i] = QVector3D(0.f, 0.f, 0.f);
    }
}

bool STLTriangle::checkVertexIndex(int index)
{
    if (index < 0 || index > 2) {
        qDebug() << "CRITICAL: invalid index provided to STLTriangle::SetVertex()!";
        return false;
    }
    return true;
}

float STLTriangle::GetArea()
{
    QVector3D AB = v[0] - v[1];
    QVector3D AC = v[0] - v[2];
    float area = 0.5 * QVector3D::crossProduct(AB, AC).length();
    return area;
}

float STLTriangle::GetTheta()
{
    if (fabs(n.z()) < 0.0001) {
        return 0.;
    }
    float theta = qAtan(n.z() / sqrt(n.x()*n.x() + n.y()*n.y()));
    return qRadiansToDegrees(theta);
}

void STLTriangle::SetNormal(float nx, float ny, float nz)
{
    n = QVector3D(nx, ny, nz);
}

void STLTriangle::SetVertex(int index, QVector3D point3D)
{
    if (!checkVertexIndex(index)) {
        return;
    }
    v[index] = point3D;
}

QVector3D STLTriangle::GetVertex(int index)
{
    if (!checkVertexIndex(index)) {
        return QVector3D();
    }
    return v[index];
}

QVector3D STLTriangle::GetNormal()
{
    return n;
}

// ################################################
// ############ Model class desciption ############
// ################################################

Model::Model()
{
    Reset();
}

void Model::Reset()
{
    name = "";
    triangles.clear();
    initialized = false;

    const float MAXIMUM_POSSIBLE = std::numeric_limits<float>::max();
    const float MINIMUM_POSSIBLE = std::numeric_limits<float>::min();
    min_x = MAXIMUM_POSSIBLE;
    max_x = MINIMUM_POSSIBLE;
    min_y = MAXIMUM_POSSIBLE;
    max_y = MINIMUM_POSSIBLE;
    min_z = MAXIMUM_POSSIBLE;
    max_z = MINIMUM_POSSIBLE;
}

void Model::SetName(QString newName)
{
    name = newName;
}

QString Model::GetName()
{
    return name;
}

bool Model::isInitialized()
{
    return initialized;
}

void Model::DeInitialize()
{
    initialized = false;
}

void Model::AddTriangle(STLTriangle t)
{
    if (!initialized) {
        initialized = true;
    }

    QVector3D v;
    for(int i=0; i<3; ++i) {
        v = t.GetVertex(i);
        if (v.x() < min_x) min_x = v.x();
        if (v.x() > max_x) max_x = v.x();
        if (v.y() < min_y) min_y = v.y();
        if (v.y() > max_y) max_y = v.y();
        if (v.z() < min_z) min_z = v.z();
        if (v.z() > max_z) max_z = v.z();
    }
    triangles.push_back(t);
}

STLTriangle Model::GetTriangle(int index)
{
    return triangles.at(index);
}

int Model::GetNTriangles()
{
    return triangles.size();
}

QVector3D Model::GetCenter()
{
    return QVector3D((min_x+max_x)/2., (min_y+max_y)/2., (min_z+max_z)/2.);
}

float Model::xMin()
{
    return min_x;
}

float Model::xMax()
{
    return max_x;
}

float Model::yMin()
{
    return min_y;
}

float Model::yMax()
{
    return max_y;
}

float Model::zMin()
{
    return min_z;
}

float Model::zMax()
{
    return max_z;
}

