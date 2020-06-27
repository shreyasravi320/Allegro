#include "matrix.h"

using namespace std;

mat4_t mat4_identity() {
    // | 1 0 0 0 |
    // | 0 1 0 0 |
    // | 0 0 1 0 |
    // | 0 0 0 1 |

    mat4_t m = {{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }};

    return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
    vec4_t result;

    // Matrix        * Vector 4
    // [x1 y1 z1 w1] * [x]
    // [x2 y2 z2 w2] * [y]
    // [x3 y3 z3 w3] * [z]
    // [x4 y4 z4 w4] * [w]

    result.x = m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * v.w;
    result.y = m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * v.w;
    result.z = m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * v.w;
    result.w = m.mat[3][0] * v.x + m.mat[3][1] * v.y + m.mat[3][2] * v.z + m.mat[3][3] * v.w;

    return result;
}

// Multiply 2 Matrix4s together
mat4_t mat4_mul_mat(mat4_t a, mat4_t b) {

    mat4_t c = mat4_identity();
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            // Normally there would be another for loop in here if the matrices were not always the same size
            // But since it's always 4x4, we can reduce the power of runtime by 1 with a little extra manual code
            c.mat[i][j] =
                a.mat[i][0] * b.mat[0][j] +
                a.mat[i][1] * b.mat[1][j] +
                a.mat[i][2] * b.mat[2][j] +
                a.mat[i][3] * b.mat[3][j];
        }
    }
    return c;
}

// Create a matrix for scaling vertices
mat4_t mat4_scale(float sx, float sy, float sz) {
    // | sx  0  0  0 |
    // |  0 sy  0  0 |
    // |  0  0 sz  0 |
    // |  0  0  0  1 |

    // After multiplying by vector4, result:
    // |  sx  0  0  0  |
    // |   0 sy  0  0  |
    // |   0  0 sz  0  |
    // |   0  0  0  1  |

    // Simplifies to
    // | sx |
    // | sy |
    // | sz |
    // |  1 |

    mat4_t m = mat4_identity();
    m.mat[0][0] = sx;
    m.mat[1][1] = sy;
    m.mat[2][2] = sz;

    return m;
}

// Create a matrix for translating vertices
mat4_t mat4_translate(float dx, float dy, float dz) {
    // |  1  0  0 dx |
    // |  0  1  0 dy |
    // |  0  0  1 dz |
    // |  0  0  0  1 |

    // After multiplying by vector4, result:
    // |  x 0 0 dx  |
    // |  0 y 0 dy  |
    // |  0 0 z dz  |
    // |  0 0 0  1  |

    // Simplifies to
    // | x + dx |
    // | y + dy |
    // | z + dz |
    // |   1    |

    mat4_t m = mat4_identity();
    m.mat[0][3] = -dx;
    m.mat[1][3] = -dy;
    m.mat[2][3] = -dz;

    return m;
}

// Create a matrix for translating vertices around z axis
mat4_t mat4_rotate_z(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    // |  c -s  0  0  |
    // |  s  c  0  0  |
    // |  0  0  1  0  |
    // |  0  0  0  1  |

    // After multiply by vector4 and simplifying, result:
    // | xcos(theta) - ysin(theta) |
    // | xsin(theta) + ycos(theta) |
    // |             z             |
    // |             1             |

    mat4_t m = mat4_identity();
    m.mat[0][0] = c;
    m.mat[0][1] = -s;
    m.mat[1][0] = s;
    m.mat[1][1] = c;

    return m;
}

// Create a matrix for translating vertices around x axis
mat4_t mat4_rotate_x(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    // |  1  0  0  0  |
    // |  0  c -s  0  |
    // |  0  s  c  0  |
    // |  0  0  0  1  |

    // After multiply by vector4 and simplifying, result:
    // |             x             |
    // | ycos(theta) - zsin(theta) |
    // | ysin(theta) + zcos(theta) |
    // |             1             |

    mat4_t m = mat4_identity();
    m.mat[1][1] = c;
    m.mat[1][2] = -s;
    m.mat[2][1] = s;
    m.mat[2][2] = c;

    return m;
}

// Create a matrix for translating vertices around y axis
mat4_t mat4_rotate_y(float theta) {
    float c = cos(theta);
    float s = sin(theta);

    // |  c  0 -s  0  |
    // |  0  1  0  0  |
    // |  s  0  c  0  |
    // |  0  0  0  1  |

    // After multiply by vector4 and simplifying, result:
    // | xcos(theta) - zsin(theta) |
    // |             y             |
    // | xsin(theta) + zcos(theta) |
    // |             1             |

    mat4_t m = mat4_identity();
    m.mat[0][0] = c;
    m.mat[0][2] = -s;
    m.mat[2][0] = s;
    m.mat[2][2] = c;

    return m;
}
