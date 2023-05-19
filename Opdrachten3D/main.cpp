#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "ObjModel.h"
#include <glm/gtc/matrix_transform.hpp>

#include "FpsCam.h"

/* TODO:
*	Bal toevoegen
*	Bal logica (locatie, richting, snelheid (afnemend), botsing, etc)
*	Afmetigen tafel
*	Collision met tafel
*	Stok toevoegen
*	Stok logica (afschieten, richting laten bepalen door gebruiker, etc)
*	Belichting
*	Automatisch bewegend object
*	Speler object laten bedienen (zie `Stok logica`)
*/

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

ObjModel* biljartTalbe[3];
FpsCam* camera;
float rotation = 0;

void init();
void update();
void draw();

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}


void init()
{

	int value[10];
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});


	biljartTalbe[0] = new ObjModel("models/biljart/Biljart_table.obj");
	biljartTalbe[1] = new ObjModel("models/biljart/Biljart_edge.obj");
	biljartTalbe[2] = new ObjModel("models/biljart/Biljart_cloth.obj");
	camera = new FpsCam(window);
}

void update()
{
	camera->update(window);
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);

	glEnable(GL_DEPTH_TEST);

	for (auto& model : biljartTalbe) 
		model->draw();
}