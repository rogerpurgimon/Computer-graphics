#include "application.h"
#include "utils.h"
#include "image.h"
#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "light.h"
#include "material.h"

Camera* camera = NULL;
Mesh* mesh = NULL;
Matrix44 model_matrix;
Shader* shader = NULL;
Shader* phong1_shader = NULL;
Shader* phong2_shader = NULL;
Shader* phong3_shader = NULL;
Texture* texture = NULL;
Texture* normalmap_texture = NULL;
Light* light1 = NULL;
Light* light2 = NULL;
Light* light3 = NULL;
Material* material = NULL;

Vector3 ambient_light(60/255, 60/255, 60/255); //here we can store the global ambient light of the scene

float anglex = 0;
float angley = 0;

Vector2 first_pos;
bool left_click = false;
Vector2 d;
int num_light = 1;

enum Exercises { simple, task1, task2, task3, task4, task6 };
Exercises exercise = simple;


Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
	
	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0,20,20),Vector3(0,10,0),Vector3(0,1,0));
	camera->setPerspective(60,window_width / window_height,0.1,10000);

	//then we load a mesh
	mesh = new Mesh();
	mesh->loadOBJ("../res/meshes/lee.obj");

	//load the texture
	texture = new Texture();
	if(!texture->load("../res/textures/lee_color_specular.tga"))
	{
		std::cout << "Texture not found" << std::endl;
		exit(1);
	}
	normalmap_texture = new Texture();
	if (!normalmap_texture->load("../res/textures/lee_normal.tga"))
	{
		std::cout << "Texture not found" << std::endl;
		exit(1);
	}

	//we load a shader
	shader = Shader::Get("../res/shaders/texture.vs","../res/shaders/texture.fs");
	phong1_shader = Shader::Get("../res/shaders/phong_v.vs", "../res/shaders/phong_f1.fs");
	phong2_shader = Shader::Get("../res/shaders/phong_v.vs", "../res/shaders/phong_f2.fs");
	phong3_shader = Shader::Get("../res/shaders/phong_v.vs", "../res/shaders/phong_f3.fs");

	//load whatever you need here
	light1 = new Light();
	light1->position.set(20, 20, 10);
	light1->diffuse_color.set(1.0f, 1.0f, 1.0f);
	light1->specular_color.set(1.0f, 1.0f, 1.0f);
	light2 = new Light();
	material = new Material();
	light2->position.set(-5, 0, -5);
	light2->diffuse_color.set(0.2f, 0.25f, 0.5f);
	light2->specular_color.set(0.2f, 0.25f, 0.5f);
	light3 = new Light();
	lights.push_back(*light1);
	lights.push_back(*light2);
}

