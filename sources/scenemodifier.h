#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <QtCore/QObject>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

#include <vector>

using namespace std;

class SceneModifier : public QObject
{
    Q_OBJECT

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity,  vector < int > s, vector< vector < int > > c, int nC );
    ~SceneModifier();

private:
    int numberCuboids;
    Qt3DCore::QEntity *m_rootEntity;
    vector < Qt3DCore::QEntity * > m_cuboidEntities;
    vector < Qt3DExtras::QCuboidMesh * > cuboids;
    vector < Qt3DCore::QTransform * > cuboidTransforms;
    Qt3DExtras::QPhongMaterial * cuboidMaterial;
    vector < int > sizes;
    vector< vector < int > > centers;
};

#endif // SCENEMODIFIER_H
