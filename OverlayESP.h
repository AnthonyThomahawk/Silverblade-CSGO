#pragma once
#include <string>
#include "BladeMemory.h"
#include "Globals.h"
#include "Offsets.h"
#include "dxoverlay.h"
#include <vector>
struct vec3_t {
	float x = 0, y = 0, z = 0;

	vec3_t();
	vec3_t(float _x, float _y, float _z);

	vec3_t operator+(const vec3_t& other);
	vec3_t operator+=(const vec3_t& other);

	vec3_t operator-(const vec3_t& other);
	vec3_t operator-=(const vec3_t& other);

	vec3_t operator*(float factor);
	vec3_t operator*(const vec3_t& other);
	vec3_t operator*=(float factor);
	vec3_t operator*=(const vec3_t& other);

	vec3_t operator/(float factor);
	vec3_t operator/(const vec3_t& other);
	vec3_t operator/=(float factor);
	vec3_t operator/=(const vec3_t& other);

	bool is_zero();

	float dot(const vec3_t& other);
	float dist(const vec3_t& other);
	float length();
	float length_sqr();
};

struct ent_info_t {
	std::uintptr_t entity_ptr = 0x0;
	__int32 serial_number = 0;
	ent_info_t* prev = nullptr;
	ent_info_t* next = nullptr;
};

class entity_t {
public:
	entity_t(std::uintptr_t ptr);

	bool dormant();

	int health();
	int index();
	int life_state();
	int team();

	vec3_t origin();

	void get_name(std::string& out);

	bool is_alive();

private:
	std::uintptr_t m_ptr = 0x0;
};

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

bool world_to_screen(const vec3_t& screen_size, const vec3_t& pos, vec3_t& out, view_matrix_t matrix);

struct esp_entity_t {
	bool valid = false;

	int health = 0;
	std::string name = "";
	vec3_t origin, top_origin;
};

extern std::vector< esp_entity_t > entities;

extern void draw(dx_renderer* renderer);

DWORD WINAPI EntityInfoThread(LPVOID lpParam);
DWORD WINAPI RendererThread(LPVOID lpParam);