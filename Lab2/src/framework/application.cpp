#include "application.h"
#include "utils.h"
#include "image.h"


//Aplication state

enum Exercises { home, ex1, ex2, ex3, ex4, ex5};
Exercises exercise = home;

Vector2 start1_pos;
Vector2 end1_pos;

Vector2 start2_pos;
Vector2 end2_pos;

Vector2 start3_pos;
Vector2 end3_pos;

Vector2 lines1[50][2];
int numLines1 = 0;

Vector2 lines2[50][2];
int numLines2 = 0;

Vector2 circles[100][2];
int numCircles = 0;
bool filledCircle = false;

Vector2 triangles[100][4];
int numTriangles = 0;
bool filledTriangle = false;
Vector2 point1;
Vector2 point2;
Vector2 point3;



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

	framebuffer.resize(w, h);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;

	//here add your init stuff
	std::vector<Image::sCelda> table;
	table.resize(this->window_height);
	for (int i = 0; i < table.size(); i++) {
		table[i].minx = 100000;
		table[i].maxx = -100000;
	}
}

//render one frame
void Application::render( Image& framebuffer )
{
	//clear framebuffer if we want to start from scratch
	switch (exercise)
	{
	case ex1:
		//tasca 1
		for (int i = 0; i < numLines1; i++) {
			Application::framebuffer.DDA(lines1[i][0].x, lines1[i][0].y, lines1[i][1].x, lines1[i][1].y, Color::GREEN);
		}
	case ex2:
		//tasca 2
		for (int i = 0; i < numLines2; i++) {
			Application::framebuffer.BresenhamLine(lines2[i][0].x, lines2[i][0].y, lines2[i][1].x, lines2[i][1].y, Color::PURPLE);
		}
	case ex3: 
		//tasca 3
		for (int i = 0; i < numCircles; i++) {
			if (circles[i][1].y) filledCircle = true; else filledCircle = false;
			Application::framebuffer.BresenhamCircle(circles[i][0].x, circles[i][0].y, circles[i][1].x, Color::WHITE, filledCircle);
		}
	case ex4:
		//tasca 4(a)
		for (int i = 0; i < numTriangles; i++) {
			if (triangles[i][3].x) filledTriangle = true; else filledTriangle = false;
			Application::framebuffer.drawTriangle(triangles[i][0].x, triangles[i][0].y, triangles[i][1].x, triangles[i][1].y, triangles[i][2].x, triangles[i][2].y, Color::BLUE, filledTriangle);
		}
	}
}

//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed
	{
		//...
	}

	//to read mouse position use mouse_position
}

//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode)
	{
	case SDL_SCANCODE_1:
		exercise = ex1;
		std::cout << "\n\nTASK 1: DDA Lines" << std::endl;
		break;
	case SDL_SCANCODE_2:
		exercise = ex2;
		std::cout << "\n\nTASK 2: Bresenham Lines" << std::endl;
		break;
	case SDL_SCANCODE_3:
		exercise = ex3;
		std::cout << "\n\nTASK 3: Circles" << std::endl;
		break;
	case SDL_SCANCODE_4:
		exercise = ex4;
		std::cout << "\n\nTASK 4: Triangles" << std::endl;
		break;
	case SDL_SCANCODE_ESCAPE:
		exit(0);
		break; //ESC key, kill the app
	}
}

