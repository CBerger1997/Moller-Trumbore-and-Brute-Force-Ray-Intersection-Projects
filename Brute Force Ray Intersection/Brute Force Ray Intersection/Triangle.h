#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector.h"

//Class used to store the public and private variables and functions of the triangle class
class Triangle
{
public:

	Triangle();
	~Triangle();

	Vector n() { return m_n; };
	void n(Vector& n) { m_n = n; };
	float k() { return m_k; };
	void k(float k) { m_k = k; };

	Vector a() const { return m_a; };
	void a(Vector& a) { m_a = a; };
	Vector b() const { return m_b; };
	void b(Vector& b) { m_b = b; };
	Vector c() const { return m_c; };
	void c(Vector& c) { m_c = c; };
	
private:
	Vector m_a;
	Vector m_b;
	Vector m_c;

	Vector m_n;
	float m_k;
};

#endif // !TRIANGLE_H