#ifndef RAY_H
#define RAY_H

#include "Vector.h"

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

private:
	Vector m_p;
	Vector m_d;
	Vector m_q;
	float m_t;
	bool m_intersect;
};

#endif // !RAY_H