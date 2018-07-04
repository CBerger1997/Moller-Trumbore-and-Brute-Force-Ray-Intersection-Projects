//  Holds all function code for the core of the program

#include <SDL.h>

#include "Vector.h"
#include "Ray.h"
#include "GameObject.h"
#include "Triangle.h"

#include <iostream>
#include <string>

//Function Identifiers

bool InitialiseSDL();

SDL_Window* InitialiseWindow(SDL_Window*);

void Update(bool, SDL_Window*, SDL_Renderer*);

void Renderer(SDL_Window*);

bool IntersectionPoint(Triangle&, Ray&);

SDL_Renderer* MollerTrumboreCalculation(SDL_Renderer*, int&, float*, GameObject);

//main Function
int main(int argc, char *argv[])
{
	//creates the bool to loop through the program
	bool updateLoop = false;

	//creates the SDL variables for displaying the window and connecting SDL to OPenGL
	SDL_Window *window = 0;
	SDL_Renderer *renderer = 0;
	SDL_GLContext context = 0;

	//initialises SDL, allowing the program to run if initialised correctly
	updateLoop = InitialiseSDL();

	//initialies the window and renderer and connects SDL and OpenGl
	window = InitialiseWindow(window);
	renderer = SDL_CreateRenderer(window, -1, 0);

	GameObject object;

	updateLoop = object.LoadOBJVectors(object.filename());

	//Runs the Update function
	Update(updateLoop, window, renderer);

	std::cin.get();

	//deletes the SDL and OpenGl context, the window and then quits SDL
	//SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

//Initialisation functions
#pragma region Initialisation

//Function used to initialise SDL
bool InitialiseSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL did not intialise correctly or cannot initialise" << std::endl;
		return false;
	}

	return true;
}

