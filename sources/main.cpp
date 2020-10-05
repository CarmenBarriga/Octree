#include "scenemodifier.h"
#include "Octree.h"

#include <QGuiApplication>
#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>
#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>
#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#define printVec(v) for(auto x:v){cout<<x<<" ";}cout<<"\n";
#define printMat(m) for(auto x:m){printVec(x);}

vector < int > sizes;
vector < vector < int > > centers;

using namespace std;

vector < vector < int > > readPoints(){
  ifstream input ("../files/points.obj");
  vector < vector < int > > points;
  string s;
  float x, y, z;
  if (input){
    cout << "File Opened\n";
  }
  else{
   cout << "FILE NOT OPENED\n";
  }
  while (!input.eof()){
    input >> s;
    if(s != "v") break;
    input >> x >> y >> z;
    x *= 100; y *= 100; z *= 100; // This allows working with points of integer type
    x = int(x); y = int(y); z = int(z);
    vector < int > tmp(3);
    tmp[0] = x; tmp[1] = y; tmp[2]=z;
    points.emplace_back(tmp);
  }
  return points;
}

void insertPoints (Octree < int > & octree, vector < vector < int > > points){
  for (int i = 0; i < points.size(); ++i) {
    Point < int > p (points[i][0], points[i][1], points[i][2]);
    Node < int > * ptr = octree.root;
    octree.insert(p, ptr, 1);
  }
}

void getData(Node < int > * ptr){
    if(ptr == NULL){return;}
    if(ptr->numberCurrentPoints >= ptr->numberPoints){
        int size;
        vector < int > coordinates (3);
        coordinates[0] = (ptr->limit1.x  + ptr->limit2.x  ) / 2;
        coordinates[1] = (ptr->limit1.y + ptr->limit2.y  ) / 2;
        coordinates[2] = (ptr->limit1.z  + ptr->limit2.z ) / 2;
        size = (ptr->limit2.x - ptr->limit1.x);
        sizes.emplace_back(size);
        centers.emplace_back(coordinates);
    }else{
        for(int i = 0; i < 8; ++i){
            getData(ptr->children[i]);
        }
    }
}


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    QWidget *container = QWidget::createWindowContainer(view);
    QSize screenSize = view->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    widget->setWindowTitle(QStringLiteral("Octree Visualization"));

    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

    Qt3DRender::QCamera *cameraEntity = view->camera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(400, 600, 800));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(-500, -300, -100));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);

    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(cameraEntity);

    Point < int > A (-500, -300, -100), B (400, 600, 800);  // lower and upper bound of the initial space
    Octree < int > octree (A, B);
    vector < vector < int > > points = readPoints();
    insertPoints(octree, points);

    SceneModifier *modifier = new SceneModifier(rootEntity, sizes, centers, sizes.size());
    view->setRootEntity(rootEntity);

    widget->show();
    widget->resize(1200, 800);

    return app.exec();
}
