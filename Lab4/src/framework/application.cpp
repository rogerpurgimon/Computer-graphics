#include "application.h"
#include "utils.h"
#include "includes.h"
#include "utils.h"

#include "image.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "material.h"
#include "light.h"

Camera* camera = NULL;
Mesh* mesh = NULL;
Shader* shader = NULL;

//might be useful...
Material* material = NULL;
Light* light = NULL;
Light* light2 = NULL;
Shader* phong_shader = NULL;
Shader* gouraud_shader = NULL;

Vector3 ambient_light(0.1,0.2,0.3); //here we can store the global ambient light of the scene

float angle = 0;
Vector3 axis = Vector3(0, 1, 0);

enum Exercises { simple, gouraud, phong, task3, task4, task5, task6 };
Exercises exercise = simple;

Vector2 first_pos;
bool left_click = false;
Vector2 d;

bool m_away = false;
bool m_closer = false;

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
	if( !mesh->loadOBJ( "../res/meshes/lee.obj" ) )
		std::cout << "FILE Lee.obj NOT FOUND " << std::endl;

	//we load one or several shaders...
	shader = Shader::Get( "../res/shaders/simple.vs", "../res/shaders/simple.fs" );
	

	//load your Gouraud and Phong shaders here and stored them in some global variables
	gouraud_shader = Shader::Get("../res/shaders/gouraud.vs", "../res/shaders/gouraud.fs");
	phong_shader = Shader::Get("../res/shaders/phong.vs", "../res/shaders/phong.fs");

	//CODE HERE:
	//create a light (or several) and and some materials
	light = new Light();
	light2 = new Light();
	light2->position.set(0, 50, 0);
	light2->specular_color.set(0.6f, 0.0, 0.0);
	material = new Material();
	lights.push_back(*light);
	lights.push_back(*light2);

}

