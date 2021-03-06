#include "hydro.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#define PGM_8_BIT 255
#define PGM_16_BIT 65535
#define IF_OUT_OF_BOUNDS if (x < 0 || y < 0 || x >= static_cast <int> (heightmap.size()) || y >= static_cast <int> (heightmap[0].size()))
#define GRAVITY 10.f
#define NORMALIZATION_FACTOR 400
#define TIME_STEP 0.1f
#define FOR_EACH_CELL for (int i = 0; i < static_cast <int> (heightmap.size()); i++) { for (int k = 0; k < static_cast <int> (heightmap[0].size()); k++) {
#define FOR_EACH_CELL_2 for (int i = static_cast <int> (heightmap.size()) - 1; i >= 0; i--) { for (int k = 0; k < static_cast <int> (heightmap[0].size()); k++) {
#define END }}
#define MINIMUM_EVAPORATION_THRESHOLD 0.01f


hydro :: hydro() {

    water = 0;
    capacity = 0;
    erosion_rate = 0;
    deposition_rate = 0;
    evaporation_rate = 0;
    post_evaporation_rate = 0;
    drops = false;

}


void hydro :: load_heightmap() {

    using namespace std;

    // open .pgm image
    ifstream terrain_data("../terrain/heightmap.pgm");

    // or don't
    if (terrain_data.fail()) return;

    int terrain_size = 0;
    string happy_string, unhappy_string, str;
    istringstream happy_string_stream;

    // image file checks
    for (int i = 0; i < 3; i++) {

        getline(terrain_data, happy_string);

        // check P2 header
        if (i == 0 && happy_string.compare("P2") != 0) return;

        // get terrain dimensions
        else if (i == 1) terrain_size = stoi(happy_string.substr(0, happy_string.find(' ')));

        // accept 8-bit or 16-bit pgm only.
        else if (i == 2 && stoi(happy_string) != PGM_8_BIT && stoi(happy_string) != PGM_16_BIT) return;
    }

    heightmap.clear();
    temp_heightmap.clear();
    temp_sedimap.clear();

    for (int i = 0; i < terrain_size; i++) {

        getline(terrain_data, happy_string);

        istringstream unhappy_string_stream(happy_string);

        vector <float> pixel_row;

        while (getline(unhappy_string_stream, unhappy_string, ' ')) pixel_row.push_back(stof(unhappy_string));

        vector <float> temp_row(pixel_row.size(), 0.f);

        heightmap.push_back(pixel_row);
        temp_heightmap.push_back(temp_row);
        temp_sedimap.push_back(temp_row);
    }

}


hydro_flux hydro :: compute_flux(int x, int y, float current_height) {

    hydro_flux flux;

    IF_OUT_OF_BOUNDS return flux;

    if (x - 1 < 0) flux.up = 0;
    else flux.up = std :: max(0.f, current_map -> flux_field[x][y] -> up + TIME_STEP * GRAVITY * (current_height - heightmap[x - 1][y] - current_map -> watermap[x - 1][y]));

    if (y - 1 < 0) flux.left = 0;
    else flux.left = std :: max(0.f, current_map -> flux_field[x][y] -> left + TIME_STEP * GRAVITY * (current_height - heightmap[x][y - 1] - current_map -> watermap[x][y - 1]));

    if (x + 1 >= static_cast <int> (heightmap.size())) flux.down = 0;
    else flux.down = std :: max(0.f, current_map -> flux_field[x][y] -> down + TIME_STEP * GRAVITY * (current_height - heightmap[x + 1][y] - current_map -> watermap[x + 1][y]));

    if (y + 1 >= static_cast <int> (heightmap[0].size())) flux.right = 0;
    else flux.right = std :: max(0.f, current_map -> flux_field[x][y] -> right + TIME_STEP * GRAVITY * (current_height - heightmap[x][y + 1] - current_map -> watermap[x][y + 1]));

    return flux;

}


void hydro :: scale_flux(int x, int y, float current_height) {

    if (current_height < 0) return;

    IF_OUT_OF_BOUNDS return;

    hydro_flux flux;

    flux = compute_flux(x, y, current_height);

    if (flux.sum() <= 0) flux.set_zeroes();
    else if (flux.sum() > current_map -> watermap[x][y]) flux.scale(current_map -> watermap[x][y] / (flux.sum() * TIME_STEP));

    *(updated_map -> flux_field[x][y])  = flux;
    *(current_map -> flux_field[x][y]) = flux;

}


