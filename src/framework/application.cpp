#include "application.h"
#include "utils.h"
#include "image.h"


//application state
Vector2 start_pos;
Vector2 end_pos;

Vector2 lines[50][2];
int numLines = 0;
Vector2 rectangles[50]; //Vector2 rectangles[50][2];
int numRectangles = 0;
Vector2 circles[50][2];
int numCircles = 0;

enum Exercises { home, ex1, ex2, ex3, ex4, ex5, ex6 };
Exercises exercise = home;

char ex2Image = 'a';
char ex3Image = 'b';
char ex4Image = 'c';
char ex5Image = 'r';

bool first_timeEx2 = true;
bool first_timeEx3 = true;
bool first_timeEx4 = true;
bool first_timeEx5 = true;

//double rotEx4 = 0.0;

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

	img.loadTGA("../res/fruits.tga");
	img.scale(window_width, window_height);
	//tasca 5a
	for (int i = 0; i < 100; i++) {
		pos p = { 50 + (randomValue() * (window_width-100)), 50 + (randomValue() * (window_height-100)) };
		particles.push_back(p);
		rand_radi.push_back(randomValue() * 10);
	}
	//Tasca 5b
	for (int i = 0; i < 100; i++) {
		pos p1 = { 100 + randomValue() * (window_width-200), 100 + randomValue() * (window_height-200) };
		particles1.push_back(p1);

	}
}

