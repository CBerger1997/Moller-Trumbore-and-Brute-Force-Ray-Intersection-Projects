#include "Vector.h"

//Constructor for the Vector Class

Vector::Vector()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

//Deconstructor for the Vector Class

Vector::~Vector()
{
}

//Function used to set the coordinates of a vector

void Vector::SetPoint(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

//Function used to normalize a vector

void Vector::Normalize()
//9 operations
{
	//calculates the length of a vector
	float length = sqrt((m_x*m_x) + (m_y * m_y) + (m_z * m_z));

	//normalizes a vector by dividing the coordinates by the length of the vector
	m_x = m_x / length;
	m_y = m_y / length;
	m_z = m_z / length;
}

//Function used to find the cross product of two vectors

void Vector::CrossProduct(Vector& a, Vector& b)
//9 operations
{
	this->m_x = (a.m_y * b.m_z) - (a.m_z * b.m_y);
	this->m_y = (a.m_z * b.m_x) - (a.m_x * b.m_z);
	this->m_z = (a.m_x * b.m_y) - (a.m_y * b.m_x);
}

//Function used to find the dot product of two vectors

float Vector::DotProduct(Vector& a, Vector& b)
//5 operations
{
	return float(a.m_x * b.m_x) + (a.m_y * b.m_y) + (a.m_z * b.m_z);
}

//Function used to subtract two vectors
void Vector::Subtraction(const Vector &v1, const Vector &v2)
//3 operations
{
	this->m_x = v1.m_x - v2.m_x;
	this->m_y = v1.m_y - v2.m_y;
	this->m_z = v1.m_z - v2.m_z;
}

//Function used to add two vectors
void Vector::Addition(const Vector &v1, const Vector &v2)
//3 operations
{
	this->m_x = v1.m_x + v2.m_x;
	this->m_y = v1.m_y + v2.m_y;
	this->m_z = v1.m_z + v2.m_z;
}