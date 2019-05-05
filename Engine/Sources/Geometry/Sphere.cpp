#include "Geometry/Sphere.h"

Sphere::Sphere(ShaderProgram* shader, glm::vec3 position, float radius) {
    this->shader = shader;
    this->SetLightColor(glm::vec3(1.0f));
    this->SetLightPosition(glm::vec3(0.0f, 100.0f, 100.0f));
    this->SetSurfaceParameters(0.0f, 1.0f, 1.0f);
    this->position = position;
    this->radius = radius;

    glm::mat4 m = glm::identity<glm::mat4>();
    m = glm::translate(m, glm::vec3(position));
    m = glm::scale(m, glm::vec3(radius));
    this->SetModelMatrix(m);
    this->vao = new VertexArray();

    int vertices_offset = 0;
    int indexes_offset = 0;
    float* vertices = new float[3 * SPHERE_PHI_TILES_COUNT * (SPHERE_THETA_TILES_COUNT + 1)];
    float* normals = new float[3 * SPHERE_PHI_TILES_COUNT * (SPHERE_THETA_TILES_COUNT + 1)];
    unsigned int* indexes = new unsigned int[6 * SPHERE_PHI_TILES_COUNT * SPHERE_THETA_TILES_COUNT];
    
    float theta_step = glm::pi<float>() / SPHERE_THETA_TILES_COUNT;
    float phi_step = 2.0f * glm::pi<float>() / SPHERE_PHI_TILES_COUNT;
    for (int theta_index = 0; theta_index <= SPHERE_THETA_TILES_COUNT; ++theta_index) {
        for (int phi_index = 0; phi_index < SPHERE_PHI_TILES_COUNT; ++phi_index) {
            float theta = theta_index * theta_step;
            float phi = phi_index * phi_step;
            float normal_x = glm::sin(theta) * glm::cos(phi);
            float normal_y = glm::sin(theta) * glm::sin(phi);
            float normal_z = glm::cos(theta);

            normals[vertices_offset] = normal_x;
            normals[vertices_offset + 1] = normal_y;
            normals[vertices_offset + 2] = normal_z;
            vertices[vertices_offset] = normal_x;
            vertices[vertices_offset + 1] = normal_y;
            vertices[vertices_offset + 2] = normal_z;
            vertices_offset += 3;
            if (theta_index == SPHERE_THETA_TILES_COUNT) continue;

            int index1 = theta_index * SPHERE_PHI_TILES_COUNT + phi_index;
            int index2 = (theta_index + 1) * SPHERE_PHI_TILES_COUNT + phi_index;
            int index3 = (theta_index + 1) * SPHERE_PHI_TILES_COUNT + (phi_index + 1) % SPHERE_PHI_TILES_COUNT;
            int index4 = theta_index * SPHERE_PHI_TILES_COUNT + (phi_index + 1) % SPHERE_PHI_TILES_COUNT;
            indexes[indexes_offset] = index1;
            indexes[indexes_offset + 1] = index2;
            indexes[indexes_offset + 2] = index3;
            indexes[indexes_offset + 3] = index3;
            indexes[indexes_offset + 4] = index4;
            indexes[indexes_offset + 5] = index1;
            indexes_offset += 6;
        }
    }

    vertices_vbo = new VertexBuffer(vertices, vertices_offset * sizeof(float));
    BufferLayout vertices_layout;
    vertices_layout.AddElement<float>(3, vertices_location);
    vao->AddBuffer(*vertices_vbo, vertices_layout);
    delete [] vertices;

    normals_vbo = new VertexBuffer(normals, vertices_offset * sizeof(float));
    BufferLayout normals_layout;
    normals_layout.AddElement<float>(3, normals_location);
    vao->AddBuffer(*normals_vbo, normals_layout);
    delete [] normals;

    this->ibo = new IndexBuffer(indexes, indexes_offset);
    delete [] indexes;
}

Sphere::~Sphere() {
    delete ibo;
    delete vertices_vbo;
    delete normals_vbo;
    delete vao;
}

void Sphere::Render() {
	shader->Bind();
    vao->Bind();
    ibo->Bind();
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
	glCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0));
    //glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    vao->Unbind();
	shader->Unbind();
}

void Sphere::SetPosition(glm::vec3 position) {
    this->position = position;
    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 trans_m = glm::translate(identity, position);
    glm::mat4 scale_m = glm::scale(identity, glm::vec3(radius));
    this->SetModelMatrix(trans_m * scale_m);
}

void Sphere::SetRadius(float radius) {
    this->radius = radius;
    glm::mat4 identity = glm::identity<glm::mat4>();
    glm::mat4 trans_m = glm::translate(identity, position);
    glm::mat4 scale_m = glm::scale(identity, glm::vec3(radius));
    this->SetModelMatrix(trans_m * scale_m);
}

bool Sphere::ray_intersection(glm::vec3 origin, glm::vec3 ray, float& depth) {
    origin = origin - position;
    float xo = origin.x, yo = origin.y, zo = origin.z;
    float xr = ray.x, yr = ray.y, zr = ray.z;
    float a = xr * xr + yr * yr + zr * zr;
    float b = 2.0f * (xo * xr + yo * yr + zo * zr);
    float c = xo * xo + yo * yo + zo * zo - radius * radius;
    // solve equation a * d^2 + b * d + c = 0
    float delta = b * b - 4 * a * c;
    if (delta < 0) return false;
    float sqrt_delta = glm::sqrt(delta);
    float d1 = -(b + sqrt_delta) / a / 2.0f;
    float d2 = -(b - sqrt_delta) / a / 2.0f;
    float max = d1 > d2 ? d1 : d2;
    float min = d1 < d2 ? d1 : d2;
    if (max < 0 || min < 0) return false;
    depth = min;
    return true;
}

float Sphere::is_point_inside(glm::vec3 point) {
    return glm::length(point - position) <= radius;
}

void Sphere::SetModelMatrix(glm::mat4 matrix) {
    this->model_matrix = matrix;
    this->shader->FillUniformMat4f("u_model", matrix);
}