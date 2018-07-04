#include "GameObject.h"

GameObject::GameObject()
{
	m_filename = "plane.obj";

	LoadOBJVectors(m_filename);
}

GameObject::~GameObject()
{
}

bool GameObject::LoadOBJVectors(std::string filename)
{
	//retrieves the file from the specified file location
	std::ifstream inputFile(filename);

	//checks whether the file is open
	if (inputFile.is_open())
	{
		//creates vectors for the intial data
		std::vector<Vector> PositionData;

		//string variable containing the current line text
		std::string currentLine;

		//while loop used to loop through thr file
		while (std::getline(inputFile, currentLine))
		{
			//variable used to access individual parts of the string
			std::stringstream currentLineStream(currentLine);

			//if statements to loop through the string, finding the specific parts of the object 
			//e.g. the vertices, texture coordinates, vector normals and faces
			//and sets the data to it's corresponding intial vector variable

			if (!currentLine.substr(0, 2).compare(0, 1, "v"))
			{
				
				std::string ignoredData;

				float x;
				float y;
				float z;

				currentLineStream >> ignoredData >> x >> y >> z;

				Vector data(x, y, z);

				PositionData.push_back(data);

				float curx, cury, curz;

				curx = x;

				if (curx > m_highestx)
				{
					m_highestx = curx;
				}
				else if (curx < m_lowestx)
				{
					m_lowestx = curx;
				}

				cury = y;

				if (cury > m_highesty)
				{
					m_highesty = cury;
				}
				else if (cury < m_lowesty)
				{
					m_lowesty = cury;
				}

				curz = z;

				if (curz > m_highestz)
				{
					m_highestz = curz;
				}
				else if (curz < m_lowestz)
				{
					m_lowestz = curz;
				}

			}
			else if (!currentLine.substr(0, 2).compare(0, 1, "f"))
			{
				//string variables used to define what is useful vertex data and what is useless data
				std::string ignoredData;
				std::string verticeData[4];

				currentLineStream >> ignoredData >> verticeData[0] >> verticeData[1] >> verticeData[2] >> verticeData[3];

				//checks for triangulated faces
				if (verticeData[3].empty())
				{
					for (unsigned int i = 0; i < 3; i++)
					{
						//splits up the face data into position data, uv data and normal data
						std::stringstream currentSection(verticeData[i]);

						//position data variables
						unsigned int posID = 0;

						currentSection >> posID;

						//sets the intial data to the fixed data
						if (posID > 0)
						{
							m_fixedPosData.push_back(PositionData[posID - 1]);
						}

					}

				}
				//produces an error if the faces of the object haven't been triangulated
				else
				{
					std::cout << "Quad has been detected, the model needs to be triangulated" << std::endl;
					inputFile.close();
				}

			}

		}
		//closes the file
		inputFile.close();

		bool outsideBounds = false;

		//if statemetns, determining if the objects vertices are within the range of 0 and 255
		if (m_lowestx < 0)
		{
			m_lowestx = 0;
			outsideBounds = true;
		}
		if (m_highestx > 255)
		{
			m_highestx = 255;
			outsideBounds = true;
		}
		if (m_lowesty < 0)
		{
			m_lowesty = 0;
			outsideBounds = true;
		}
		if (m_highesty > 255)
		{
			m_highesty = 255;
			outsideBounds = true;
		}
		if (m_lowestz < 0)
		{
			m_lowestz = 0;
			outsideBounds = true;
		}
		if (m_highestz > 255)
		{
			m_highestz = 255;
			outsideBounds = true;
		}

		//outputs to the console if the objects vertices are within the range of 0 and 255
		if (outsideBounds)
		{
			std::cout << "The object is outside of the rays bounds.\n";
			std::cout << "Rendering will continue\n";
		}

		return true;
	}
	//produces an error if the file could not be opened
	else
	{

		std::cout << "The file was not found in directory: " << filename << std::endl;

		return false;
	}

	return true;
}

std::vector<Vector> GameObject::GetFixedPosData()
{
	return m_fixedPosData;
}
