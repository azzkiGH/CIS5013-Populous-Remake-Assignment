#include "PrincipleAxes.h"


using namespace std;
using namespace glm;



static float positionArray[] = {

	
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.025f, 0.025f, 0.0, 1.0f,
	1.075f, 0.075f, 0.0, 1.0f,
	1.025f, 0.075f, 0.0, 1.0f,
	1.075f, 0.025f, 0.0, 1.0f,


	0.0, 0.0, 0.0, 1.0f,
	0.0, 1.0, 0.0, 1.0f,
	-0.075f, 1.075f, 0.0, 1.0f,
	-0.05f, 1.05f, 0.0, 1.0f,
	-0.025f, 1.075f, 0.0, 1.0f,
	-0.075f, 1.025f, 0.0, 1.0f,


	0.0, 0.0, 0.0, 1.0f,
	0.0, 0.0, 1.0, 1.0f,
	0.025f, 0.075f, 1.0, 1.0f,
	0.075f, 0.075f, 1.0, 1.0f,
	0.025f, 0.025f, 1.0, 1.0f,
	0.075f, 0.025f, 1.0, 1.0f
};

static float colourArray[] = {

	
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f,


	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f,
	0.28f, 0.5f, 0.9f, 1.0f
};



static unsigned int indexArray[] = {

	0, 1, 2, 3, 4, 5,					
	6, 7, 8, 9, 10, 11,					
	12, 13, 14, 15, 15, 16, 16, 17		
};



CGPrincipleAxes::CGPrincipleAxes() {

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionArray), positionArray, GL_STATIC_DRAW);

	
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourArray), colourArray, GL_STATIC_DRAW);

	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);
}


CGPrincipleAxes::~CGPrincipleAxes() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colourBuffer);
	glDeleteBuffers(1, &indexBuffer);
}


void CGPrincipleAxes::render(bool showZAxis) {

	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(4, GL_FLOAT, 0, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glColorPointer(4, GL_FLOAT, 0, (GLvoid*)0);

	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	int numElements = (showZAxis) ? 20 : 12;
	glDrawElements(GL_LINES, numElements, GL_UNSIGNED_INT, (const GLvoid*)0);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