void hydro :: update_water_level(int x, int y) {

    IF_OUT_OF_BOUNDS return;

    updated_map -> watermap[x][y] = std :: max(0.f, current_map -> watermap[x][y] + TIME_STEP * (updated_map -> inflow_sum(x, y) - updated_map -> flux_field[x][y] -> sum()));

    if (updated_map -> watermap[x][y] < 0) updated_map -> watermap[x][y] = 0;

}


normal_vector hydro :: normal(int x, int y) {

    normal_vector normal;

    int triangles = 0;

    normal.x = 0;
    normal.y = 0;
    normal.z = 0;

    float vector_vx;
    float vector_vy;
    float vector_vz;

    float vector_ux;
    float vector_uy;
    float vector_uz;

    if (x - 1 >= 0 && y - 1 >= 0) {

        vector_vx = 0;
        vector_vy = 1;
        vector_vz = heightmap[x - 1][y] - heightmap[x][y];

        vector_ux = -1;
        vector_uy = 0;
        vector_uz = heightmap[x][y - 1] - heightmap[x][y];

        normal.x += ((vector_vy * vector_uz) - (vector_vz * vector_uy));
        normal.y += ((vector_vz * vector_ux) - (vector_vx * vector_uz));
        normal.z += ((vector_vx * vector_uy) - (vector_vy * vector_ux));

        triangles++;

    }

    if (y - 1 >= 0 && x + 1 < static_cast <int> (heightmap.size())) {

        vector_vx = -1;
        vector_vy = 0;
        vector_vz = heightmap[x][y - 1] - heightmap[x][y];

        vector_ux = 0;
        vector_uy = -1;
        vector_uz = heightmap[x + 1][y] - heightmap[x][y];

        normal.x += ((vector_vy * vector_uz) - (vector_vz * vector_uy));
        normal.y += ((vector_vz * vector_ux) - (vector_vx * vector_uz));
        normal.z += ((vector_vx * vector_uy) - (vector_vy * vector_ux));

        triangles++;

    }

    if (x + 1 < static_cast <int> (heightmap.size()) && y + 1 < static_cast <int> (heightmap[0].size())) {

        vector_vx = 0;
        vector_vy = -1;
        vector_vz = heightmap[x + 1][y] - heightmap[x][y];

        vector_ux = 1;
        vector_uy = 0;
        vector_uz = heightmap[x][y + 1] - heightmap[x][y];

        normal.x += ((vector_vy * vector_uz) - (vector_vz * vector_uy));
        normal.y += ((vector_vz * vector_ux) - (vector_vx * vector_uz));
        normal.z += ((vector_vx * vector_uy) - (vector_vy * vector_ux));

        triangles++;

    }


    if (y + 1 < static_cast <int> (heightmap[0].size()) && x - 1 >= 0) {

        vector_vx = 1;
        vector_vy = 0;
        vector_vz = heightmap[x][y + 1] - heightmap[x][y];

        vector_ux = 0;
        vector_uy = 1;
        vector_uz = heightmap[x - 1][y] - heightmap[x][y];

        normal.x += ((vector_vy * vector_uz) - (vector_vz * vector_uy));
        normal.y += ((vector_vz * vector_ux) - (vector_vx * vector_uz));
        normal.z += ((vector_vx * vector_uy) - (vector_vy * vector_ux));

        triangles++;

    }

    if (triangles > 0) {

        normal.x /= triangles;
        normal.y /= triangles;
        normal.z /= triangles;

    }

    if (normal.x == 0 && normal.y == 0 && normal.z == 0) normal.z = 1;

    return normal;

}


float hydro :: incline_sin(normal_vector normal) { return std :: max(0.1f, (float)sin(acos(normal.z / vector_length(normal.x, normal.y, normal.z) * vector_length(0, 0, 1)))); }


float hydro :: vector_length(float x, float y, float z) { return (sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2))); }


float hydro :: transport_capacity(int x, int y) { return capacity * incline_sin(normal(x, y)) * updated_map -> velocity_field[x][y] -> magnitude(); }