//render one frame
void Application::render(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable( GL_DEPTH_TEST );

	//Get the viewprojection
	camera->aspect = window_width / window_height;
	Matrix44 viewprojection = camera->getViewProjectionMatrix();
	if (exercise == simple) {
		//enable the shader
		shader->enable();

		model_matrix.setIdentity();
		model_matrix.translate(0, 0, 0);
		model_matrix.rotate(anglex, Vector3(0, 1, 0));
		model_matrix.rotate(angley, Vector3(1, 0, 0));
		shader->setMatrix44("model", model_matrix); //upload info to the shader
		shader->setMatrix44("viewprojection", viewprojection); //upload info to the shader

		shader->setTexture("color_texture", texture, 0); //set texture in slot 0

		//render the data
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();

		//swap between front buffer and back buffer
		SDL_GL_SwapWindow(this->window);
	}

	else if (exercise == task1) {
		//enable the shader
		phong1_shader->enable();

		for (int i = 0; i < lights.size(); i++) {
			model_matrix.setIdentity();
			model_matrix.translate(0, 0, 0);
			model_matrix.rotate(anglex, Vector3(0, 1, 0));
			model_matrix.rotate(angley, Vector3(1, 0, 0));
			phong1_shader->setMatrix44("model", model_matrix); //upload info to the shader
			phong1_shader->setMatrix44("viewprojection", viewprojection); //upload info to the shader

			phong1_shader->setTexture("u_texture", texture, 0); //set texture in slot 0
		
			phong1_shader->setUniform3("u_camera_pos", camera->eye);
			phong1_shader->setUniform3("u_light_pos", lights[i].position);

			phong1_shader->setUniform3("u_Ka", material->ambient);
			phong1_shader->setUniform3("u_Kd", material->diffuse);
			phong1_shader->setUniform3("u_Ks", material->specular);
			phong1_shader->setUniform1("u_alpha", material->shininess);

			phong1_shader->setUniform3("u_Ia", ambient_light);
			phong1_shader->setUniform3("u_Id", lights[i].diffuse_color);
			phong1_shader->setUniform3("u_Is", lights[i].specular_color);

			glDepthFunc(GL_LEQUAL);
			if (i == 0) { glDisable(GL_BLEND); mesh->render(GL_TRIANGLES); }
			else { glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); mesh->render(GL_TRIANGLES); glDisable(GL_BLEND); }
		}
		//disable shader
		phong1_shader->disable();

		//swap between front buffer and back buffer
		SDL_GL_SwapWindow(this->window);
	}
	else if (exercise == task2) {
		//enable the shader
		phong2_shader->enable();
		for (int i = 0; i < lights.size(); i++) {
			model_matrix.setIdentity();
			model_matrix.translate(0, 0, 0);
			model_matrix.rotate(anglex, Vector3(0, 1, 0));
			model_matrix.rotate(angley, Vector3(1, 0, 0));
			phong2_shader->setMatrix44("model", model_matrix); //upload info to the shader
			phong2_shader->setMatrix44("viewprojection", viewprojection); //upload info to the shader

			phong2_shader->setTexture("u_texture", texture, 0); //set texture in slot 0
		
			phong2_shader->setUniform3("u_camera_pos", camera->eye);
			phong2_shader->setUniform3("u_light_pos", lights[i].position);

			phong2_shader->setUniform3("u_Ka", material->ambient);
			phong2_shader->setUniform3("u_Kd", material->diffuse);
			phong2_shader->setUniform3("u_Ks", material->specular);
			phong2_shader->setUniform1("u_alpha", material->shininess);

			phong2_shader->setUniform3("u_Ia", ambient_light);
			phong2_shader->setUniform3("u_Id", lights[i].diffuse_color);
			phong2_shader->setUniform3("u_Is", lights[i].specular_color);

			glDepthFunc(GL_LEQUAL);
			if (i == 0) { glDisable(GL_BLEND); mesh->render(GL_TRIANGLES); }
			else { glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); mesh->render(GL_TRIANGLES); glDisable(GL_BLEND); }
		}
		//disable shader
		phong2_shader->disable();

		//swap between front buffer and back buffer
		SDL_GL_SwapWindow(this->window);
	}
	else if (exercise == task3) {
		//enable the shader
		phong3_shader->enable();
		for (int i = 0; i < lights.size(); i++) {
			model_matrix.setIdentity();
			model_matrix.translate(0, 0, 0);
			model_matrix.rotate(anglex, Vector3(0, 1, 0));
			model_matrix.rotate(angley, Vector3(1, 0, 0));
			phong3_shader->setMatrix44("model", model_matrix); //upload info to the shader
			phong3_shader->setMatrix44("viewprojection", viewprojection); //upload info to the shader

			phong3_shader->setTexture("u_texture", texture, 0); //set texture in slot 0
			phong3_shader->setTexture("u_normalmap", normalmap_texture, 1); //set texture in slot 1
		
			phong3_shader->setUniform3("u_camera_pos", camera->eye);
			phong3_shader->setUniform3("u_light_pos", lights[i].position);

			phong3_shader->setUniform3("u_Ka", material->ambient);
			phong3_shader->setUniform3("u_Kd", material->diffuse);
			phong3_shader->setUniform3("u_Ks", material->specular);
			phong3_shader->setUniform1("u_alpha", material->shininess);

			phong3_shader->setUniform3("u_Ia", ambient_light);
			phong3_shader->setUniform3("u_Id", lights[i].diffuse_color);
			phong3_shader->setUniform3("u_Is", lights[i].specular_color);

			glDepthFunc(GL_LEQUAL);
			if (i == 0) { glDisable(GL_BLEND); mesh->render(GL_TRIANGLES); }
			else { glEnable(GL_BLEND); glBlendFunc(GL_ONE, GL_ONE); mesh->render(GL_TRIANGLES); glDisable(GL_BLEND); }
		}
		//disable shader
		phong3_shader->disable();

		//swap between front buffer and back buffer
		SDL_GL_SwapWindow(this->window);
	}

}

