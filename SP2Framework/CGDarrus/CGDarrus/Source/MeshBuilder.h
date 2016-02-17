#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateCube(const std::string &meshName, Color color, Vector3 min, Vector3 max);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlices);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlices, float height);
	static Mesh* GenerateCone2(const std::string &meshName, Color color, unsigned numSlices, float height, float degree);
	static Mesh* GenerateRing(const std::string &meshName, Color color, float innerRadius, unsigned numSlices);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, bool hemi = false);
	static Mesh* GenerateHollowHemiSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float thickness);
	static Mesh* GenerateTorus(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float Radius, float Thickness);
	static Mesh* GenerateTorus2(const std::string &meshName, Color color, unsigned degree, unsigned numSlices, float Radius, float Thickness);
	static Mesh* GenerateCyclinder(const std::string &meshName, Color color, unsigned numSlices, float height, bool innerCyclinder = false, float innerRadius = 1, float thickness = 1);
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateFloor(const std::string &meshName, Color color, float lengthX, float lengthY);
};

#endif