float hydro :: euler_step(float x, float y) {

    // not on the map == no sediment
    if (floor(x) < 0) return 0;
    if (floor(y) < 0) return 0;
    if (ceil(x) >= static_cast <int> (heightmap.size())) return 0;
    if (ceil(y) >= static_cast <int> (heightmap[0].size())) return 0;

    if (x == floor(x) && x == ceil(x) && y == floor(y) && y == ceil(y)) return current_map -> sedimap[x][y];
    if (x == floor(x) && x == ceil(x)) return current_map -> sedimap[x][floor(y)] * (y - floor(y)) + current_map -> sedimap[x][ceil(y)] * (1 - (y - floor(y)));
    if (y == floor(y) && y == ceil(y)) return current_map -> sedimap[floor(x)][y] * (x - floor(x)) + current_map -> sedimap[ceil(x)][y] * (1 - (x - floor(x)));

    float y_diff = y - floor(y);
    float top    = current_map -> sedimap[floor(x)][floor(y)] * (y_diff) + current_map -> sedimap[floor(x)][ceil(y)] * (1 - y_diff);
    float bottom = current_map -> sedimap[ ceil(x)][floor(y)] * (y_diff) + current_map -> sedimap[ceil(x) ][ceil(y)] * (1 - y_diff);

    float x_diff = x - floor(x);
    return top * (x_diff) + bottom * (1 - x_diff);

}


void hydro :: erosion_deposition(int x, int y) {

    if (transport_capacity(x, y) > current_map -> sedimap[x][y]) {

        if (heightmap[x][y] <= erosion_rate * (transport_capacity(x, y) - current_map -> sedimap[x][y])) {
            current_map -> sedimap[x][y] += heightmap[x][y];
            temp_heightmap[x][y] = -heightmap[x][y];
        } else {
            temp_heightmap[x][y] = -erosion_rate * (transport_capacity(x, y) - current_map -> sedimap[x][y]);
            current_map -> sedimap[x][y] += erosion_rate * (transport_capacity(x, y) - current_map -> sedimap[x][y]);
        }

    } else {

        temp_heightmap[x][y] = deposition_rate * (current_map -> sedimap[x][y] - transport_capacity(x, y));
        current_map -> sedimap[x][y] -= deposition_rate * (current_map -> sedimap[x][y] - transport_capacity(x, y));

    }

}


void hydro :: update_cell(int x, int y) {

    current_map -> watermap[x][y] = updated_map -> watermap[x][y];
    current_map -> sedimap[x][y] = updated_map -> sedimap[x][y];

    *(current_map -> velocity_field[x][y]) = *(updated_map -> velocity_field[x][y]);
    *(current_map -> flux_field[x][y]) = *(updated_map -> flux_field[x][y]);

}

void hydro :: set_parameters(float water, float capacity, float erosion_rate, float deposition_rate, float evaporation_rate, float post_evaporation_rate, bool drops) {

    this -> water = water;
    this -> capacity = capacity;
    this -> erosion_rate = erosion_rate;
    this -> deposition_rate = deposition_rate;
    this -> evaporation_rate = evaporation_rate;
    this -> post_evaporation_rate = post_evaporation_rate;
    this -> drops = drops;

}


