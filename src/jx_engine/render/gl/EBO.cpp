#include"EBO.h"

// An EBO stores indices that define which vertices to use for each primitive (e.g., triangle, line) during rendering. 
// This allows you to reuse vertices, reducing the amount of memory required 
// and improving performance by enabling efficient use of the vertex data.

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(const GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

EBO::EBO(std::vector<GLuint> indicesVector) {

	GLuint* indices = indicesVector.data();
	GLsizeiptr size = sizeof(GLuint) * indicesVector.size();

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

// Binds the EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the EBO
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}