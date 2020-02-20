#include "BasicWidget.h"

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : 
	QOpenGLWidget(parent), 
	vboBunny_(QOpenGLBuffer::VertexBuffer), 
	vboMonkey_(QOpenGLBuffer::VertexBuffer), 
	iboBunny_(QOpenGLBuffer::IndexBuffer),
	iboMonkey_(QOpenGLBuffer::IndexBuffer),
	logger_(this), 
	bunnyObj("./objects/bunny_centered.obj"),
	monkeyObj("./objects/monkey_centered.obj")
{
  setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
	vboBunny_.release();
	vboBunny_.destroy();
	iboBunny_.release();
	iboBunny_.destroy();

	vboMonkey_.release();
	vboMonkey_.destroy();
	iboMonkey_.release();
	iboMonkey_.destroy();

	vao_.release();
	vao_.destroy();
}

//////////////////////////////////////////////////////////////////////
// Privates
QString BasicWidget::vertexShaderString() const
{
  QString str =
    "#version 330\n"
    "layout(location = 0) in vec3 position;\n"
    
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(position, 1.0);\n"
    "}\n";
  return str;
}

QString BasicWidget::fragmentShaderString() const
{
  QString str =
	"#version 330\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"  color = vec4(1.0,1.0,1.0,1.0);\n"
	"}\n";
  return str;
}

void BasicWidget::createShader()
{
  QOpenGLShader vert(QOpenGLShader::Vertex);
  vert.compileSourceCode(vertexShaderString());
  QOpenGLShader frag(QOpenGLShader::Fragment);
  frag.compileSourceCode(fragmentShaderString());
  bool ok = shaderProgram_.addShader(&vert);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.addShader(&frag);
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
  ok = shaderProgram_.link();
  if(!ok) {
	qDebug() << shaderProgram_.log();
  }
}

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  if (keyEvent->key() == Qt::Key_W) {
    qDebug() << "w pressed. Wireframe mode toggled.";
    fillMode = !fillMode;
    update();
  } else if (keyEvent->key() == Qt::Key_Q) {
    qDebug() << "q pressed. Exit application";
    exit(0);
    update(); 
  } else if (keyEvent->key() == Qt::Key_1) {
    qDebug() << "1 pressed. Render Bunny";
    monkeyMode = 0;
    update(); 
  } else if (keyEvent->key() == Qt::Key_2) {
    qDebug() << "2 pressed. Render Monkey";
    monkeyMode = 1;
    update(); 
  } else {
    qDebug() << "You Pressed an unsupported Key!";
  }
}

void BasicWidget::initializeGL()
{
  makeCurrent();
  initializeOpenGLFunctions();
  logger_.initialize();

  connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=](){
  	const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
  	for (auto msg: messages) {
  	  qDebug() << msg;
	}
  });
  logger_.startLogging();

  QOpenGLContext* curContext = this->context();
  qDebug() << "[BasicWidget]::initializeGL() -- Context Information:";
  qDebug() << "  Context Valid: " << std::string(curContext->isValid() ? "true" : "false").c_str();
  qDebug() << "  GL Version Used: " << curContext->format().majorVersion() << "." << curContext->format().minorVersion();
  qDebug() << "  Vendor: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  qDebug() << "  Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  qDebug() << "  Version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << "  GLSL Version: " << reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Initialize buffers
  vboBunny_.create();
  vboBunny_.bind();
  vboBunny_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vboBunny_.allocate(&(bunnyObj.vertices[0]), bunnyObj.vertices.size() * sizeof(GL_FLOAT));
  vboBunny_.release();

  vboMonkey_.create();
  vboMonkey_.bind();
  vboMonkey_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  vboMonkey_.allocate(&(monkeyObj.vertices[0]), monkeyObj.vertices.size() * sizeof(GL_FLOAT));
  vboMonkey_.release();
  
  iboBunny_.create();
  iboBunny_.bind();
  iboBunny_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  iboBunny_.allocate(&(bunnyObj.faceVertices[0]), bunnyObj.faceVertices.size() * sizeof(GL_UNSIGNED_INT));
  iboBunny_.release();

  iboMonkey_.create();
  iboMonkey_.bind();
  iboMonkey_.setUsagePattern(QOpenGLBuffer::StaticDraw);
  iboMonkey_.allocate(&(monkeyObj.faceVertices[0]), monkeyObj.faceVertices.size() * sizeof(GL_UNSIGNED_INT));
  iboMonkey_.release();

  vao_.create();
  createShader();

  doRender();
}

void BasicWidget::doRender() {

  shaderProgram_.bind();
  // Which buffers should we use when rendering?
  QOpenGLBuffer vbo_ = monkeyMode ? vboMonkey_ : vboBunny_; 
  QOpenGLBuffer ibo_ = monkeyMode ? iboMonkey_ : iboBunny_; 

  vao_.bind();
  vbo_.bind();

  shaderProgram_.enableAttributeArray(0);
  shaderProgram_.setAttributeBuffer(0, // location
  		  GL_FLOAT, // type
  		  0, // offset
  		  3, // tupleSize
  		  0); // Stride

  ibo_.bind();

  Obj objUsing = monkeyMode ? monkeyObj : bunnyObj;
  glDrawElements(GL_TRIANGLES, objUsing.faceVertices.size(), GL_UNSIGNED_INT, 0);
  
  vbo_.release();
  ibo_.release();
  vao_.release();
  shaderProgram_.release();
  
  glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
}

void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glPolygonMode(GL_FRONT_AND_BACK, fillMode ? GL_FILL : GL_LINE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  doRender();
}
