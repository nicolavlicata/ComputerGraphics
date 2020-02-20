#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include "Obj.h"

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

private:
  // Initially show the bunny
  bool monkeyMode = 0;
  // Start with line mode to show the full render
  bool fillMode = 1;

  Obj bunnyObj;
  Obj monkeyObj;

  QString vertexShaderString() const;
  QString fragmentShaderString() const;
  void createShader();
  QOpenGLVertexArrayObject vao_;
  void doRender();

protected:
  // Required interaction overrides
  void keyReleaseEvent(QKeyEvent* keyEvent) override;

  // Required overrides form QOpenGLWidget
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  QOpenGLBuffer vboBunny_;
  QOpenGLBuffer vboMonkey_;
  QOpenGLBuffer iboBunny_;
  QOpenGLBuffer iboMonkey_;

  QOpenGLShaderProgram shaderProgram_;
  QOpenGLDebugLogger logger_;

public:
  BasicWidget(QWidget* parent=nullptr);
  virtual ~BasicWidget();
  
  // Make sure we have some size that makes sense.
  QSize sizeHint() const {return QSize(800,600);}
};
