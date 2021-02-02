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

Vector2 lines1[50][2];
int numLines1 = 0;

Vector2 lines2[50][2];
int numLines2 = 0;


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
}

//render one frame
void Application::render( Image& framebuffer )
{
	//clear framebuffer if we want to start from scratch
	framebuffer.fill(Color::BLACK);

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
			Application::framebuffer.Bresenham(lines2[i][0].x, lines2[i][0].y, lines2[i][1].x, lines2[i][1].y, Color::PURPLE);
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
			break;
		case ex4:
			break;
		case ex5:
			break;
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
			break;
		case ex4:
			break;
		case ex5:
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
