#include "application.h"
#include "utils.h"
#include "image.h"


//application state

Color pixel;

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
char ex4Image = 'a';
char ex5Image = 'a';

bool first_timeEx2 = true;
bool first_timeEx3 = true;
bool first_timeEx4 = true;
bool first_timeEx5 = true;
bool ex5init = true;

double t = 0;
double randY[500], randX[500];

short unsigned int ex6Color = BLACK;
Color ex6Color_ = Color::BLACK;

bool draw = false;

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

	imgFruits.loadTGA("../res/fruits.tga");
	imgFruits.scale(window_width, window_height);

	imgEx6_guardar.loadTGA("../res/toolbar.tga");
	//	imgEx6_guardar.scale(50, 50);

	imgEx6_novaPag.loadTGA("../res/toolbar.tga");
	//imgEx6_novaPag.scale(50, 50);



	printf("Window_width: %f   Window_height: %f\n", window_width, window_height);

	//init tasca 5a
	for (int i = 0; i < 500; i++) {
		pos p = { randomValue() * window_width, (randomValue() * window_height) };
		positions.push_back(p);
		initPositions.push_back(p);
		rand_radi.push_back(randomValue() * 5);
	}
	//init tasca 5b
	for (int i = 0; i < 200; i++) {
		pos p_particle = { 3 * window_width / 8 + randomValue() * window_width / 4, 3 * window_height / 8 + randomValue() * window_height / 4 };
		particles.push_back(p_particle);
		initParticles.push_back(p_particle);

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
					pixel = imgFruits.getPixel(x, y);
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
					pixel = imgFruits.getPixel(x, y);
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
		//Tasca 4(a)
		if (ex4Image == 'a') {
			for (int x = 0; x < window_width; x++)
			{
				for (int y = 0; y < window_height; y++)
				{
					float theta = PI / 4.;  //float theta = rotEx4 * PI / 4.;
					int rot_x = (int)(x + 100) * cos(theta) - (y - 400) * sin(theta);
					int rot_y = (int)(x + 100) * sin(theta) + (y - 400) * cos(theta);
					framebuffer.setPixel(x, y, imgFruits.getPixelSafe(rot_x, rot_y));
				}
			}
			
		}
		else 
		//Tasca 4(b)
		{
			for (int x = 0; x < window_width; x++)
			{
				for (int y = 0; y < window_height; y++)
				{
					framebuffer.setPixel(x,y,imgFruits.getPixelSafe(x*0.5,y*0.5));
				}
			}
		}

		break;
	case ex5:
		//Tasca 5a
		if (ex5Image == 'a') {
			
			for (int i = 0; i < positions.size(); i++)
			{
				if ((positions[i].x > 10) && (positions[i].x < window_width-10)) {
					Application::framebuffer.drawCircle(positions[i].x, positions[i].y, rand_radi[i], Color(200, 200, 200), true);
				}
			}
		}
		//Tasca 5b
		else {
			for (int i = 0; i < particles.size(); i++) {
				
				int lenx = particles[i].x - window_width / 2;
				int leny = particles[i].y - window_height / 2;
				if (abs(lenx) < 225 && abs(leny) < 150) {
					Color color = Color::PURPLE;
					Application::framebuffer.drawLine(particles[i].x, particles[i].y, particles[i].x + lenx / 6, particles[i].y + leny / 6, color);
				}
				else if (abs(lenx) < 250 && abs(leny) < 175) {
					Color color = Color::CYAN;
					Application::framebuffer.drawLine(particles[i].x, particles[i].y, particles[i].x + lenx / 6, particles[i].y + leny / 6, color);
				}
				else {
					Color color = Color::WHITE;
					Application::framebuffer.drawLine(particles[i].x, particles[i].y, particles[i].x + lenx / 6, particles[i].y + leny / 6, color);
				}				
			}

		}
		
		break;
	case ex6:
		framebuffer.fill(Color::GRAY);

		Application::framebuffer.drawRectangle(window_width / 2, window_height / 2 - 30, window_width - 20, window_height - 80, Color::WHITE, true);

		switch (ex6Color) {
		case BLACK:
			Application::framebuffer.drawRectangle(3 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		case RED:
			Application::framebuffer.drawRectangle(4 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		case GREEN:
			Application::framebuffer.drawRectangle(5 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		case BLUE:
			Application::framebuffer.drawRectangle(6 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		case YELLOW:
			Application::framebuffer.drawRectangle(7 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		case PURPLE:
			Application::framebuffer.drawRectangle(8 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		case CYAN:
			Application::framebuffer.drawRectangle(9 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		case WHITE:
			Application::framebuffer.drawRectangle(10 * window_width / 13, window_height - 35, window_width / 25 + window_width / 80, window_width / 25 + window_width / 80, Color::WHITE, true);
			break;
		}

		
		Application::framebuffer.drawRectangle(3 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::BLACK, true);
		Application::framebuffer.drawRectangle(4 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::RED, true);
		Application::framebuffer.drawRectangle(5 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::GREEN, true);
		Application::framebuffer.drawRectangle(6 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::BLUE, true);
		Application::framebuffer.drawRectangle(7 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::YELLOW, true);
		Application::framebuffer.drawRectangle(8 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::PURPLE, true);
		Application::framebuffer.drawRectangle(9 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::CYAN, true);
		Application::framebuffer.drawRectangle(10 * window_width / 13, window_height - 35, window_width / 25, window_width / 25, Color::WHITE, true);

		for (unsigned int x = 11; x < 35; x++)
		{
			for (unsigned int y = 9; y < 42; y++)
			{
				pixel = imgEx6_novaPag.getPixel(x, y);
				/*pixel.b = pixel.b;
				pixel.r = pixel.r;
				pixel.g = pixel.g;*/
				framebuffer.setPixel(x + window_width / 13 - 20, y + window_height - 60, pixel);
			}
		}
		for (unsigned int x = 60; x < 92; x++)
		{
			for (unsigned int y = 9; y < 41; y++)
			{
				pixel = imgEx6_guardar.getPixel(x, y);
				/*pixel.b = pixel.b;
				pixel.r = pixel.r;
				pixel.g = pixel.g;*/
				framebuffer.setPixel(x + window_width / 13 - 20, y + window_height - 60, pixel);
			}
		}

		//draw
		for (int i = 0; i < tr.size(); i++)
		{
			framebuffer.drawCircle(tr[i].x, tr[i].y, 5, tr[i].c, true);
		}

		if (15 < mouse_position.y && mouse_position.y<window_height-75 && 15< mouse_position.x && mouse_position.x < window_width-15) {
			framebuffer.drawCircle(mouse_position.x, mouse_position.y, 5, ex6Color_, true);
			
			
		}
		
	}
}


//called after render
void Application::update(double seconds_elapsed)
{

	switch (exercise) {
	case ex5:
		if (ex5Image == 'a') {
			if (ex5init) {
				for (int i = 0; i < positions.size(); i++) {
					randX[i] = randomValue();
					randY[i] = randomValue();
				}
			}
			ex5init = false;

			t += seconds_elapsed;

			for (int i = 0; i < positions.size(); i++)
			{
				positions[i].y -= seconds_elapsed * (double)200 * randY[i];

				positions[i].x = initPositions[i].x + 120 * sin(t) * randX[i];

				if (positions[i].y < 10) {
					positions[i].y = window_height - 10;
				}
			}
		}
		else {
			for (int i = 0 ; i < particles.size(); i++)
			{
				if (particles[i].x > window_width / 2 && particles[i].y > window_height/2) {
					float dx = (particles[i].x - window_width / 2) / ((particles[i].x - window_width / 2) + (particles[i].y - window_height / 2));
					float dy = (particles[i].y - window_height / 2) / ((particles[i].x - window_width / 2) + (particles[i].y - window_height / 2));
					particles[i].y += seconds_elapsed * 400 * dy;
					particles[i].x += seconds_elapsed * 400 * dx;
				}
				else if (particles[i].x > window_width / 2 && particles[i].y <= window_height / 2) {
					float dx = (particles[i].x - window_width / 2) / ((particles[i].x - window_width / 2) + (window_height / 2 - particles[i].y));
					float dy = (window_height / 2 - particles[i].y) / ((particles[i].x - window_width / 2) + (window_height / 2 - particles[i].y));
					particles[i].y -= seconds_elapsed * 400 * dy;
					particles[i].x += seconds_elapsed * 400 * dx;
				}
				else if (particles[i].x <= window_width / 2 && particles[i].y > window_height / 2) {
					float dx = (window_width / 2 - particles[i].x) / ((window_width / 2 -particles[i].x) + (particles[i].y - window_height / 2));
					float dy = (particles[i].y - window_height / 2) / ((window_width / 2 - particles[i].x) + (particles[i].y - window_height / 2));
					particles[i].y += seconds_elapsed * 400 * dy;
					particles[i].x -= seconds_elapsed * 400 * dx;
				}
				else if (particles[i].x <= window_width / 2 && particles[i].y <= window_height / 2) {
					float dx = (window_width / 2 - particles[i].x) / ((window_width / 2 - particles[i].x) + (window_height / 2 - particles[i].y));
					float dy = (window_height / 2 - particles[i].y) / ((window_width / 2 - particles[i].x) + (window_height / 2 - particles[i].y));
					particles[i].y -= seconds_elapsed * 400 * dy;
					particles[i].x -= seconds_elapsed * 400 * dx;
				}
				if (particles[i].y < 50 || particles[i].y > window_height - 50)
				{
					particles[i].y = initParticles[i].y; particles[i].x = initParticles[i].x;
				}
				else if (particles[i].x < 75|| particles[i].x > window_width - 75)
				{
					particles[i].y = initParticles[i].y; particles[i].x = initParticles[i].x;
				}
			}
		}
	}

	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed
	{
		printf("seconds_elapsed: %f\n", seconds_elapsed);
		//...
	}

	//to read mouse position use mouse_position
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
		if (first_timeEx3) printf("\nDrawing proposed image filter INVERT (b)...\n(Click to change)\n");
		first_timeEx3 = false;
		break;
	case SDL_SCANCODE_4:
		exercise = ex4;
		std::cout << "\n\nTASK 4: Warping an image" << std::endl;
		if (first_timeEx4) printf("\nDrawing 45 degrees ROTATED image (a)...\n(Click to change)\n");
		first_timeEx4 = false;
		break;
	case SDL_SCANCODE_5:
		exercise = ex5;
		std::cout << "\n\nTASK 5: Create one particles animation" << std::endl;
		if (first_timeEx5) printf("\nDrawing SNOW animation (a)...\n(Click to change)\n");
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
				printf("\nDrawing proposed image filter THRESHOLD (d)...\n(Click to change)\n");
			}
			else {
				ex3Image = 'b';
				printf("\nDrawing proposed image filter INVERT (b)...\n(Click to change)\n");
			}
			break;
		case ex4:
			if (ex4Image == 'a') {
				ex4Image = 'b';
				printf("\nDrawing 10 times SCALED image (b)...\n(Click to change)\n");
			}
			else {
				ex4Image = 'a';
				printf("\nDrawing 45 degrees ROTATED image (a)...\n(Click to change)\n");
			}
			break;
		case ex5:
			if (ex5Image == 'a') {
				ex5Image = 'b';
				printf("\nDrawing STARFIELD animation (b)...\n(Click to change)\n");
			}
			else {
				ex5Image = 'a';
				printf("\nDrawing SNOW animation (a)...\n(Click to change)\n");
			}
			break;
		case ex6:
			//3 * window_width / 13, window_height - 35, window_width / 25, window_width / 25
			if ((mouse_position.x > 3 * window_width / 13 - window_width / 50) && (mouse_position.x < 3 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = BLACK;
				ex6Color_ = Color::BLACK;
				draw = true;
			}
			else if ((mouse_position.x > 4 * window_width / 13 - window_width / 50) && (mouse_position.x < 4 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = RED;
				ex6Color_ = Color::RED;

			}
			else if ((mouse_position.x > 5 * window_width / 13 - window_width / 50) && (mouse_position.x < 5 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = GREEN;
				ex6Color_ = Color::GREEN;

			}
			else if ((mouse_position.x > 6 * window_width / 13 - window_width / 50) && (mouse_position.x < 6 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = BLUE;
				ex6Color_ = Color::BLUE;

			}
			else if ((mouse_position.x > 7 * window_width / 13 - window_width / 50) && (mouse_position.x < 7 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = YELLOW;
				ex6Color_ = Color::YELLOW;

			}
			else if ((mouse_position.x > 8 * window_width / 13 - window_width / 50) && (mouse_position.x < 8 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = PURPLE;
				ex6Color_ = Color::PURPLE;

			}
			else if ((mouse_position.x > 9 * window_width / 13 - window_width / 50) && (mouse_position.x < 9 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = CYAN;
				ex6Color_ = Color::CYAN;

			}
			else if ((mouse_position.x > 10 * window_width / 13 - window_width / 50) && (mouse_position.x < 10 * window_width / 13 + window_width / 50)
				&& (mouse_position.y > window_height - 50) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				ex6Color = WHITE;
				ex6Color_ = Color::WHITE;
			}
			//NewPage
			else if ((mouse_position.x > 51) && (mouse_position.x < 77)
				&& (mouse_position.y > window_height - 51) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				printf("New Page...\n");

			}

			//Save
			else if ((mouse_position.x > 102) && (mouse_position.x < 132)
				&& (mouse_position.y > window_height - 51) && (mouse_position.y < window_height - 50 + window_width / 25)) {
				printf("Saving drawing as TGA image...\n");
				
			}

			//Draw
			else if ((mouse_position.x > 10 && mouse_position.x < window_width - 10)
				&& (mouse_position.y > 10 && mouse_position.y < window_height - 70)) {
				switch (ex6Color) {
					case BLACK:
						printf("Drawing with color BLACK\n");
						pen = { (int)mouse_position.x, (int)mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
					case RED:
						printf("Drawing with color RED\n");
						pen = { (int)mouse_position.x, (int)mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
					case GREEN:
						printf("Drawing with color GREEN\n");
						pen = { (int)mouse_position.x, (int)mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
					case BLUE:
						printf("Drawing with color BLUE\n");
						pen = { (int)mouse_position.x, (int)mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
					case YELLOW:
						printf("Drawing with color YELLOW\n");
						pen = { (int)mouse_position.x, (int)mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
					case PURPLE:
						printf("Drawing with color PURPLE\n");
						pen = { (int)mouse_position.x, (int)mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
					case CYAN:
						printf("Drawing with color CYAN\n");
						pen = { (int) mouse_position.x, (int) mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
					case WHITE:
						printf("Drawing with color WHITE\n");
						pen = { (int)mouse_position.x, (int)mouse_position.y, ex6Color_ };
						tr.push_back(pen);
						break;
				}

			}
				
			break;
		}
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
	}

	if (event.button == SDL_BUTTON_RIGHT) //left mouse pressed
	{
		if (exercise == ex1) {
			start_pos = mouse_position;
			printf("\nRectangle %d --> Start pos: ", numRectangles);
			printf("(%d, ", (int)start_pos.x);
			printf("%d)\n", (int)start_pos.y);
			rectangles[numRectangles] = start_pos;
			numRectangles++; 
		}
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
				break;
			case ex2:
				break;
			case ex3:
				break;
			case ex4:
				break;
			case ex5:
				break;
			case ex6:
				draw = false;
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