void hydro :: erode(int cycles) {

    load_heightmap();

    FOR_EACH_CELL heightmap[i][k] /= NORMALIZATION_FACTOR; END

    current_map = new hydro_map(static_cast <int> (heightmap.size()), static_cast <int> (heightmap[0].size()));
    updated_map = new hydro_map(static_cast <int> (heightmap.size()), static_cast <int> (heightmap[0].size()));

    for (int cycle = 0; cycle < cycles; cycle++) {

        // distribute water
        if (drops) { FOR_EACH_CELL drop(i, k); END }

        else { FOR_EACH_CELL current_map -> watermap[i][k] += water * TIME_STEP; END }

        // compute new flux
        FOR_EACH_CELL scale_flux(i, k, heightmap[i][k] + current_map -> watermap[i][k]); END

        // update water level based on flux
        FOR_EACH_CELL update_water_level(i, k); END

        // velocity field
        FOR_EACH_CELL updated_map -> update_velocity(i, k, current_map -> watermap[i][k], updated_map -> watermap[i][k]); END

        // erosion and deposition
        FOR_EACH_CELL erosion_deposition(i, k); END

        // update sediment
        FOR_EACH_CELL heightmap[i][k] += temp_heightmap[i][k]; END

        // sediment transport
        FOR_EACH_CELL temp_sedimap[i][k] = euler_step(i - updated_map -> velocity_field[i][k] -> x * TIME_STEP, k - updated_map -> velocity_field[i][k] -> y * TIME_STEP); END

        // update sediment
        FOR_EACH_CELL updated_map -> sedimap[i][k] = temp_sedimap[i][k]; END

        // evaporation
        FOR_EACH_CELL

            if (updated_map -> watermap[i][k] <= 0.001) updated_map -> watermap[i][k] = 0;
            updated_map -> watermap[i][k] *= (1 - evaporation_rate * TIME_STEP);

        END

        // update parameters for next iteration
        FOR_EACH_CELL update_cell(i, k); END

    }

    while(is_wet()) {

        // compute new flux
        FOR_EACH_CELL scale_flux(i, k, heightmap[i][k] + current_map -> watermap[i][k]); END

        // update water level based on flux
        FOR_EACH_CELL update_water_level(i, k); END

        // velocity field
        FOR_EACH_CELL updated_map -> update_velocity(i, k, current_map -> watermap[i][k], updated_map -> watermap[i][k]); END

        // erosion and deposition
        FOR_EACH_CELL erosion_deposition(i, k); END

        // update sediment
        FOR_EACH_CELL heightmap[i][k] += temp_heightmap[i][k]; END

        // sediment transport
        FOR_EACH_CELL temp_sedimap[i][k] = euler_step(i - updated_map -> velocity_field[i][k] -> x * TIME_STEP, k - updated_map -> velocity_field[i][k] -> y * TIME_STEP); END

        // update sediment again
        FOR_EACH_CELL updated_map -> sedimap[i][k] = temp_sedimap[i][k]; END

        // evaporation
        FOR_EACH_CELL

            updated_map -> watermap[i][k] *= (1 - post_evaporation_rate * TIME_STEP);
            if (updated_map -> watermap[i][k] <= 0.001) updated_map -> watermap[i][k] = 0;

        END

        // update parameters for next iteration
        FOR_EACH_CELL update_cell(i, k); END

    }

    // output results
    output_heightmap();

    current_map -> velocity_field.clear();
    current_map -> velocity_field.clear();
    updated_map -> flux_field.clear();
    updated_map -> flux_field.clear();

    delete current_map;
    delete updated_map;

}


void hydro :: output_heightmap() {

    std :: ofstream happy_file;

    // eroded heightmap for rendering
    happy_file.open("../terrain/heightmap_eroded.pgm");
    happy_file << "P2\n";
    happy_file << heightmap.size() << ' ' << heightmap[0].size() << '\n';
    happy_file << "65535\n";

    FOR_EACH_CELL

        if (k == static_cast <int> (heightmap[0].size()) - 1) happy_file << floor(heightmap[i][k] * 400) << '\n';
        else happy_file << floor(heightmap[i][k] * 400)  << ' ';

    END

    happy_file.close();

    // eroded heightmap for preview
    happy_file.open("../terrain/heightmap_eroded_preview.ppm");
    happy_file << "P3\n";
    happy_file << heightmap.size() << ' ' << heightmap[0].size() << '\n';
    happy_file << "65535\n";

    FOR_EACH_CELL

        happy_file << static_cast <int> (floor(heightmap[i][k] * 400 * 0.85)) << ' ';
        happy_file << static_cast <int> (floor(heightmap[i][k] * 400 * 0.85)) << ' ';

        if (k == static_cast <int> (heightmap[0].size()) - 1) happy_file << floor(heightmap[i][k] * 400) << '\n';
        else happy_file << floor(heightmap[i][k] * 400) << ' ';

    END

    happy_file.close();

}


void hydro :: dynamic_load() {

    load_heightmap();

    FOR_EACH_CELL heightmap[i][k] /= NORMALIZATION_FACTOR ; END

    current_map = new hydro_map(static_cast <int> (heightmap.size()), static_cast <int> (heightmap[0].size()));
    updated_map = new hydro_map(static_cast <int> (heightmap.size()), static_cast <int> (heightmap[0].size()));

}


