#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "Vector.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	bool LoadOBJVectors(std::string);

	std::vector<Vector> GetFixedPosData();

	float GetLowestX() { return m_lowestx; };
	void SetLowestX(float x) { m_lowestx = x; };

	float GetHighestX() { return m_highestx; };
	void SetHighestX(float x) { m_highestx = x; };

	float GetLowestY() { return m_lowesty; };
	void SetLowestY(float y) { m_lowesty = y; };

	float GetHighestY() { return m_highesty; };
	void SetHighestY(float y) { m_highesty = y; };

	float GetLowestZ() { return m_lowestz; };
	void SetLowestZ(float z) { m_lowestz = z; };

	float GetHighestZ() { return m_highestz; };
	void SetHighestZ(float z) { m_highestz = z; };

	std::string filename() { return m_filename; };

private:
	std::vector<Vector> m_fixedPosData;

	Vector triangle[3];

	float  m_highestx, m_lowestx, m_highesty,
		m_lowesty, m_highestz, m_lowestz;

	std::string m_filename;
};

#endif // !GAMEOBJECT_H