#pragma once
#include "Renderer.h"
#include "Core/ShaderProgram.h"
#include "Core/VertexArray.h"

//class ShaderProgram;
//class VertexArray;

class MandelBox {
private:
	ShaderProgram* shader;
	VertexArray* vao;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::mat4 inv_proj_view;
    float near_plane;
    float far_plane;
public:
	MandelBox(ShaderProgram* shader);
	~MandelBox();

	void SetProjectionMatrix(glm::mat4 projection);
	void SetViewMatrix(glm::mat4 view);
	void SetNearFarPlanes(float near_plane, float far_plane);
    void SetBailout(float bailout);
    void SetIterations(int iterations);
    void SetMinRad2(float MinRad2);
    void SetScale(float scale);
    void SetRaymarchingParams(float min_distance, float max_distance, int max_steps);

	void Render();
};