//keyboard key up event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//...
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
		switch (exercise) {
		case ex1:
			start1_pos = mouse_position;
			printf("\nLine %d --> Start pos: ", numLines1);
			printf("(%d, ", (int)start1_pos.x);
			printf("%d)", (int)start1_pos.y);
			lines1[numLines1][0] = start1_pos;
			break;
		case ex2:
			start2_pos = mouse_position;
			printf("\nLine %d --> Start pos: ", numLines2);
			printf("(%d, ", (int)start2_pos.x);
			printf("%d)", (int)start2_pos.y);
			lines2[numLines2][0] = start2_pos;
			break;
		case ex3:
			start3_pos = mouse_position;
			printf("\nCircle %d --> Start pos: ", numCircles);
			printf("(%d, ", (int)start3_pos.x);
			printf("%d)", (int)start3_pos.y);
			circles[numCircles][0] = start3_pos;
			break;
		case ex4:
			if (triangles[numTriangles][0].x == NULL) {
				triangles[numTriangles][3].x = 0; //we use this to know if triangle is fileld
				point1 = mouse_position;
				printf("\nTriangle %d --> First point: ", numTriangles);
				printf("(%d, ", (int)point1.x);
				printf("%d) ", (int)point1.y);
				triangles[numTriangles][0] = point1;
			}
			else if (triangles[numTriangles][1].x == NULL && triangles[numTriangles][0].x != NULL) {
				point2 = mouse_position;
				printf("\nTriangle %d --> Second point: ", numTriangles);
				printf("(%d, ", (int)point2.x);
				printf("%d) ", (int)point2.y);
				triangles[numTriangles][1] = point2;
			}
			else {
				point3 = mouse_position;
				printf("\nTriangle %d --> Third point: ", numTriangles);
				printf("(%d, ", (int)point3.x);
				printf("%d) ", (int)point3.y);
				triangles[numTriangles][2] = point3;
				numTriangles++;
			}
			break;
			
		case ex5:
			break;
		}
	} 
	else if (event.button == SDL_BUTTON_RIGHT) //left mouse pressed
	{
		switch (exercise) {
		case ex3:
			start3_pos = mouse_position;
			printf("\nFilled circle %d --> Start pos: ", numCircles);
			printf("(%d, ", (int)start3_pos.x);
			printf("%d)", (int)start3_pos.y);
			circles[numCircles][0] = start3_pos;
			break;
		case ex4:
			if (triangles[numTriangles][0].x == NULL) {
				triangles[numTriangles][3].x = 1; //we use this to know if triangle is fileld
				point1 = mouse_position;
				printf("\nTriangle %d --> First point: ", numTriangles);
				printf("(%d, ", (int)point1.x);
				printf("%d) ", (int)point1.y);
				triangles[numTriangles][0] = point1;
			}
			else if (triangles[numTriangles][1].x == NULL && triangles[numTriangles][0].x != NULL) {
				point2 = mouse_position;
				printf("\nTriangle %d --> Second point: ", numTriangles);
				printf("(%d, ", (int)point2.x);
				printf("%d) ", (int)point2.y);
				triangles[numTriangles][1] = point2;
			}
			else {
				point3 = mouse_position;
				printf("\nTriangle %d --> Third point: ", numTriangles);
				printf("(%d, ", (int)point3.x);
				printf("%d) ", (int)point3.y);
				triangles[numTriangles][2] = point3;
				numTriangles++;
			}
		}
	
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{
		switch (exercise) {
		case ex1:
			end1_pos = mouse_position;
			printf(" EndPos: ");
			printf("(%d, ", (int)end1_pos.x);
			printf("%d)\n", (int)end1_pos.y);
			lines1[numLines1][1] = end1_pos;
			numLines1++;
			break;
		case ex2:
			end2_pos = mouse_position;
			printf(" EndPos: ");
			printf("(%d, ", (int)end2_pos.x);
			printf("%d)\n", (int)end2_pos.y);
			lines2[numLines2][1] = end2_pos;
			numLines2++;
			break;
		case ex3:
		{
			end3_pos = mouse_position;
			Vector2 v = end3_pos - start3_pos;
			Vector2 r_0;
			r_0.x = sqrt(pow(v.x, 2) + pow(v.y, 2)); r_0.y = 0; //we use r_0.y to indicate if the circle must be filled or not (0-> Circle not filled)
			circles[numCircles][1] = r_0;
			printf(" Radius: ");
			printf("%d\n", (int)r_0.x);
			numCircles++;
			break; 
		}
		case ex4:
			
			break;
		case ex5:
			break;
		}
	}
	else if (event.button == SDL_BUTTON_RIGHT) //left mouse pressed
	{
		switch (exercise) {
		case ex3:
			end3_pos = mouse_position;
			Vector2 v = end3_pos - start3_pos;
			Vector2 r_0;
			r_0.x = sqrt(pow(v.x, 2) + pow(v.y, 2)); r_0.y = 1; //we use r_0.y to indicate if the circle must be filled or not (1 -> Circle filled)
			circles[numCircles][1] = r_0;
			printf(" Radius: ");
			printf("%d\n", (int)r_0.x);
			numCircles++;
			break;
		}
	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	std::cout << "\nNavigate through exercises using the numbers keys (1, 2, 3, 4, 5 or 6). Each key takes you to the task with the same number. (Ex: Press 4 to go to TASK 4)\n" << std::endl;
	launchLoop(this);
}