//render one frame
void Application::render(Image& framebuffer)
{

	//clear framebuffer if we want to start from scratch
	framebuffer.fill(Color::BLACK);

	switch (exercise) {
	case ex1:
		//tasca 1
		for (int i = 0; i < numLines; i++) {
			Application::framebuffer.drawLine(lines[i][0].x, lines[i][0].y, lines[i][1].x, lines[i][1].y, Color::GREEN);
		}
		for (int i = 0; i < numRectangles; i++) {
			Application::framebuffer.drawRectangle(rectangles[i].x, rectangles[i].y, 75, 100, Color::RED, false);
		}
		for (int i = 0; i < numCircles; i++) {
			Application::framebuffer.drawCircle(circles[i][0].x, circles[i][0].y, circles[i][1].x, Color::WHITE, false);
		}
		break;
	case ex2:
		//tasca 2(a)
		if (ex2Image == 'a') {
			for (int x = 0; x < window_width; ++x)
			{
				for (int y = 0; y < window_height; ++y)
				{
					float red = 255 * x / (float)framebuffer.width; //red intensitat 255 quan x gran
					float blue = 255 * (1 - x) / (float)framebuffer.width; //blue intensitat 255 quan x petita (invers)
					framebuffer.setPixel(x, y, Color(red, 0, blue));
				}
			}
		}
		else {
			//tasca 2(e)
			for (int x = 0; x < window_width; ++x)
			{
				for (int y = 0; y < window_height; ++y)
				{
					float y_w = y / (float)framebuffer.height; //així operem amb valors de 0 a 1
					float x_w = x / (float)framebuffer.width;
					if (y_w > 0.2 * sin(x_w * 6) + 0.5)
					{
						float green = 255 * (1 - y) / (float)framebuffer.height;
						framebuffer.setPixel(x, y, Color(0, green, 0));
					}
					else
					{
						float green = 255 * (y) / (float)framebuffer.height;
						framebuffer.setPixel(x, y, Color(0, green, 0));
					}
				}
			}
		}
		break;
	case ex3:
		// Tasca 3(b)
		if (ex3Image == 'b') {
			for (unsigned int x = 0; x < window_width; x++)
			{
				for (unsigned int y = 0; y < window_height; y++)
				{
					Color pixel = img.getPixel(x, y);
					pixel.b = 255 - pixel.b;
					pixel.r = 255 - pixel.r;
					pixel.g = 255 - pixel.g;
					framebuffer.setPixel(x, y, pixel);
				}
			}
		}
		else {
			// Tasca 3(d)
			for (unsigned int x = 0; x < window_width; x++)
			{
				for (unsigned int y = 0; y < window_height; y++)
				{
					Color pixel = img.getPixel(x, y);
					if (pixel.r + pixel.g + pixel.b > 255 * 3 / 2)
					{
						pixel.r = 255; pixel.b = 255; pixel.g = 255;
					}
					else
					{
						pixel.r = 0; pixel.b = 0; pixel.g = 0;
					}
					framebuffer.setPixel(x, y, pixel);
				}
			}
		}
		break;
	case ex4:
		//Tasca 4
		if (ex4Image == 'c') {
			for (int x = 0; x < window_width; x++)
			{
				for (int y = 0; y < window_height; y++)
				{
					float theta = PI / 4.;  //float theta = rotEx4 * PI / 4.;
					int rot_x = (int)(x + 100) * cos(theta) - (y - 400) * sin(theta);
					int rot_y = (int)(x + 100) * sin(theta) + (y - 400) * cos(theta);
					framebuffer.setPixel(x, y, img.getPixelSafe(rot_x, rot_y));
				}
			}
			
		}
		else {
			for (int x = 0; x < window_width; x++)
			{
				for (int y = 0; y < window_height; y++)
				{
					framebuffer.setPixel(x,y,img.getPixelSafe(x*0.5,y*0.5));
				}
			}
		}

		break;
	case ex5:
		if (ex5Image == 'r') {
			
			for (int i = 0; i < particles.size(); i++)
			{
				Application::framebuffer.drawCircle(particles[i].x, particles[i].y, rand_radi[i], Color(200,200,200), true);
			}
		}
		else {
			for (int i = 0; i < particles.size(); i++)
			{
				framebuffer.setPixel(particles1[i].x, particles1[i].y, Color::WHITE);
			}
		}
		
		break;
	case ex6:
		break;
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

	switch (exercise) {
	case ex5:
		if (ex5Image == 'r') 
		{
			for (int i = 0; i < particles.size(); i++)
			{
				particles[i].y -= seconds_elapsed * 100;

				if (particles[i].y < 10) {
					particles[i].y = window_height - 10;
				}
				if (particles[i].x < 10) {
					particles[i].x = window_width - 10;
				}
			}
		}
		else {
			for (int i = 0; i < particles.size(); i++)
			{
				if (particles1[i].x < (window_width / 2) && particles1[i].y < (window_height / 2)) {
					particles1[i].y -= seconds_elapsed * 100;
					particles1[i].x -= seconds_elapsed * 100;
				}
				if (particles1[i].x < (window_width / 2) && particles1[i].y >= (window_height / 2)) {
					particles1[i].y += seconds_elapsed * 100;
					particles1[i].x -= seconds_elapsed * 100;
				}
				if (particles1[i].x >= (window_width / 2) && particles1[i].y < (window_height / 2)) {
					particles1[i].y -= seconds_elapsed * 100;
					particles1[i].x += seconds_elapsed * 100;
				}

				if (particles1[i].x >= (window_width / 2) && particles1[i].y >= (window_height / 2)) {
					particles1[i].y += seconds_elapsed * 100;
					particles1[i].x += seconds_elapsed * 100;
				}

				if (particles1[i].y < 10) {
					particles1[i].y = (window_height/2) - randomValue()*300;
					particles1[i].x = (window_width / 2) - randomValue()*300;
				}
				if (particles1[i].y >= (window_height) - 10) {
					particles1[i].y = (window_height / 2) - randomValue()*300;
					particles1[i].x = (window_width / 2) + randomValue()*300;
				}
				if (particles1[i].x < 10) {
					particles1[i].x = (window_width/2) + randomValue()*300;
					particles1[i].y = (window_height / 2) - randomValue()*300;
				}
				if (particles1[i].x >= window_width - 10) {
					particles1[i].x = (window_width / 2) + randomValue()*300;
					particles1[i].y = (window_height / 2) + randomValue()*300;
				}
			}
		}
	}
}


//method automatically called when a key is pressed
void Application::onKeyDown(SDL_KeyboardEvent event)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode)
	{
	case SDL_SCANCODE_1:
		exercise = ex1;
		std::cout << "\n\nTASK 1: Drawing simple figures" << std::endl;
		break;
	case SDL_SCANCODE_2:
		exercise = ex2;
		std::cout << "\n\nTASK 2: Drawing 2 formulas from this collection" << std::endl;
		if(first_timeEx2) printf("\nDrawing proposed image (a)...\n(Click to change)\n");
		first_timeEx2 = false;
		break;
	case SDL_SCANCODE_3:
		exercise = ex3;
		std::cout << "\n\nTASK 3: Code 2 Image filters" << std::endl;
		if (first_timeEx3) printf("\nDrawing proposed image (b)...\n(Click to change)\n");
		first_timeEx3 = false;
		break;
	case SDL_SCANCODE_4:
		exercise = ex4;
		std::cout << "\n\nTASK 4: Warping an image" << std::endl;
		if (first_timeEx4) printf("\nDrawing proposed image (c)...\n(Click to change)\n");
		first_timeEx4 = false;
		break;
	case SDL_SCANCODE_5:
		exercise = ex5;
		std::cout << "\n\nTASK 5: Create one particles animation" << std::endl;
		if (first_timeEx5) printf("\nDrawing proposed image (r)...\n(Click to change)\n");
		first_timeEx5 = false;
		break;
	case SDL_SCANCODE_6:
		exercise = ex6;
		std::cout << "\n\nTASK 6: Make a drawing tool" << std::endl;
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
		switch (exercise) {
		case ex1:
			start_pos = mouse_position;
			printf("\nLine %d --> Start pos: ", numLines);
			printf("(%d, ", (int)start_pos.x);
			printf("%d)", (int)start_pos.y);
			lines[numLines][0] = start_pos;
			break;
		case ex2:
			if (ex2Image == 'a') {
				ex2Image = 'e';
				printf("\nDrawing proposed image (e)...\n(Click to change)\n");
			}
			else {
				ex2Image = 'a';
				printf("\nDrawing proposed image (a)...\n(Click to change)\n");
			}
			break;
		case ex3:
			if (ex3Image == 'b') {
				ex3Image = 'd';
				printf("\nDrawing proposed image (d)...\n(Click to change)\n");
			}
			else {
				ex3Image = 'b';
				printf("\nDrawing proposed image (b)...\n(Click to change)\n");
			}
			break;
		case ex4:
			if (ex4Image == 'c') {
				ex4Image = 'e';
				printf("\nDrawing proposed image (z)...\n(Click to change)\n");
			}
			else {
				ex4Image = 'c';
				printf("\nDrawing proposed image (c)...\n(Click to change)\n");
			}
			break;
		case ex5:
			if (ex5Image == 'r') {
				ex5Image = 's';
				printf("\nDrawing proposed image (s)...\n(Click to change)\n");
			}
			else {
				ex5Image = 'r';
				printf("\nDrawing proposed image (r)...\n(Click to change)\n");
			}
			break;
		case ex6:
			break;
		}
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
	}

	if (event.button == SDL_BUTTON_RIGHT) //left mouse pressed
	{
		if (exercise == ex1)
			start_pos = mouse_position;
			printf("\nRectangle %d --> Start pos: ", numRectangles);
			printf("(%d, ", (int)start_pos.x);
			printf("%d)\n", (int)start_pos.y);
			rectangles[numRectangles] = start_pos;
			numRectangles++; //
	}

	if (event.button == SDL_BUTTON_MIDDLE) //left mouse pressed
	{
		if (exercise == ex1)
			start_pos = mouse_position;
			printf("\nCircle %d --> Start pos: ", numCircles);
			printf("(%d, ", (int)start_pos.x);
			printf("%d)", (int)start_pos.y);
			circles[numCircles][0] = start_pos;
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{
		switch (exercise) {
			case ex1:
				end_pos = mouse_position;
				printf(" EndPos: ");
				printf("(%d, ", (int)end_pos.x);
				printf("%d)\n", (int)end_pos.y);
				lines[numLines][1] = end_pos;
				numLines++;
			case ex2:
				break;
			case ex3:
				break;
			case ex4:
				break;
			case ex5:
				break;
			case ex6:
				break;
		}
	}
	/*
	if (event.button == SDL_BUTTON_RIGHT) //right mouse unpressed
	{
		if (exercise == ex1)
			end_pos = mouse_position;
			Vector2 w_h = end_pos - start_pos;
			printf(" (Weight, Height): ");
			printf("(%d, ", (int)w_h.x);
			printf("%d)", (int)w_h.y);
			rectangles[numRectangles][1] = w_h;
			numRectangles++;
	}*/

	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse unpressed
	{
		if (exercise == ex1) {
			end_pos = mouse_position;
			Vector2 v = end_pos - start_pos;
			Vector2 r_0;
			r_0.x = sqrt(pow(v.x, 2) + pow(v.y, 2)); r_0.y = 0;
			circles[numCircles][1] = r_0;
			printf(" Radius: ");
			printf("%d\n", (int)r_0.x);
			numCircles++;
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
