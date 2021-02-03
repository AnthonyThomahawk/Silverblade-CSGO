#include "stdafx.h"
#include "OverlayESP.h"



vec3_t::vec3_t() {
	x = y = z = 0;
}

vec3_t::vec3_t(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

vec3_t vec3_t::operator+(const vec3_t& other) {
	return { x + other.x, y + other.y, z + other.z };
}

vec3_t vec3_t::operator+=(const vec3_t& other) {
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

vec3_t vec3_t::operator-(const vec3_t& other) {
	return { x - other.x, y - other.y, z - other.z };
}

vec3_t vec3_t::operator-=(const vec3_t& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

vec3_t vec3_t::operator*(float factor) {
	return { x * factor, y * factor, z * factor };
}

vec3_t vec3_t::operator*(const vec3_t& other) {
	return { x * other.x, y * other.y, z * other.z };
}

vec3_t vec3_t::operator*=(float factor) {
	x *= factor;
	y *= factor;
	z *= factor;

	return *this;
}

vec3_t vec3_t::operator*=(const vec3_t& other) {
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

vec3_t vec3_t::operator/(float factor) {
	return { x / factor, y / factor, z / factor };
}

vec3_t vec3_t::operator/(const vec3_t& other) {
	return { x / other.x, y / other.y, z / other.z };
}

vec3_t vec3_t::operator/=(float factor) {
	x /= factor;
	y /= factor;
	z /= factor;

	return *this;
}

vec3_t vec3_t::operator/=(const vec3_t& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;

	return *this;
}

bool vec3_t::is_zero() {
	return x == 0 && y == 0 && z == 0;
}

float vec3_t::dot(const vec3_t& other) {
	return x * other.x + y * other.y + z * other.z;
}

float vec3_t::dist(const vec3_t& other) {
	return (*this - other).length();
}

float vec3_t::length() {
	return sqrt(length_sqr());
}

float vec3_t::length_sqr() {
	return x * x + y * y + z * z;
}

bool world_to_screen(const vec3_t& screen_size, const vec3_t& pos, vec3_t& out, view_matrix_t matrix) {
	out.x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	out.y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	if (w < 0.01f)
		return false;

	float inv_w = 1.f / w;
	out.x *= inv_w;
	out.y *= inv_w;

	float x = screen_size.x * .5f;
	float y = screen_size.y * .5f;

	x += 0.5f * out.x * screen_size.x + 0.5f;
	y -= 0.5f * out.y * screen_size.y + 0.5f;

	out.x = x;
	out.y = y;

	return true;
}

#define ent_netvar( nv_type, nv_name, nv_offset ) nv_type entity_t::##nv_name( ) { return Globals::gamememory.Read<nv_type>( m_ptr + nv_offset );  }

ent_netvar(bool, dormant, Offsets::bDormant);

ent_netvar(int, health, Offsets::hpoffset);
ent_netvar(int, index, 0x64);
ent_netvar(int, life_state, 0x25F);
ent_netvar(int, team, Offsets::dwTeam);

ent_netvar(vec3_t, origin, 0x138);

entity_t::entity_t(std::uintptr_t ent_ptr) {
	m_ptr = ent_ptr;
}

void entity_t::get_name(std::string& out) {
	struct player_info_t {
		char __pad[0x10];
		char name[32];
	};

	std::uintptr_t client_state = Globals::gamememory.Read< std::uintptr_t >(Globals::dwEngine + Offsets::clientstate); //m_dwClientState
	std::uintptr_t user_info_table = Globals::gamememory.Read< std::uintptr_t >(client_state + Offsets::dwClientState_PlayerInfo); //m_dwClientState_PlayerInfo
	std::uintptr_t x = Globals::gamememory.Read< std::uintptr_t >(Globals::gamememory.Read< std::uintptr_t >(user_info_table + 0x40) + 0xC);
	player_info_t p = Globals::gamememory.Read< player_info_t >(Globals::gamememory.Read< uintptr_t >(x + 0x28 + 0x34 * (index() - 1)));

	out.resize(32);
	memcpy(out.data(), p.name, 32);
}

bool entity_t::is_alive() {
	return life_state() == 0;
}

//esp
vec3_t screen_size;


std::vector< esp_entity_t > entities(64);

void draw(dx_renderer* renderer) {
	//Create our vectors for the 2D screen position
	vec3_t screen_bottom, screen_top;

	//Get the module base for client_panorama.dll
	static std::uintptr_t client_dll = Globals::dwClient;

	//Grab the view matrix
	view_matrix_t view_matrix = Globals::gamememory.Read< view_matrix_t >(client_dll + Offsets::dwViewMatrix); //view_matrix

	//Loop our vector containing the entities
	for (auto& ent : entities) {
		//If our entity isn't valid, skip it
		if (!ent.valid)
			continue;

		//Convert 3D space coordinates to 2D screen coordinates
		if (!world_to_screen(screen_size, ent.origin, screen_bottom, view_matrix) || !world_to_screen(screen_size, ent.top_origin, screen_top, view_matrix))
			continue;

		//Box properties
		int box_height = screen_bottom.y - screen_top.y;
		int box_width = box_height / 2;

		//Draw the ESP
		if (Globals::renderboxes)
			renderer->draw_outlined_rect(screen_top.x - box_width / 2, screen_top.y, box_width, box_height, D3DCOLOR_RGBA(65, 135, 245, 255));
		if (Globals::rendernames)
			renderer->draw_text(ent.name, screen_top.x, screen_top.y - 14, 0xFFFFFFFF);
		if (Globals::renderhps)
			renderer->draw_text(std::to_string(ent.health) + " HP", screen_top.x, screen_bottom.y, 0xFFFFFFFF);
	}
}

DWORD WINAPI EntityInfoThread(LPVOID lpParam)
{
	std::vector< ent_info_t > ent_info(64);

	while (true) {
		Sleep(1);
		//Invalidate all ESP entities as we're updating them
		for (auto& esp_ent : entities) // entities is the ent_info_t table the renderer uses
			esp_ent.valid = false;

		//Get clientstate for the entitylist
		std::uintptr_t client_state = Globals::gamememory.Read < std::uintptr_t >(Globals::dwEngine + Offsets::clientstate); //m_dwClientState

		//Read the whole entity list at once
		Globals::gamememory.Read_ex< ent_info_t >(ent_info.data(), Globals::dwClient + Offsets::dwEntityList, ent_info.size()); //m_dwEntityList

		//Get our local player ptr
		int local_player_idx = Globals::gamememory.Read< int >(client_state + 0x180); //m_dwClientState_GetLocalPlayer
		std::uintptr_t local_ptr = ent_info[local_player_idx].entity_ptr;

		//Is our local player ptr valid?
		if (!local_ptr)
			continue;

		//Get our local player
		entity_t local(local_ptr);

		//Gather entity information for our ESP
		for (std::size_t i = 0; i < ent_info.size(); i++) {
			std::uintptr_t ent_ptr = ent_info[i].entity_ptr;

			//Entity is invalid, don't draw on ESP
			if (!ent_ptr)
				continue;

			//Create an entity object so we can get information the easy way
			entity_t entity(ent_ptr);

			//Continue if entity is dormant or dead
			if (entity.dormant() || !entity.is_alive())
				continue;
			
			if (entity.team() == local.team() && Globals::renderonlyenemy)
				continue;

			//We have a valid entity, get a reference to it for ease of use
			esp_entity_t& esp_entity = entities[i];

			//Get entity information for our ESP
			esp_entity.health = entity.health();
			entity.get_name(esp_entity.name);
			esp_entity.origin = entity.origin();
			esp_entity.top_origin = esp_entity.origin + vec3_t(0.f, 0.f, 75.f);

			//Our ESP entity is now valid to draw
			esp_entity.valid = true;
		}
	}
}

DWORD WINAPI RendererThread(LPVOID lpParam)
{
	screen_size.x = 1920;
	screen_size.y = 1080;
	screen_size.z = 0;
	dx_renderer renderer = Globals::overlay.create_renderer();
	while (true)
	{
		//Render our ESP
		renderer.begin_rendering();
		renderer.draw_text(std::to_string(renderer.get_fps()), 2, 2, 0xFFFFFFFF, false);
		draw(&renderer);
		renderer.end_rendering();

		Sleep(1);
	}
}