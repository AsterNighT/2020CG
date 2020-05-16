#include "Item.h"

void Item::draw(){
	vertexArray->bind();
	shader->configurate();
	material->configurate();
	indexBuffer->bind();
	glDrawElements(
		GL_TRIANGLES,      // mode
		material->getVertexCount(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);
}