//render one frame
void Application::render(void)
{

	//update the aspect of the camera acording to the window size
	camera->aspect = window_width / window_height;
	camera->updateProjectionMatrix();
	//Get the viewprojection matrix from our camera
	Matrix44 viewprojection = camera->getViewProjectionMatrix();

	//set the clear color of the colorbuffer as the ambient light so it matches
	glClearColor(ambient_light.x, ambient_light.y, ambient_light.z, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear framebuffer and depth buffer 
	glEnable(GL_DEPTH_TEST); //enable depth testing for occlusions
	glDepthFunc(GL_LEQUAL); //Z will pass if the Z is LESS or EQUAL to the Z of the pixel


	if (exercise == simple) {
		//choose a shader and enable it
		shader->enable();

		Matrix44 model_matrix;
		model_matrix.setIdentity();
		model_matrix.translate(0, 0, 0); //example of translation
		model_matrix.rotate(angle, axis);
		shader->setMatrix44("model", model_matrix); //upload the transform matrix to the shader
		shader->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader

		//CODE HERE: pass all the info needed by the shader to do the computations
		//send the material and light uniforms to the shader
		//...

		//do the draw call into the GPU
		mesh->render(GL_TRIANGLES);

		//disable shader when we do not need it any more
		shader->disable();

		//swap between front buffer and back buffer
		SDL_GL_SwapWindow(this->window);
	}
	else if(exercise == gouraud) {
		//choose a shader and enable it
		gouraud_shader->enable();

		Matrix44 model_matrix;
		model_matrix.setIdentity();
		model_matrix.translate(0, 0, 0); //example of translation
		model_matrix.rotate(angle, axis);
		gouraud_shader->setMatrix44("model", model_matrix); //upload the transform matrix to the shader
		gouraud_shader->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader

		//CODE HERE: pass all the info needed by the shader to do the computations
		//send the material and light uniforms to the shader

		gouraud_shader->setUniform3("u_camera_pos", camera->eye);
		gouraud_shader->setUniform3("u_light_pos", light->position);
	
		gouraud_shader->setUniform3("u_Ka", material->ambient);
		gouraud_shader->setUniform3("u_Kd", material->diffuse);
		gouraud_shader->setUniform3("u_Ks", material->specular);
		gouraud_shader->setUniform1("u_alpha", material->shininess);

		gouraud_shader->setUniform3("u_Ia", ambient_light);
		gouraud_shader->setUniform3("u_Id", light->diffuse_color);
		gouraud_shader->setUniform3("u_Is", light->specular_color);

		//do the draw call into the GPU
		mesh->render(GL_TRIANGLES);

		//disable shader when we do not need it any more
		gouraud_shader->disable();

		//swap between front buffer and back buffer
		SDL_GL_SwapWindow(this->window);
	}
	else if(exercise == phong) {
		//choose a shader and enable it
		phong_shader->enable();

		Matrix44 model_matrix;
		model_matrix.setIdentity();
		model_matrix.translate(0, 0, 0); //example of translation
		model_matrix.rotate(angle, axis);
		phong_shader->setMatrix44("model", model_matrix); //upload the transform matrix to the shader
		phong_shader->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader

		//CODE HERE: pass all the info needed by the shader to do the computations
		//send the material and light uniforms to the shader

		phong_shader->setUniform3("u_camera_pos", camera->eye);
		phong_shader->setUniform3("u_light_pos", light->position);

		phong_shader->setUniform3("u_Ka", material->ambient);
		phong_shader->setUniform3("u_Kd", material->diffuse);
		phong_shader->setUniform3("u_Ks", material->specular);
		phong_shader->setUniform1("u_alpha", material->shininess);

		phong_shader->setUniform3("u_Ia", ambient_light);
		phong_shader->setUniform3("u_Id", light->diffuse_color);
		phong_shader->setUniform3("u_Is", light->specular_color);

		//do the draw call into the GPU
		mesh->render(GL_TRIANGLES);

		//disable shader when we do not need it any more
		phong_shader->disable();

		//swap between front buffer and back buffer
		SDL_GL_SwapWindow(this->window);
	}
	else if (exercise == task3) {
		for (int i = 0; i < lights.size(); i++) {
			//choose a shader and enable it
			phong_shader->enable();

			//glDepthFunc(GL_LEQUAL);

			Matrix44 model_matrix;
			model_matrix.setIdentity();
			model_matrix.translate(10, 0, 0); //example of translation
			model_matrix.rotate(angle, axis);
			phong_shader->setMatrix44("model", model_matrix); //upload the transform matrix to the shader
			phong_shader->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader

			//CODE HERE: pass all the info needed by the shader to do the computations
			//send the material and light uniforms to the shader

			phong_shader->setUniform3("u_camera_pos", camera->eye);
			phong_shader->setUniform3("u_light_pos", lights[i].position);

			phong_shader->setUniform3("u_Ka", material->ambient);
			phong_shader->setUniform3("u_Kd", material->diffuse);
			phong_shader->setUniform3("u_Ks", material->specular);
			phong_shader->setUniform1("u_alpha", material->shininess);

			phong_shader->setUniform3("u_Ia", ambient_light);
			phong_shader->setUniform3("u_Id", lights[i].diffuse_color);
			phong_shader->setUniform3("u_Is", light[i].specular_color);

			
			//do the draw call into the GPU
			
			//glDisable(GL_BLEND);
			mesh->render(GL_TRIANGLES);

			//disable shader when we do not need it any more
			phong_shader->disable();

			//swap between front buffer and back buffer
			SDL_GL_SwapWindow(this->window);
		}
	}
}

//called after render
void Application::update(double seconds_elapsed)
{
	if (keystate[SDL_SCANCODE_SPACE]) { angle += seconds_elapsed; }
	
	d = mouse_position - first_pos;
	int distance = sqrt(pow(d.x, 2) + pow(d.y, 2));
	if (left_click) {
		if (distance) {
			angle += seconds_elapsed * distance;
			if (abs(d.x) > abs(d.y) && d.x > 0) { axis.set(0, -1, 0); } //provisional, s'ha de canviar
			if (abs(d.y) > abs(d.x) && d.y > 0) { axis.set(-1, 0, 0); } //
			if (abs(d.x) > abs(d.y) && d.x < 0) { axis.set(0, 1, 0); } //
			if (abs(d.y) > abs(d.x) && d.y < 0) { axis.set(1, 0, 0); } //
			first_pos = mouse_position;
		}
	}

	if (keystate[SDL_SCANCODE_RIGHT])
		camera->eye = camera->eye + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
	else if (keystate[SDL_SCANCODE_LEFT])
		camera->eye = camera->eye + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
	if (keystate[SDL_SCANCODE_UP])
		camera->eye = camera->eye + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
	else if (keystate[SDL_SCANCODE_DOWN])
		camera->eye = camera->eye + Vector3(0, -1, 0) * seconds_elapsed * 10.0;

	if (keystate[SDL_SCANCODE_F]){ camera->fov += 10 * seconds_elapsed; }
	if (keystate[SDL_SCANCODE_G]) { camera->fov -= 10 * seconds_elapsed; }
}


//keyboard press event 
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); break; //ESC key, kill the app
		case SDLK_r: 
			Shader::ReloadAll();
			break; //ESC key, kill the app
		case SDLK_1: exercise = gouraud; std::cout << "\n\nTASK 1: Gouraud" << std::endl; break;
		case SDLK_2: exercise = phong; std::cout << "\n\nTASK 2: Phong" << std::endl; break;
		case SDLK_3: exercise = task3; std::cout << "\n\nTASK 3: Adding multiple lights using multiple passes Phong" << std::endl; break;
		//case SDLK_4: exercise = task4; break;
		//case SDLK_5: exercise = task5; break;
		//case SDLK_6: exercise = task6; break;

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
