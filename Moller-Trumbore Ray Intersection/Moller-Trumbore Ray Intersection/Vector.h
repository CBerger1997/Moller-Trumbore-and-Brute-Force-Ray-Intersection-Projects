#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <iostream>
#include <SDL.h>
#include <fstream>
#include <sstream>
#include <vector>
//Class used to store the public and private variables and funcitons of the vector class
class Vector
{
public:
	Vector();
	~Vector();

	Vector(float x, float y, float z);

	void SetPoint(float, float, float);
	void CrossProduct(Vector&, Vector&);
	float DotProduct(Vector&, Vector&);

	void Subtraction(const Vector &v1, const Vector &v2);
	void Addition(const Vector &v1, const Vector &v2);

	float x() { return m_x; };
	void x(float& x) { m_x = x; };
	float y() { return m_y; };
	void y(float& y) { m_y = y; };
	float z() { return m_z; };
	void z(float& z) { m_z = z; };

private:
	float m_x;
	float m_y;
	float m_z;
};

inline Vector::Vector(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

#endif // !VECTOR_H