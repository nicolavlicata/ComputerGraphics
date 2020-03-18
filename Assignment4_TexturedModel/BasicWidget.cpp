#include "BasicWidget.h"
#include "Obj.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent, std::string objFilename) : QOpenGLWidget(parent), logger_(this)
{
  objFilename_ = objFilename;
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
    for (auto renderable : renderables_) {
        delete renderable;
    }
    renderables_.clear();
}

//////////////////////////////////////////////////////////////////////
// Privates
///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // Handle key events here.
  if (keyEvent->key() == Qt::Key_Left) {
    qDebug() << "Left Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else if (keyEvent->key() == Qt::Key_Right) {
    qDebug() << "Right Arrow Pressed";
    update();  // We call update after we handle a key press to trigger a redraw when we are ready
  } else if (keyEvent->key() == Qt::Key_Q) {
    qDebug() << "q pressed. Exiting application";
    exit(0);
    update(); 
  } else if (keyEvent->key() == Qt::Key_W) {
    qDebug() << "w pressed. Toggling wireframe mode.";
    fillmode = !fillmode;
    update();
  } else {
    qDebug() << "You Pressed an unsupported Key!";
  }
}
void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();

  qDebug() << QDir::currentPath();

  Obj houseObj = Obj(objFilename_);
  QString texFile = QString::fromStdString(houseObj.diffuseTextureFilename);
  QVector<QVector3D> pos;
  QVector<QVector3D> norm;
  QVector<QVector2D> texCoord;
  QVector<unsigned int> idx;

  for(int i = 0; i < houseObj.combinedPositionData.size(); i+=3) {
      pos << QVector3D(houseObj.combinedPositionData[i], houseObj.combinedPositionData[i+1], houseObj.combinedPositionData[i+2]);
  	  norm << QVector3D(0.0,0.0,0.0); // TODO when we use normals they go here.
  }
  for(int i = 0; i < houseObj.combinedTextureData.size(); i+=2) {
      texCoord << QVector2D(houseObj.combinedTextureData[i], houseObj.combinedTextureData[i+1]);
  }
  for(int i = 0; i < houseObj.combinedIndexes.size(); i++) {
      idx << houseObj.combinedIndexes[i];
  }

  Renderable* ren1 = new Renderable();
  ren1->init(pos, norm, texCoord, idx, texFile);
  ren1->setRotationAxis(QVector3D(0,1,0));

  renderables_.push_back(ren1);
  glViewport(0, 0, width(), height());
  frameTimer_.start();
}

void BasicWidget::resizeGL(int w, int h)
{
    if (!logger_.isLogging()) {
        logger_.initialize();
        // Setup the logger for real-time messaging
        connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=]() {
            const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
            for (auto msg : messages) {
                qDebug() << msg;
            }
            });
        logger_.startLogging();
    }
  glViewport(0, 0, w, h);
  view_.setToIdentity();
  view_.lookAt(QVector3D(5.0f, 0.0f, 2.0f),
      QVector3D(0.0f, 0.0f, 0.0f),
      QVector3D(0.0f, 1.0f, 0.0f));
  projection_.setToIdentity();
  projection_.perspective(70.f, (float)w/(float)h, 0.001, 1000.0);
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  qint64 msSinceRestart = frameTimer_.restart();
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, fillmode ? GL_FILL : GL_LINE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto renderable : renderables_) {
      renderable->update(msSinceRestart);
      renderable->draw(view_, projection_);
  }
  update();
}