void hydro :: drop(int x, int y) {

    int factor = 32;
    int live_cells = 0;

    if (rand() % 32 == 0) {

        for (int i = -2; i < 3; i++) {
            for (int k = -2; k < 3; k++) {
                if (!((i == -2 || i == 2 || k == -2 || k == 2) && (k != 0 || i != 0))) {
                    if (is_in_bounds(x + i, y + k)) live_cells++;
                }
            }
        }

        // it can't be, but who knows...
        if (live_cells == 0) return;

        for (int i = -2; i < 3; i++) {
            for (int k = -2; k < 3; k++) {
                if (!((i == -2 || i == 2 || k == -2 || k == 2) && (k != 0 || i != 0))) {
                    if (is_in_bounds(x + i, y + k)) current_map -> watermap[x + i][y + k] += (factor * water * TIME_STEP / live_cells);

                }
            }
        }
    }
}


void hydro :: dynamic_erode() {

    // distribute water
    if (drops) { FOR_EACH_CELL drop(i, k); END }

    else { FOR_EACH_CELL current_map -> watermap[i][k] += water * TIME_STEP; END }

    // compute new flux
    FOR_EACH_CELL scale_flux(i, k, heightmap[i][k] + current_map -> watermap[i][k]); END

    // update water level based on flux
    FOR_EACH_CELL update_water_level(i, k); END

    // velocity field
    FOR_EACH_CELL updated_map -> update_velocity(i, k, current_map -> watermap[i][k], updated_map -> watermap[i][k]); END

    // erosion and deposition
    FOR_EACH_CELL erosion_deposition(i, k); END

    // update sediment
    FOR_EACH_CELL heightmap[i][k] += temp_heightmap[i][k]; END

    // sediment transport
    FOR_EACH_CELL temp_sedimap[i][k] = euler_step(i - updated_map -> velocity_field[i][k] -> x * TIME_STEP, k - updated_map -> velocity_field[i][k] -> y * TIME_STEP); END

    // update sediment again
    FOR_EACH_CELL updated_map -> sedimap[i][k] = temp_sedimap[i][k]; END

    // evaporation
    FOR_EACH_CELL

        updated_map -> watermap[i][k] *= (1 - evaporation_rate * TIME_STEP);
        if (updated_map -> watermap[i][k] <= 0.001) updated_map -> watermap[i][k] = 0;

    END

    // update parameters for next iteration
    FOR_EACH_CELL update_cell(i, k); END

}


void hydro :: dynamic_evaporate() {

    // compute new flux
    FOR_EACH_CELL scale_flux(i, k, heightmap[i][k] + current_map -> watermap[i][k]); END

    // update water level based on flux
    FOR_EACH_CELL update_water_level(i, k); END

    // velocity field
    FOR_EACH_CELL updated_map -> update_velocity(i, k, current_map -> watermap[i][k], updated_map -> watermap[i][k]); END

    // erosion and deposition
    FOR_EACH_CELL erosion_deposition(i, k); END

    // update sediment
    FOR_EACH_CELL heightmap[i][k] += temp_heightmap[i][k]; END

    // sediment transport
    FOR_EACH_CELL temp_sedimap[i][k] = euler_step(i - updated_map -> velocity_field[i][k] -> x * TIME_STEP, k - updated_map -> velocity_field[i][k] -> y * TIME_STEP); END

    // update sediment again
    FOR_EACH_CELL updated_map -> sedimap[i][k] = temp_sedimap[i][k]; END

    // evaporation
    FOR_EACH_CELL

        updated_map -> watermap[i][k] *= (1 - post_evaporation_rate * TIME_STEP);
        //updated_map -> watermap[i][k] -= MINIMUM_EVAPORATION_THRESHOLD;
        if (updated_map -> watermap[i][k] <= 0.001) updated_map -> watermap[i][k] = 0;

    END

    // update parameters for next iteration
    FOR_EACH_CELL update_cell(i, k); END

}


void hydro :: dynamic_delete() {

    current_map -> velocity_field.clear();
    current_map -> velocity_field.clear();
    updated_map -> flux_field.clear();
    updated_map -> flux_field.clear();

    delete current_map;
    delete updated_map;

}



bool hydro :: is_wet() {

    FOR_EACH_CELL if (current_map -> watermap[i][k] > 0) return true; END

    return false;

}


bool hydro :: is_in_bounds(int x, int y) {

    IF_OUT_OF_BOUNDS return false;

    return true;
}
