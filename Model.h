#ifndef MODEL_H
#define MODEL_H

#include <QtCore>
#include <QVector3D>

class STLTriangle
{
public:
    STLTriangle();

    void SetVertex(int index, QVector3D point3D);
    QVector3D GetVertex(int index);
    void SetNormal(float nx, float ny, float nz);
    QVector3D GetNormal();

    void Reset();
    float GetArea();


private:
    QVector3D v[3];
    QVector3D n;

    bool checkVertexIndex(int index);
};

class Model
{
public:
    Model();
    Model(QVector<STLTriangle> stlModel);

    void AddTriangle(STLTriangle t);
    STLTriangle GetTriangle(int index);
    size_t GetNTriangles();
    void Reset();
    bool isInitialized();

private:
    bool initialized;
    QVector<STLTriangle> triangles;
};

#endif // MODEL_H
