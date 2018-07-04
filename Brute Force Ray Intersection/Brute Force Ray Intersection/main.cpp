//  Scene.cpp
//  Holds all function code for the core of the program

#include <SDL.h>

#include "Vector.h"
#include "Ray.h"
#include "GameObject.h"
#include "Triangle.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//Function Identifiers

bool InitialiseSDL();

SDL_Window* InitialiseWindow(SDL_Window*);

void Update(bool, SDL_Window*, SDL_Renderer*);

void Renderer(SDL_Window*);

bool IntersectionPoint(Triangle&, Ray&);

Vector FindNormalVector(Vector&, Vector&, Vector&);
float FindConstant(Vector&, Vector&);
float FindIntersectionParameter(float, Vector&, Vector&, Vector&);
Vector FindIntersectionPoint(Vector&, float, Vector&);

SDL_Renderer* BruteForceCalculation(SDL_Renderer*, int&, float*, GameObject);

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

	window = SDL_CreateWindow("Brute Force Ray Intersection", windowXPosition, windowYPosition, windowWidth, windowHeight,
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
			//calls the brute-force ray intersection function 
			//and sets the renderer equal to value receieved from function
			renderer = BruteForceCalculation(renderer, counter, times, object);

			//increments counter variable
			counter++;
		}


	}
}

//function used to calculate all of brute-froce ray intersection
SDL_Renderer* BruteForceCalculation(SDL_Renderer* renderer, int& counter, float times[], GameObject object)
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

			//sets the normal of the triangle through the find normal vector function
			tri.n(FindNormalVector(tri.a(), tri.b(), tri.c()));

			//sets the constant of the triangle through the find constant function
			tri.k(FindConstant(tri.n(), tri.a()));

			//if statement, stopping any rays that are parallel ot the plane
			if (tri.k() != 0)
			{

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

	//returns the rendere
	return renderer;
}

//function used to calculate the intersection point of the ray and triangle
bool IntersectionPoint(Triangle& tri, Ray& ray)
{
	//sets the intersectiopn paramter of the ray through the
	//find intersection paramter function
	ray.t(FindIntersectionParameter(tri.k(), tri.n(), ray.p(), ray.d()));

	//checks if the variable t is greater than or equal to 0
	if (ray.t() >= 0)
	{
		//sets the intersection of the ray and triangle through the
		//find intersection point function
		ray.q(FindIntersectionPoint(ray.p(), ray.t(), ray.d()));

		Vector edge;

		//calculates a triangle edge through subtraction of vertice a and b of the triangle
		edge.Subtraction(tri.b(), tri.a());

		Vector vp;

		//calculates the variable vp through subtraction of ray intersection 
		//and triangle vertice a
		vp.Subtraction(ray.q(), tri.a());

		Vector perp;

		//sets the perp variable equal to the cross product of the edge and vp variable
		perp.CrossProduct(edge, vp);

		//checks if the intersection point is to the right of triangle
		if (perp.DotProduct(tri.n(), perp) < 0)
		{
			return false;
		}

		//repeats the above step for each edge making sure the ray intersection point is
		//to the left of each edge
		edge.Subtraction(tri.c(), tri.b());

		vp.Subtraction(ray.q(), tri.b());

		perp.CrossProduct(edge, vp);

		if (perp.DotProduct(tri.n(), perp) < 0)
		{
			return false;
		}

		edge.Subtraction(tri.a(), tri.c());

		vp.Subtraction(ray.q(), tri.c());

		perp.CrossProduct(edge, vp);

		if (perp.DotProduct(tri.n(), perp) < 0)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

//Function used to find the normal between 3 vectors
Vector FindNormalVector(Vector& _a, Vector& _b, Vector& _c)
//24 operations
{
	//subtraction functions used for the calculation of vectors _p1 and _p2
	Vector _p1;
	_p1.Subtraction(_b, _a);
	Vector _p2;
	_p2.Subtraction(_c, _a);

	//cross product function used to calculate the normal vector before normalization
	Vector _n;
	_n.CrossProduct(_p1, _p2);

	//normalize funciton used to normalize the normal vector
	_n.Normalize();

	//returns the normalized vector
	return _n;
}

//function used to calculate the constant for the plane
float FindConstant(Vector& a, Vector& b)
//5 operations
{
	//dot product function used to calculate the constant value for the plane
	float _k = a.DotProduct(a, b);

	//returns the constant value
	return _k;
}

//Function used to calculate the intersection parameter between the ray and triangle
float FindIntersectionParameter(float k, Vector& n, Vector& p, Vector& d)
//17 operations
{
	//Variable decleration
	float _t;

	//checks if the ray and triangle are parallel
	if ((n.DotProduct(n, d) != 0))
	{
		//uses the dot product function along with an algorithm to calculate the parameter value at the intersection point
		_t = (k - n.DotProduct(n, p)) / (n.DotProduct(n, d));
	}
	else
	{
		_t = 0;
	}

	//returns the intersection parameter
	return _t;
}

//Function used to calculate the point of intersection between the ray and the triangle
Vector FindIntersectionPoint(Vector& _p, float _t, Vector& _d)
{
	Vector _d2;

	//calculates the directio vector after being multiplied by the intersection parameter
	_d2.SetPoint(_d.x() * _t, _d.y() * _t, _d.z() * _t);

	//uses the addition function to calculate the intersection point between the ray and the triangle
	Vector _q;
	_q.Addition(_p, _d2);

	//returns the point of intersection
	return _q;
}