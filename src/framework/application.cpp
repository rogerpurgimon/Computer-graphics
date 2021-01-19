#include "application.h"
#include "utils.h"
#include "image.h"

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
}

//render one frame
void Application::render( Image& framebuffer )
{
	
	//clear framebuffer if we want to start from scratch
	framebuffer.fill(Color::BLACK);

	//here you can add your code to fill the framebuffer
	Application::framebuffer.drawRectangle(200, 200, 100, 200, Color::RED, false);
	Application::framebuffer.drawRectangle(500, 500, 150, 20, Color::BLUE, true);
	Application::framebuffer.drawLine(400, 500, 600, 100, Color::GREEN);
	Application::framebuffer.drawCircle(400, 300, 50, Color::WHITE, false);
	Application::framebuffer.drawCircle(600, 100, 100, Color::YELLOW, true);


	//tasca 2(a)
	for (int x = 0; x < window_width; ++x)
	{
		for (int y = 0; y < window_height; ++y)
		{
			float red = 255 * x / (float)framebuffer.width; //red intensitat 255 quan x gran
			float blue = 255 * (1 - x) / (float)framebuffer.width; //blue intensitat 255 quan x petita (invers)
			framebuffer.setPixel(x, y, Color(red, 0, blue));
		}
	}


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
	//fill every pixel of the image with some random data
	for (unsigned int x = 0; x < framebuffer.width; x++)
	{
		for (unsigned int y = 0; y < framebuffer.height; y++)
		{
			framebuffer.setPixel(x, y, Color(randomValue() * 255, randomValue() * 255, randomValue() * 255)); //random color
		}
	}
	// Tasca 3(b)
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
	
	// Tasca 3(d)
	for (unsigned int x = 0; x < window_width; x++)
	{
		for (unsigned int y = 0; y < window_height; y++)
		{
			Color pixel = img.getPixel(x, y);
			if (pixel.r + pixel.g + pixel.b > 255*3/2)
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
	//Tasca 4
	for (int x = 0; x < window_width; x++)
	{
		for (int y = 0; y < window_height; y++)
		{
			float theta = PI / 4.;
			int rot_x =(int) (x+100) *cos(theta) - (y-400) *sin(theta);
			int rot_y =(int) (x+100) *sin(theta) + (y-400) *cos(theta);
			framebuffer.setPixel(x, y, img.getPixelSafe(rot_x, rot_y));
			
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
	switch(event.keysym.scancode)
	{
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
