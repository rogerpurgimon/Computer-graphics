#include "application.h"
#include "utils.h"
#include "image.h"
#include "mesh.h"


std::vector<Image::sCelda> table;

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

	zbuffer.resize(w, h);
	framebuffer.resize(w, h);
}

//Here we have already GL working, so we can create meshes and textures
//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
	
	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0,10,20),Vector3(0,10,0),Vector3(0,1,0)); //define eye,center,up
	camera->perspective(60, window_width / (float)window_height, 0.1, 10000); //define fov,aspect,near,far

	//load a mesh
	mesh = new Mesh();
	if( !mesh->loadOBJ("lee.obj") )
		std::cout << "FILE Lee.obj NOT FOUND" << std::endl;

	//load the texture
	texture = new Image();
	texture->loadTGA("color.tga");

	table.resize(this->window_height);
	for (int i = 0; i < table.size(); i++) {
		table[i].minx = 100000;
		table[i].maxx = -100000;
	}
}

//this function fills the triangle by computing the bounding box of the triangle in screen space and using the barycentric interpolation
//to check which pixels are inside the triangle. It is slow for big triangles, but faster for small triangles 
//void Application::fillTriangle(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector2& uv0, const Vector2& uv1, const Vector2& uv2)
//{
	//compute triangle bounding box in screen space

	//clamp to screen area

	//loop all pixels inside bounding box
	// for()

		//we must compute the barycentrinc interpolation coefficients, weights of pixel P(x,y)

		//check if pixel is inside or outside the triangle

		//here add your code to test occlusions based on the Z of the vertices and the pixel (TASK 5)

		//here add your code to compute the color of the pixel (barycentric interpolation) (TASK 4)

		//draw the pixels in the colorbuffer x,y 
		//framebuffer.setPixel(x, y, COMPUTED COLOR);
//}

//render one frame
void Application::render(Image& framebuffer)
{
	framebuffer.fill(Color(40, 45, 60)); //clear

	//for every point of the mesh (to draw triangles take three points each time and connect the points between them (1,2,3,   4,5,6,   ...)
	for (int i = 0; i < mesh->vertices.size()-2; ++i)
	{
		Vector3 vertex0 = mesh->vertices[i]; //extract vertex from mesh
		Vector3 vertex1 = mesh->vertices[i + 1];
		Vector3 vertex2 = mesh->vertices[i + 2];

		Vector2 texcoord = mesh->uvs[i]; //texture coordinate of the vertex (they are normalized, from 0,0 to 1,1)

		//project every point in the mesh to normalized coordinates using the viewprojection_matrix inside camera
		Vector3 normalized_point0 = camera->projectVector(vertex0);
		Vector3 normalized_point1 = camera->projectVector(vertex1);
		Vector3 normalized_point2 = camera->projectVector(vertex2);
		

		//convert from normalized (-1 to +1) to framebuffer coordinates (0,W)
		if (-1 < normalized_point0.x && normalized_point0.x < 1 && -1 < normalized_point0.y && normalized_point0.y < 1 && -1 < normalized_point0.z && normalized_point0.z < 1){
			if (-1 < normalized_point1.x && normalized_point1.x < 1 && -1 < normalized_point1.y && normalized_point1.y < 1 && -1 < normalized_point1.z && normalized_point1.z < 1) {
				if (-1 < normalized_point2.x && normalized_point2.x < 1 && -1 < normalized_point2.y && normalized_point2.y < 1 && -1 < normalized_point2.z && normalized_point2.z < 1) {
					Vector3 point0, point1, point2;
					point0.x = (1 + normalized_point0.x) * (window_width / 2);
					point0.y = (1 + normalized_point0.y) * (window_height / 2);
					point0.z = (1 + normalized_point0.y) * (window_width / 2);

					point1.x = (1 + normalized_point1.x) * (window_width / 2);
					point1.y = (1 + normalized_point1.y) * (window_height / 2);
					point1.z = (1 + normalized_point1.y) * (window_width / 2);

					point2.x = (1 + normalized_point2.x) * (window_width / 2);
					point2.y = (1 + normalized_point2.y) * (window_height / 2);
					point2.z = (1 + normalized_point2.y) * (window_width / 2);

					//paint points in framebuffer (using your drawTriangle function or the fillTriangle function)
					float d01 = sqrt(pow(point1.x - point0.x, 2) + pow(point1.y - point0.y, 2));
					float d12 = sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));

					if (d01 < 25 && d12 < 25) {//que no pinti triangles amb arestes llargues que distorsionen la figura.
						framebuffer.drawTriangle(point0.x, point0.y, point1.x, point1.y, point2.x, point2.y, Color::WHITE, true, table);
					}
				}
			}		
		}
	}
}

//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE])
	{
		//...
	}

	//example to move eye
	if (keystate[SDL_SCANCODE_LEFT])
		camera->eye.x -= 5 * seconds_elapsed;
	if (keystate[SDL_SCANCODE_RIGHT])
		camera->eye.x += 5 * seconds_elapsed;

	//if we modify the camera fields, then update matrices
	camera->updateViewMatrix();
	camera->updateProjectionMatrix();
}

//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode)
	{
		case SDL_SCANCODE_ESCAPE: exit(0); break; //ESC key, kill the app
	}
}

//keyboard released event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode)
	{
	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{

	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}
