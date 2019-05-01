#include "HeightMap.h"

#define PI 3.14f

HeightMap::HeightMap(float* map, int width, int height) 
		: heightMap(map), width(width), height(height) {
	glCall(glGenTextures(1, &texture_id));
	glCall(glBindTexture(GL_TEXTURE_2D, texture_id));
	
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	
	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_FLOAT, map));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

HeightMap* HeightMap::GenerateRandomHeightMap(int width, int height, int seed) {
	float* map = GenerateMap(width, height, seed);
	HeightMap* h_map = new HeightMap(map, width, height);
	return h_map;
}

float* HeightMap::GenerateMap(int width, int height, int seed) {
	srand(seed);
	float* map1 = new float[width * height];
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			map1[i * width + j] = (float)(rand() % 10000) / 10000.0f;
		}
	}

	const auto get = [&](int x, int y) -> float { return map1[x * width + y]; };

	const auto get_smooth_noise = [&](int x, int y) -> float {
		float center = 0.0f, corners = 0.0, sides = 0.0f;
        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                float val = get((x + i + width) % width, (y + j + height) % height);
                if (i == 0 && j == 0) center = val;
                else if (i == 0 || j == 0) sides += val;
                else corners += val;
            }
        }
		return corners / 16.0f + sides / 8.0f + center / 4.0f;//map1[x * width + y] / 4.0f;
	};

	const auto interpolate = [](float a, float b, float blend) -> float {
		float theta = blend * PI;
		float f = (1.0f - std::cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	};

	const auto get_interpolated_noise = [&](float x, float y) -> float {
		int intX = (int)x;
		int intY = (int)y;
		float fracX = x - intX;
		float fracY = y - intY;

		float v1 = get_smooth_noise(intX, intY);
		float v2 = get_smooth_noise((intX + 1) % width, intY);
		float v3 = get_smooth_noise(intX, (intY + 1) % height);
		float v4 = get_smooth_noise((intX + 1) % width, (intY + 1) % height);

		float i1 = interpolate(v1, v2, fracX);
		float i2 = interpolate(v3, v4, fracX);

		return interpolate(i1, i2, fracY);
	};

	float* map = new float[width * height];
    float max_height = 0.0f;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			map[i * width + j] = get_interpolated_noise(i / 8.0f, j / 8.0f);
            if (map[i * width + j] > max_height) max_height = map[i * width + j];
		}
	}
    std::cout << "max_height " << max_height << std::endl;
	delete map1;
	return map;
}

float* HeightMap::GetHeightMap() {
	return this->heightMap;
}

float HeightMap::BarycentricInterpolation(std::pair<float, float> v1, std::pair<float, float> v2, std::pair<float, float> v3, std::pair<float, float> p, std::tuple<float, float, float> vals) {
	float& x1 = v1.first, y1 = v1.second;
	float& x2 = v2.first, y2 = v2.second;
	float& x3 = v3.first, y3 = v3.second;
	float& px = p.first, py = p.second;

	float d = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
	float w1 = ((y2 - y3) * (px - x3) + (x3 - x2) * (py - y3)) / d;
	float w2 = ((y3 - y1) * (px - x3) + (x1 - x3) * (py - y3)) / d;
	float w3 = 1.0f - w1 - w2;
	return w1 * std::get<0>(vals) + w2 * std::get<1>(vals) + w3 * std::get<2>(vals);
}

float HeightMap::GetHeight(float x, float z) {
	float map_x = z;
	float map_z = x;
	int intX = (int)map_x;
	int intZ = (int)map_z;
	float fracX = map_x - intX;
	float fracZ = map_z - intZ;

	std::pair<float, float> p1(0.0f, 0.0f);
	std::pair<float, float> p2(1.0f, 0.0f);
	std::pair<float, float> p3(0.0f, 1.0f);
	std::pair<float, float> p4(1.0f, 1.0f);
	std::pair<float, float> p(fracX, fracZ);
	float h1 = heightMap[intX * width + intZ];
	float h2 = heightMap[(intX + 1) % width * width + intZ];
	float h3 = heightMap[intX * width + (intZ + 1) % height];
	float h4 = heightMap[(intX + 1) % width * width + (intZ + 1) % height];
	float result;
	if (fracZ < fracX) result = BarycentricInterpolation(p1, p2, p4, p, std::make_tuple(h1, h2, h4));
	else result = BarycentricInterpolation(p1, p3, p4, p, std::make_tuple(h1, h3, h4));
	return result - 0.5f;
}

void HeightMap::Bind(unsigned int slot) {
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, texture_id));
}

void HeightMap::Unbind() {
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

HeightMap::~HeightMap() {
	glCall(glDeleteTextures(1, &texture_id));
	if (heightMap != nullptr) delete heightMap;
}