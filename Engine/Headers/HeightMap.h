#pragma once
#include "Core/Common.h"
#include "Core/VertexBuffer.h"
#include <tuple>

class VertexBuffer;

class HeightMap {
private:
	unsigned int texture_id;
	float* heightMap = nullptr;
	int width;
	int height;
	HeightMap(float* map, int width, int height);
	static float* GenerateMap(int width, int height, int seed);
	static float BarycentricInterpolation(std::pair<float, float> v1, std::pair<float, float> v2, std::pair<float, float> v3, std::pair<float, float> p, std::tuple<float, float, float> vals);
public:
	static HeightMap* GenerateRandomHeightMap(int width, int height, int seed);

	float* GetHeightMap();
	float GetHeight(float x, float z);
	void Bind(unsigned int slot = 0);
	void Unbind();

public:
	~HeightMap();
};