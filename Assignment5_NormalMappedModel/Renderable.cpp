#include "Renderable.h"

#include <QtGui>
#include <QtOpenGL>

Renderable::Renderable() : vbo_(QOpenGLBuffer::VertexBuffer), ibo_(QOpenGLBuffer::IndexBuffer), texture_(QOpenGLTexture::Target2D), normal_(QOpenGLTexture::Target2D), numTris_(0), vertexSize_(0), rotationAxis_(0.0, 0.0, 1.0), rotationSpeed_(0.25), lightPos_(0.5f, 0.5f, -2.0f)
{
	rotationAngle_ = 0.0;
}

Renderable::~Renderable()
{
	if (texture_.isCreated()) {
		texture_.destroy();
	}
	if (vbo_.isCreated()) {
		vbo_.destroy();
	}
	if (ibo_.isCreated()) {
		ibo_.destroy();
	}
	if (vao_.isCreated()) {
		vao_.destroy();
	}
}

void Renderable::createShaders()
{
	QString vertexFilename = "vert.glsl";
	bool ok = shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexFilename);
	if (!ok) {
		qDebug() << shader_.log();
	}
	QString fragmentFilename = "frag.glsl";
	ok = shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentFilename);
	if (!ok) {
		qDebug() << shader_.log();
	}
	ok = shader_.link();
	if (!ok) {
		qDebug() << shader_.log();
	}
}

void Renderable::init(const QVector<QVector3D>& positions, const QVector<QVector3D>& normals, const QVector<QVector2D>& texCoords, const QVector<unsigned int>& indexes, const QString& textureFile)
{
	// NOTE:  We do not currently do anything with normals -- we just
	// have it here for a later implementation!
	// We need to make sure our sizes all work out ok.
	if (positions.size() != texCoords.size() ||
		positions.size() != normals.size()) {
		qDebug() << "[Renderable]::init() -- positions size mismatch with normals/texture coordinates";
		qDebug() << "[Renderable]::init() -- positions " << positions.size();
		qDebug() << "[Renderable]::init() -- normals " << normals.size();
		qDebug() << "[Renderable]::init() -- texCoords " << texCoords.size();
		return;
	}

	// Set our model matrix to identity
	modelMatrix_.setToIdentity();
	// Load our texture.
	if (textureFile != "") {
	    texture_.setData(QImage(textureFile).mirrored(true, true));
	}

	// set our number of trianges.
	numTris_ = indexes.size() / 3;

	// num verts (used to size our vbo)
	int numVerts = positions.size();
	vertexSize_ = 3 + 2;  // Position + texCoord
	int numVBOEntries = numVerts * vertexSize_;

	// Setup our shader.
	createShaders();

	// Now we can set up our buffers.
	// The VBO is created -- now we must create our VAO
	vao_.create();
	vao_.bind();
	vbo_.create();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.bind();
	// Create a temporary data array
	float* data = new float[numVBOEntries];
	for (int i = 0; i < numVerts; ++i) {
		data[i * vertexSize_ + 0] = positions.at(i).x();
		data[i * vertexSize_ + 1] = positions.at(i).y();
		data[i * vertexSize_ + 2] = positions.at(i).z();
		data[i * vertexSize_ + 3] = texCoords.at(i).x();
		data[i * vertexSize_ + 4] = texCoords.at(i).y();
	}
	vbo_.allocate(data, numVBOEntries * sizeof(float));
	delete[] data;

	// Create our index buffer
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// create a temporary array for our indexes
	unsigned int* idxAr = new unsigned int[indexes.size()];
	for (int i = 0; i < indexes.size(); ++i) {
		idxAr[i] = indexes.at(i);
	}
	ibo_.allocate(idxAr, indexes.size() * sizeof(unsigned int));
	delete[] idxAr;

	// Make sure we setup our shader inputs properly
	shader_.enableAttributeArray(0);
	shader_.setAttributeBuffer(0, GL_FLOAT, 0, 3, vertexSize_ * sizeof(float));
	shader_.enableAttributeArray(1);
	shader_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 2, vertexSize_ * sizeof(float));

	// Release our vao and THEN release our buffers.
	vao_.release();
	vbo_.release();
	ibo_.release();
}

void Renderable::init(const QVector<QVector3D>& positions, const QVector<QVector3D>& normals, const QVector<QVector2D>& texCoords, const QVector<unsigned int>& indexes, const QString& textureFile, const QString& normalFile)
{
	if (normalFile != "") {
	    normal_.setData(QImage(normalFile).mirrored(true, true));
	}
	init(positions, normals, texCoords, indexes, textureFile);
}

void Renderable::update(const qint64 msSinceLastFrame)
{
	// For this lab, we want our polygon to rotate. 
	float sec = msSinceLastFrame / 1000.0f;
	float anglePart = sec * rotationSpeed_ * 360.f;
	rotationAngle_ += anglePart;
	while(rotationAngle_ >= 360.0) {
		rotationAngle_ -= 360.0;
	}
    // This is where we want to maintain our light.
    float secs = (float)msSinceLastFrame / 1000.0f;
    float angle = secs * 180.0f;
    // Rotate our light around the scene
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(angle, 0.0, 1.0, 0.0);
    QVector3D newPos = rot * lightPos_;
    lightPos_ = newPos;
    // Because we aren't doing any occlusion, the lighting on the walls looks
    // super wonky.  Instead, just move the light on the z axis.
    newPos.setX(0.5);
    shader_.bind();
    shader_.setUniformValue("pointLights[0].color", 1.0f, 1.0f, 1.0f);
    shader_.setUniformValue("pointLights[0].position", newPos);

    shader_.setUniformValue("pointLights[0].ambientIntensity", 0.5f);
    shader_.setUniformValue("pointLights[0].specularStrength", 0.5f);
    shader_.setUniformValue("pointLights[0].constant", 1.0f);
    shader_.setUniformValue("pointLights[0].linear", 0.09f);
    shader_.setUniformValue("pointLights[0].quadratic", 0.032f);

    shader_.release();
}

void Renderable::draw(const QMatrix4x4& view, const QMatrix4x4& projection)
{
	// Create our model matrix.
	QMatrix4x4 rotMatrix;
	rotMatrix.setToIdentity();
    // uncomment below to have object rotate
	//rotMatrix.rotate(rotationAngle_, rotationAxis_);

	QMatrix4x4 modelMat = modelMatrix_ * rotMatrix;
	// Make sure our state is what we want
	shader_.bind();
	// Set our matrix uniforms!
	QMatrix4x4 id;
	id.setToIdentity();
	shader_.setUniformValue("modelMatrix", modelMat);
	shader_.setUniformValue("viewMatrix", view);
	shader_.setUniformValue("projectionMatrix", projection);

	vao_.bind();

    GLuint textureId = texture_.textureId();
    GLuint normalId = normal_.textureId();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalId);
    shader_.setUniformValue("tex", GL_TEXTURE0);
    shader_.setUniformValue("norm", GL_TEXTURE2 - GL_TEXTURE0);    

	//texture_.bind();
	glDrawElements(GL_TRIANGLES, numTris_*3, GL_UNSIGNED_INT, 0);
	//texture_.release();
	vao_.release();
	shader_.release();
}

void Renderable::setModelMatrix(const QMatrix4x4& transform)
{
	modelMatrix_ = transform;
}

void Renderable::setRotationAxis(const QVector3D& axis)
{
	rotationAxis_ = axis;
}

void Renderable::setRotationSpeed(float speed)
{
	rotationSpeed_ = speed;
}
