#ifndef RAY_H
#define RAY_H

#include "Vector.h"

//Class used to store the public and private variables and functions of the ray class

class Ray
{
public:
	Ray();
	~Ray();

	inline Vector p() const { return m_p; };
	inline void p(float x, float y, float z) { m_p.SetPoint(x, y, z); };
	inline Vector d() const { return m_d; };
	inline void d(float  x, float y, float z) { m_d.SetPoint(x, y, z); };
	inline Vector q() const { return m_q; };
	inline void q(Vector& q) { m_q = q; };
	float t() { return m_t; };
	void t(float t) { m_t = t; };

private:
	Vector m_p;
	Vector m_d;
	Vector m_q;
	float m_t;
};

#endif // !RAY_H