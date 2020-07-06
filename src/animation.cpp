#include "animation.h"

using namespace std;

const double PI = 3.141592653589793238462;

int bounce_count = 0;

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

void anim_parabola(mesh_t& mesh, double xs, double max_h, double xe, double zs, double ze, double frames) {
    double dx;
    double dy;
    double dz;
    double a_of_s;
    double a;
    double b;
    double slope;

    // Calculate a and b
    // b = -a_of_s * 2 * a
    // (-max_h) / (-a_of_s)^2 = a;

    // Test: -x^2 + 4x  // Normal
    // Test: -4x^2 + 8x // Steep
    // Test: -0.25x^2 + x   // Flat
    if(xs == 0) {
        a_of_s = (xe + xs) / 2;
        a = -1 * max_h / pow(a_of_s, 2);
        b = -1 * a_of_s * 2 * a;
        slope = 2 * a * mesh.pos.x + b;
    }
    else {
        double temp_xs = 0;
        double temp_xe = xe - xs;
        a_of_s = (temp_xe + temp_xs) / 2;
        a = -1 * max_h / pow(a_of_s, 2);
        b = -1 * a_of_s * 2 * a;
        slope = 2 * a * (mesh.pos.x - xs) + b;
    }

    dx = (xe - xs) / frames;
    dy = dx * slope;
    dz = (ze - zs) / frames;

    if(xe != mesh.pos.x) {

        if(abs(mesh.pos.x - xe) < dx) {
            mesh.pos.x = xe + dx;
            mesh.pos.y = default_pos.y;
        }
        else {
            anim_pos(mesh, dx, dy, dz);
        }
    }

    // printf("%d | %lf | %lf | %lf | %lf\n",
    //     bounce_count, xs, xe, mesh.pos.x, mesh.pos.y
    // );
    // else if(xe != mesh.pos.x && repeat) {
    //     if(abs(mesh.pos.x - xe) <= dx) {
    //         mesh.pos.x = xe;
    //         mesh.pos.y = 0;
    //     }
    //     else {
    //         anim_pos(mesh, dx, dy, dz);
    //     }
    // }
    // else {
    //     repeat = true;
    //     // temp = xe;
    //     // xe += (xe - xs);
    //     // xs = temp;
    //     // exit(0);
    // }

    // printf("%lf | %lf | %lf | %lf | %lf\n",
    //     mesh.pos.x, mesh.pos.y, dy, a, b
    // );
}

void anim_bounce(mesh_t& mesh, double xs, double max_h, double xe, double zs, double ze, int bounces, double frames, double current_frame) {

    double distance_x = xe - xs;
    double distance_z = ze - zs;

    bounce_count = (int) (current_frame - 3) / (int) frames;

    // if(bounce_count == bounces && mesh.pos.x > xe) {
    //     mesh.pos.x = distance * bounce_count + default_pos.x;
    // }

    if(bounce_count < bounces) {
        distance_x = xe - xs;
        distance_z = ze - zs;

        if(bounce_count == 0) {
            anim_parabola(mesh, xs, max_h, xe, zs, ze, frames);
        }
        else {
            xs += distance_x * bounce_count;
            xe += distance_x * bounce_count;
            zs += distance_z * bounce_count;
            ze += distance_z * bounce_count;
            anim_parabola(mesh, xs, max_h, xe, zs, ze, frames);
        }
    }
}

joint_t create_joint(double x, double y, double len, double theta) {
    // joint_t joint = { .mesh = { .rot = default_rot, .scale = default_scale, .pos = default_pos },
    //                   .pos1 = { .x = x, .y = y }, .len = len, .pos2 = { .x = x1, .y = y1 } };
    // joint.mesh = load_obj_mesh_data(joint.mesh, "./models/bone.obj");
    // joint.mesh.vertices[0].x = joint.pos2.x;    // Top vertex
    // joint.mesh.vertices[2].x = joint.pos1.x;    // Bottom vertex
    joint_t joint = { .pos1 = { .x = x, .y = y }, .len = len, .theta = theta, .self_theta = theta };
    joint.pos2 = { .x = x + len * cos(theta), .y = y + len * sin(theta) };
    joint.parent = nullptr;
    return joint;
}

joint_t create_joint(joint_t* parent, double len, double theta) {
    joint_t joint = { .pos1 = { .x = parent->pos2.x, .y = parent->pos2.y }, .len = len, .theta = theta, .self_theta = theta, .parent = parent };
    joint.pos2 = { .x = parent->pos2.x + len * cos(theta), .y = parent->pos2.y + len * sin(theta) };
    return joint;
}

// Draw joints as line segments
void joint_render(joint_t& joint) {
    joint.theta = joint.self_theta;
    if(joint.parent != nullptr) {
        joint.pos1.x = joint.parent->pos2.x;
        joint.pos1.y = joint.parent->pos2.y;
        joint.theta += joint.parent->theta;
    }
    joint.pos2 = { .x = joint.pos1.x + joint.len * cos(joint.theta), .y = joint.pos1.y + joint.len * sin(joint.theta) };
    draw_line(joint.pos1.x, joint.pos1.y, joint.pos2.x, joint.pos2.y, yellow);
}
