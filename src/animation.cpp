#include "animation.h"

using namespace std;

const double PI = 3.141592653589793238462;

bool repeat = false;

void anim_pos(mesh_t& mesh, double dx, double dy, double dz) {
    mesh.pos.x += dx;
    mesh.pos.y += dy;
    mesh.pos.z += dz;
}

void anim_line(mesh_t& mesh, double xs, double ys, double zs, double xe, double ye, double ze, double frames) {
    double dx;
    double dy;
    double dz;

    if(!(xe == mesh.pos.x && ye == mesh.pos.y && ze == mesh.pos.z)) {
        anim_pos(mesh, dx, dy, dz);

        dx = (xe - xs) / frames;
        dy = (ye - ys) / frames;
        dz = (ze - zs) / frames;

        if(abs(xe - mesh.pos.x) < dx) mesh.pos.x = xe;
        if(abs(ye - mesh.pos.y) < dy) mesh.pos.y = ye;
        if(abs(ze - mesh.pos.z) < dz) mesh.pos.z = ze;
    }

    // printf("%lf %lf %d | %lf %lf %d | %lf %lf %d\n",
    //     mesh.pos.x, xe, xe == mesh.pos.x,
    //     mesh.pos.y, ye, ye == mesh.pos.y,
    //     mesh.pos.z, ze, ze == mesh.pos.z
    // );
}

void anim_parabola(mesh_t& mesh, double xs, double max_h, double xe, double frames) {
    double dx;
    double dy;
    double dz;
    double temp;

    if(repeat) {
        temp = xe;
        xe += (xe - xs);
        xs = temp;
    }

    double a_of_s = (xe + xs) / 2;
    // Calculate a and b
    // b = -a_of_s * a
    // (-max_h) / (-a_of_s)^2 = a;

    // Test: -x^2 + 4x  // Normal
    // Test: -4x^2 + 8x // Steep
    // Test: -0.25x^2 + x   // Flat
    double a = -1 * max_h / pow(a_of_s, 2);
    double b = -1 * a_of_s * 2 * a;
    double slope = 2 * a * mesh.pos.x + b;

    dx = (xe - xs) / frames;
    dy = dx * slope;

    if(xe + dx != mesh.pos.x && !repeat) {
        // repeat = false;
        if(abs(mesh.pos.x - xe) <= dx) {
            mesh.pos.x = xe + dx;
            mesh.pos.y = 0;
        }
        else {
            anim_pos(mesh, dx, dy, dz);
        }
    }
    else if(xe + dx != mesh.pos.x && repeat) {
        if(abs(mesh.pos.x - xe) <= dx) {
            mesh.pos.x = xe;
            mesh.pos.y = 0;
        }
        else {
            anim_pos(mesh, dx, dy, dz);
        }
    }
    else {
        // temp = xe;
        // xe += (xe - xs);
        // xs = temp;
        repeat = true;
        // printf("%s | %lf | %lf\n", "Completed", xs, xe);
        // exit(0);
    }

    // printf("%lf | %lf | %lf | %lf | %lf | %lf | %lf | %lf\n",
    //     mesh.pos.x, mesh.pos.y, a, b, dx, dy, xs, xe
    // );
}

void anim_sin(double& value, double frame, double speed) {
    value = sin(frame * speed);
}