//Initialises the SDL window
SDL_Window* InitialiseWindow(SDL_Window* window)
{
	int windowXPosition = SDL_WINDOWPOS_CENTERED;
	int windowYPosition = SDL_WINDOWPOS_CENTERED;
	int windowHeight = 255;
	int windowWidth = 255;

	window = SDL_CreateWindow("Moller-Trumbore Ray Intersection", windowXPosition, windowYPosition, windowWidth, windowHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	return window;
}

#pragma endregion

//Update function used to loop through the program
void Update(bool updateLoop, SDL_Window* window, SDL_Renderer* renderer)
{
	//variable delceration
	GameObject object;

	int counter = 1;

	float times[10];

	//clears the screen
	SDL_RenderClear(renderer);

	//sets the render colour
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	//presents the screen in black
	SDL_RenderPresent(renderer);

	//while loop used to loop through the program
	while (updateLoop)
	{

		//creates the event variable
		SDL_Event uEvent;

		//while loop to check for events
		while (SDL_PollEvent(&uEvent))
		{

			switch (uEvent.type)
			{
				//checks for quitting SDL
			case SDL_QUIT:
			{
				//stops the program loop variable
				updateLoop = false;
				break;
			}
			//checks for a key down
			default:
			{
				break;
			}
			}

		}
		//calls the moller-Trumbore ray intersection function 
		//and sets the renderer equal to value receieved from function
		renderer = MollerTrumboreCalculation(renderer, counter, times, object);

		//increments counter variable
		counter++;
	}
}

//function used to calculate all of Moller-Trumbore ray intersection
SDL_Renderer* MollerTrumboreCalculation(SDL_Renderer* renderer, int& counter, float times[], GameObject object)
{
	//variable decleration
	std::vector<Vector> fixedPosData;

	unsigned int beginTime, endTime;

	int intersectNo = 1;

	Ray ray;

	Triangle tri;

	std::ostringstream filename;

	//sets the fixedPosData equal to the vertices within the object loaded
	fixedPosData = object.GetFixedPosData();

	//sets the filename
	filename << "intersectFile" << counter << ".txt";

	//sets the file to be opened
	std::ofstream intersectFile(filename.str());

	//checks if the file opens
	if (intersectFile.is_open())
	{
		//outputs the object name to the file opened
		intersectFile << object.filename() << std::endl;

		//starts timing the function
		beginTime = SDL_GetTicks();

		//for loop used to iterate through the vertex position data of the object
		for (int v = 0; v < fixedPosData.size(); v += 3)
		{
			//sets the triangle vertices
			tri.a(fixedPosData.at(v));
			tri.b(fixedPosData.at(v + 1));
			tri.c(fixedPosData.at(v + 2));

			//2 for loops used to iterate through the coordinates of the rays
			for (float m = /*object.GetLowestY()*/ 0; m < /*object.GetHighestY()*/ 256; ++m)
			{
				for (float n = /*object.GetLowestZ()*/ 0; n < /*object.GetHighestZ()*/ 256; ++n)
				{
					//sets the ray origin
					ray.p(0, m, n);

					//checks whether the intersection function returned true
					if (IntersectionPoint(tri, ray))
					{
						//stores the intersection point within a text file
						intersectFile << intersectNo << ": " "x: " << ray.q().x() << ", y: " << ray.q().y() << ", z: " << ray.q().z() << std::endl;
						intersectNo++;

						//stores the intersection pixel position in the renderer
						SDL_RenderDrawPoint(renderer, ray.q().z(), ray.q().y());
					}
				}
			}
		}
		
		//ends the timing of the function
		endTime = SDL_GetTicks();

		//closes the file
		intersectFile.close();
	}
	else
	{
		//displays error message as file is unable to open
		std::cout << "Unable to open file: intersectFile" << counter << std::endl;
	}

	//sets delta time equal to time lapsed between starting and ending the intersection function
	float deltaTime = (float)(endTime - beginTime) / 1000.0f;

	//stores the time in the times array
	times[counter - 1] = deltaTime;

	//outputs the time taken for the intersection function to the console
	std::cout << "Test " << counter << ": ";
	std::cout << "Time taken: " << deltaTime << " seconds" << std::endl;

	//checks if 10 function iterations have occured
	if (counter == 10)
	{
		//sets timeSum to 0
		float timeSum = 0;

		//loops through values in the times array adding them up
		for (int i = 0; i < counter; i++)
		{
			timeSum += times[i];
		}

		//produces an average time over all 10 function iterations
		timeSum = timeSum / counter;

		//outputs the average time to the console
		std::cout << "Average time taken: " << timeSum << " seconds" << std::endl;

		//resets counter to 0
		counter = 0;
	}

	//presents the rendered object
	SDL_RenderPresent(renderer);

	//returns the renderer
	return renderer;
}

//function used to calculate the intersection point of the ray and triangle
bool IntersectionPoint(Triangle& tri, Ray& ray)
{
	Vector edge1;
	
	//calculates edge 1 of the triangle
	edge1.Subtraction(tri.b(), tri.a());

	Vector edge2;

	//calculates edge 2 of the triangle
	edge2.Subtraction(tri.c(), tri.a());

	Vector pvec;
	
	//calculates the pvec used for calculating the determinant and float 'u'
	pvec.CrossProduct(ray.d(), edge2);

	//used to check for back-face culling and other calculations
	float determinant = edge1.DotProduct(edge1, pvec);

	//back-face culling
	if (determinant <= 0)
	{
		return false;
	}

	//variable used for calculating float variables 'u', 'v' and 't'
	float invDeterminant = 1 / determinant;

	Vector tvec;

	//stores vector used for calculation of 'u'
	tvec.Subtraction(ray.p(), tri.a());

	//variable used to check if the ray point is outside the triangle or not
	float u = invDeterminant * (tvec.DotProduct(tvec, pvec));

	//check for whether the ray point is outside the triangle
	if (u < 0.0 || u > 1.0)
	{
		return false;
	}

	Vector qvec;
	
	//variable used for the calculation of 'v'
	qvec.CrossProduct(tvec, edge1);

	//variable used to check if the ray point is outside the triangle or not
	float v = invDeterminant * ray.p().DotProduct(ray.d(), qvec);

	//check for whether the ray point is outside the triangle
	if (v < 0.0 || u + v > 1.0)
	{
		return false;
	}

	//intersection parameter used to calculate the intersection point
	float t = invDeterminant * edge2.DotProduct(edge2, qvec);

	Vector q;

	Vector d2;

	//calculation to work out the vector from multiplying the ray direction vector
	//by the intersection parameter
	d2.SetPoint(d2.x() * t, d2.y() * t, d2.z() * t);

	//calculates the intersection point
	q.Addition(ray.p(), d2);

	ray.q(q);

	return true;

}