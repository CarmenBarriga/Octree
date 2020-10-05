#include "scenemodifier.h"

#include <QtCore/QDebug>

#include <vector>

using namespace std;


SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity, vector < int > s, vector < vector < int > > c, int nC)
    : numberCuboids(nC), m_rootEntity(rootEntity), sizes(s), centers(c)
{
    cuboids.resize(numberCuboids);
    cuboidTransforms.resize(numberCuboids);
    m_cuboidEntities.resize(numberCuboids);

    for (int i = 0; i < numberCuboids; ++i){
        cuboids[i] = new Qt3DExtras::QCuboidMesh();
        cuboidTransforms[i] = new Qt3DCore::QTransform();
        cuboidTransforms[i]->setScale(sizes[i]);
        cuboidTransforms[i]->setTranslation(QVector3D(centers[i][0], centers[i][1], centers[i][2]));
    }
    cuboidMaterial = new Qt3DExtras::QPhongMaterial();
    cuboidMaterial->setDiffuse(QColor(QRgb(0x665423)));

    for (int i = 0; i < numberCuboids; ++i){
        m_cuboidEntities[i] = new Qt3DCore::QEntity(m_rootEntity);
        m_cuboidEntities[i]->addComponent(cuboids[i]);
        m_cuboidEntities[i]->addComponent(cuboidMaterial);
        m_cuboidEntities[i]->addComponent(cuboidTransforms[i]);
    }
}

SceneModifier::~SceneModifier()
{
}