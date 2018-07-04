#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector.h"

class Triangle
{
public:

	Triangle();
	~Triangle();

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
};

#endif // !TRIANGLE_H