//called after render
void Application::update(double seconds_elapsed)
{
	if (keystate[SDL_SCANCODE_SPACE]) { anglex += 5 * seconds_elapsed; }

	d = mouse_position - first_pos;
	if (left_click) {
		if (d.x != 0 && d.y != 0) {
			anglex += seconds_elapsed * -d.x;
			angley += seconds_elapsed * -d.y;
			first_pos = mouse_position;
		}
	}

	if (keystate[SDL_SCANCODE_RIGHT]) {
		if (exercise == 1 || exercise == 2) light1->position = light1->position + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
		else {
			switch (num_light) {
			case 1:
				lights[0].position = lights[0].position + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
				lights[1].position = lights[1].position - Vector3(1, 0, 0) * seconds_elapsed * 10.0;
			case 2:
				lights[1].position = lights[1].position + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
				lights[2].position = lights[2].position - Vector3(1, 0, 0) * seconds_elapsed * 10.0;
			case 3:
				lights[2].position = lights[2].position + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
			}
		}
	}
	else if (keystate[SDL_SCANCODE_LEFT]) {
		if (exercise == 1 || exercise == 2) light1->position = light1->position + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
		else {
			switch (num_light) {
			case 1:
				lights[0].position = lights[0].position + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
				lights[1].position = lights[1].position - Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
			case 2:
				lights[1].position = lights[1].position + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
				lights[2].position = lights[2].position - Vector3(-1, 0, 0) * seconds_elapsed * 10.0;

			case 3:
				lights[2].position = lights[2].position + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
			}
		}
	}
	if (keystate[SDL_SCANCODE_UP]) {
		if (exercise == 1 || exercise == 2) light1->position = light1->position + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
		else {
			switch (num_light) {
			case 1:
				lights[0].position = lights[0].position + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
				lights[1].position = lights[1].position - Vector3(0, 1, 0) * seconds_elapsed * 10.0;
			case 2:
				lights[1].position = lights[1].position + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
				lights[2].position = lights[2].position - Vector3(0, 1, 0) * seconds_elapsed * 10.0;
			case 3:
				lights[2].position = lights[2].position + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
			}
		}
	}
	else if (keystate[SDL_SCANCODE_DOWN]) {
		if (exercise == 1 || exercise == 2) light1->position = light1->position + Vector3(0, -1, 0) * seconds_elapsed * 10.0;
		else {
			switch (num_light) {
			case 1:
				lights[0].position = lights[0].position + Vector3(0, -1, 0) * seconds_elapsed * 10.0;
				lights[1].position = lights[1].position - Vector3(0, -1, 0) * seconds_elapsed * 10.0;
			case 2:
				lights[1].position = lights[1].position + Vector3(0, -1, 0) * seconds_elapsed * 10.0;
				lights[2].position = lights[2].position - Vector3(0, -1, 0) * seconds_elapsed * 10.0;
			case 3:
				lights[2].position = lights[2].position + Vector3(0, -1, 0) * seconds_elapsed * 10.0;
			}
		}
	}

	if (keystate[SDL_SCANCODE_F]) { camera->fov += 20 * seconds_elapsed; }
	if (keystate[SDL_SCANCODE_G]) { camera->fov -= 20 * seconds_elapsed; }

	if (keystate[SDL_SCANCODE_W]) { camera->center.y += 5 * seconds_elapsed; }
	if (keystate[SDL_SCANCODE_A]) { camera->center.x -= 5 * seconds_elapsed; }
	if (keystate[SDL_SCANCODE_S]) { camera->center.y -= 5 * seconds_elapsed; }
	if (keystate[SDL_SCANCODE_D]) { camera->center.x += 5 * seconds_elapsed; }
}

//keyboard press event 
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode){
		case SDL_SCANCODE_R: Shader::ReloadAll(); break;
		case SDL_SCANCODE_1: exercise = task1; std::cout << "\n\nTASK 1" << std::endl; break;
		case SDL_SCANCODE_2: exercise = task2; std::cout << "\n\nTASK 2" << std::endl; break;
		case SDL_SCANCODE_3: exercise = task3; std::cout << "\n\nTASK 3" << std::endl; break;
        case SDL_SCANCODE_ESCAPE: exit(0); break; //ESC key, kill the app
		case SDL_SCANCODE_X: {
			if (num_light == 1) { num_light = 2; }
			else if (num_light == 2) { num_light = 3; }
			else { num_light = 1; }
//			printf("\nLight1 Position =(%.2f, %.2f, %.2f)\nLight2 Position =(%.2f, %.2f, %.2f)\nLight3 Position =(%.2f, %.2f, %.2f)\n", lights[0].position.x, lights[0].position.y, lights[0].position.z,
//			lights[1].position.x, lights[1].position.y, lights[1].position.z, lights[2].position.x, lights[2].position.y, lights[2].position.z);
//			printf("num_light =%d\n", num_light);
		}
	}

}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		first_pos = mouse_position;
		left_click = true;
		printf("\nMoving camera position");
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{
		left_click = false;
	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
