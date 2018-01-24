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
    float GetTheta();


private:
    QVector3D v[3];
    QVector3D n;

    bool checkVertexIndex(int index);
};

class Model
{
public:
    Model();
    Model(const QVector<STLTriangle>& stlModel);

    bool isInitialized();
    void DeInitialize();
    void Reset();

    void SetName(QString newName);
    QString GetName();

    void AddTriangle(STLTriangle t);
    STLTriangle GetTriangle(int index);
    int GetNTriangles();

    QVector3D GetCenter();
    float xMin();
    float xMax();
    float yMin();
    float yMax();
    float zMin();
    float zMax();

private:
    QString name;
    bool initialized;
    QVector<STLTriangle> triangles;

    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float min_z;
    float max_z;
};

#endif // MODEL_H
