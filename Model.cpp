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
    for(size_t i=0; i<3; ++i) {
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
    initialized = false;
}

Model::Model(QVector<STLTriangle> stlModel)
{
    if (stlModel.size() > 0) {
        initialized = true;
        triangles = stlModel;
    }
}

void Model::Reset()
{
    triangles.clear();
    initialized = false;
}

bool Model::isInitialized()
{
    return initialized;
}

void Model::AddTriangle(STLTriangle t)
{
    if (!initialized) {
        initialized = true;
    }
    triangles.push_back(t);
}

STLTriangle Model::GetTriangle(int index)
{
    return triangles.at(index);
}

size_t Model::GetNTriangles()
{
    return triangles.size();
}
