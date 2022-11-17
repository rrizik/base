#ifndef BASE_MATRIX_H
#define BASE_MATRIX_H

///////////////////////////////
// NOTE: Matrix Types
///////////////////////////////

typedef struct m2{
    union{
        struct {
            f32 _11, _12,
                _21, _22;
        };
        f32 array[4];
    };

    inline f32* operator[](u32 i){
        return(&(array[i * 2]));
    }

    inline m2(){
        _11 = _22 = 1.0f;
        _12 = _21 = 0.0f;
    }
    inline m2(f32 f11, f32 f12,
              f32 f21, f32 f22) {
        _11 = f11; _12 = f12;
        _21 = f21; _22 = f22;
    }
} m2;

typedef struct m3{
    union{
        struct{
            f32 _11, _12, _13,
                _21, _22, _23,
                _31, _32, _33;
        };
        f32 array[9];
    };

    inline f32* operator[](u32 i){
        return(&(array[i * 3]));
    }

    inline m3(){
        _11 = _22 = _33 = 1.0f;
        _12 = _13 = _21 = 0.0f;
        _23 = _31 = _32 = 0.0f;
    }

    inline m3(f32 f11, f32 f12, f32 f13,
              f32 f21, f32 f22, f32 f23,
              f32 f31, f32 ff32, f32 f33) {
        _11 = f11; _12 = f12; _13 = f13;
        _21 = f21; _22 = f22; _23 = f23;
        _31 = f31; _32 = ff32; _33 = f33;
    }
} m3;

typedef union m4{
    union{
        struct{
            f32 _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44;
        };
        f32 array[16];
    };

    inline f32* operator[](u32 i){
        return(&(array[i * 4]));
    }

    inline m4() {
        _11 = _22 = _33 = _44 = 1.0f;
        _12 = _13 = _14 = _21 = 0.0f;
        _23 = _24 = _31 = _32 = 0.0f;
        _34 = _41 = _42 = _43 = 0.0f;
    }
    inline m4(f32 f11, f32 f12, f32 f13, f32 f14,
                f32 f21, f32 f22, f32 f23, f32 f24,
                f32 f31, f32 ff32, f32 f33, f32 f34,
                f32 f41, f32 f42, f32 f43, f32 f44) {
        _11 = f11; _12 = f12; _13 = f13; _14 = f14;
        _21 = f21; _22 = f22; _23 = f23; _24 = f24;
        _31 = f31; _32 = ff32; _33 = f33; _34 = f34;
        _41 = f41; _42 = f42; _43 = f43; _44 = f44;
    }
} m4;

///////////////////////////////
// NOTE: Matrix Operators
///////////////////////////////

static m2 operator*(const m2& matrix, f32 scalar){
    m2 result = ZERO_INIT;
    for(u32 i=0; i<4; ++i){
        result.array[i] = matrix.array[i] * scalar;
    }
    return(result);
}
static m2 operator*(f32 scalar, const m2& matrix){
    m2 result = ZERO_INIT;
    for(u32 i=0; i<4; ++i){
        result.array[i] = matrix.array[i] * scalar;
    }
    return(result);
}
static m3 operator*(f32 scalar, const m3& matrix){
    m3 result = ZERO_INIT;
    for(u32 i=0; i<9; ++i){
        result.array[i] = matrix.array[i] * scalar;
    }
    return(result);
}
static m3 operator*(const m3& matrix, f32 scalar){
    m3 result = ZERO_INIT;
    for(u32 i=0; i<9; ++i){
        result.array[i] = matrix.array[i] * scalar;
    }
    return(result);
}
static m4 operator*(f32 scalar, const m4& matrix){
    m4 result = ZERO_INIT;
    for(u32 i=0; i<16; ++i){
        result.array[i] = matrix.array[i] * scalar;
    }
    return(result);
}
static m4 operator*(const m4& matrix, f32 scalar){
    m4 result = ZERO_INIT;
    for(u32 i=0; i<16; ++i){
        result.array[i] = matrix.array[i] * scalar;
    }
    return(result);
}

static bool
matrix_multiply(f32* result, const f32* mA, u32 rowsA, u32 colsA, const f32* mB, u32 rowsB, u32 colsB){
    if(colsA != rowsB){
        return(false);
    }
    for(u32 i=0; i<rowsA; ++i){
        for(u32 j=0; j<colsB; ++j){
            result[colsB * i + j] = 0.0f;
            for(u32 k=0; k<rowsB; ++k){
                u32 a = colsA * i + k;
                u32 b = colsB * k + j;
                result[colsB * i + j] += mA[a] * mB[b];
            }
        }
    }
    return(true);
}

static m2 operator*(const m2& mA, const m2& mB){
    m2 result = ZERO_INIT;
    matrix_multiply(result.array, mA.array, 2, 2, mB.array, 2, 2);
    return(result);
}

static m3 operator*(const m3& mA, const m3& mB){
    m3 result = ZERO_INIT;
    matrix_multiply(result.array, mA.array, 3, 3, mB.array, 3, 3);
    return(result);
}

static m4 operator*(const m4& mA, const m4& mB){
    m4 result = ZERO_INIT;
    matrix_multiply(result.array, mA.array, 4, 4, mB.array, 4, 4);
    return(result);
}

///////////////////////////////
// NOTE: Matrix Operations
///////////////////////////////
// TODO: profile these and make sure they dont take too long

static void
transpose_matrix(f32* src, f32* dst, u32 rows, u32 cols){
    for(u32 i=0; i < rows * cols; ++i){
        u32 row = i / rows;
        u32 col = i % rows;
        dst[i] = src[cols * col + row];
    }
}

static m2
transpose_m2(m2 m){
    m2 result = ZERO_INIT;
    transpose_matrix(m.array, result.array, 2, 2);
    return(result);
}

static m3
transpose_m3(m3 m){
    m3 result = ZERO_INIT;
    transpose_matrix(m.array, result.array, 3, 3);
    return(result);
}

static m4
transpose_m4(m4 m){
    m4 result = ZERO_INIT;
    transpose_matrix(m.array, result.array, 4, 4);
    return(result);
}

// NOTE: abs(determinant) is the scale factor of the transformation (2D area)
static float
determinant_m2(m2 m){
    float result = (m._11 * m._22) - (m._12 * m._21);
    return(result);
}

// QUESTION: I dont think I need this
static bool
has_inverse_m2(m2 m){
    bool result = determinant_m2(m) > 0 ? true : false;
    return(result);
}

static m2
cut_m3(m3 m, u32 row, u32 col){
    m2 result = ZERO_INIT;
    u32 index = 0;

    for(u32 i=0; i<3; ++i){
        for(u32 j=0; j<3; ++j){
            if(i == row || j == col){
                continue;
            }
            result.array[index++] = m.array[3 * i + j];
        }
    }
    return(result);
}

static m2
minor_m2(m2 m){
    m2 result = {m._22, m._21,
                 m._12, m._11};
    return(result);
}

static m3
minor_m3(m3 m){
    m3 result = ZERO_INIT;
    for(u32 i=0; i<3; ++i){
        for(u32 j=0; j<3; ++j){
            result[i][j] = determinant_m2(cut_m3(m, i, j));
        }
    }
    return(result);
}

static void
cofactor(f32* result, f32* minor, u32 rows, u32 cols) {
    for(u32 i=0; i<rows; ++i){
        for(u32 j=0; j<cols; ++j){
            u32 t = cols * j + i;
            u32 s = cols * j + i;
            f32 sign = powf(-1.0f, i + j);
            result[t] = minor[s] * sign;
        }
    }
}

static m2
cofactor_m2(m2 m){
    m2 result = ZERO_INIT;
    cofactor(result.array, minor_m2(m).array, 2, 2);
    return(result);
}

static m3
cofactor_m3(m3 m){
    m3 result = ZERO_INIT;
    cofactor(result.array, minor_m3(m).array, 3, 3);
    return(result);
}

static f32
determinant_m3(m3 m) {
    f32 result = 0.0f;
    m3 cof = cofactor_m3(m);
    for(u32 j = 0; j < 3; ++j){
        u32 index = 3 * 0 + j;
        result += m.array[index] * cof[0][j];
    }
    return(result);
}

static m3
cut_m4(m4 m, u32 row, u32 col){
    m3 result = ZERO_INIT;
    u32 index = 0;

    for(u32 i=0; i<4; ++i){
        for(u32 j = 0; j<4; ++j){
            if (i == row || j == col){
                continue;
            }
            u32 target = index++;
            u32 source = 4 * i + j;
            result.array[target] = m.array[source];
        }
    }
    return(result);
}

static m4
minor_m4(m4 m){
    m4 result = ZERO_INIT;
    for(u32 i=0; i<4; ++i){
        for(u32 j=0; j<4; ++j){
            result[i][j] = determinant_m3(cut_m4(m, i, j));
        }
    }
    return(result);
}

static m4
cofactor_m4(m4 m){
    m4 result = ZERO_INIT;
    cofactor(result.array, minor_m4(m).array, 4, 4);
    return(result);
}

static f32
determinant_m4(m4 m){
    f32 result = 0.0f;
    m4 cof = cofactor_m4(m);
    for(u32 j=0; j<4; ++j) {
        result += m.array[4 * 0 + j] * cof[0][j];
    }
    return(result);
}

static m2
adjugate_m2(m2 m){
    m2 result = transpose_m2(cofactor_m2(m));
    return(result);
}

static m3
adjugate_m3(m3 m){
    m3 result = transpose_m3(cofactor_m3(m));
    return(result);
}

static m4
adjugate_m4(m4 m){
    m4 result = transpose_m4(cofactor_m4(m));
    return(result);
}

static m2
inverse_m2(m2 m){
    m2 result = ZERO_INIT;
    f32 det = determinant_m2(m);
    if(det == 0.0f){
        return(result);
    }
    result = adjugate_m2(m) * (1.0f / det);
    return(result);
}

static m3
inverse_m3(m3 m){
    m3 result = ZERO_INIT;
    f32 det = determinant_m3(m);
    if(det == 0.0f){
        return(result);
    }
    result = adjugate_m3(m) * (1.0f / det);
    return(result);
}

static m4
inverse_m4(m4 m){
    m4 result = ZERO_INIT;
    f32 det = determinant_m4(m);
    if(det == 0.0f){
        return(result);
    }
    result = adjugate_m4(m) * (1.0f / det);
    return(result);
}

///////////////////////////////
// NOTE: Matrix Transformations
///////////////////////////////

static m4
translate_m4(v3 pos){
    m4 result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        pos.x, pos.y, pos.z, 1.0f,
    };
    return(result);
}

static v3
get_translation_m4(m4 m){
    v3 result = {m._41, m._42, m._43};
    return(result);
}

static m4
scale_m4(v3 pos){
    m4 result = {
        pos.x, 0.0f, 0.0f, 0.0f,
        0.0f, pos.y, 0.0f, 0.0f,
        0.0f, 0.0f, pos.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    return(result);
}

static v3
get_scaled_m4(m4 m){
    v3 result = {m._11, m._22, m._33};
    return(result);
}

static m3
rotate_x_m3(f32 degree){
    f32 rad = deg_to_rad(degree);
    m3 result = {
        1.0f,  0.0f,         0.0f,
        0.0f,  cos_f32(rad), sin_f32(rad),
        0.0f, -sin_f32(rad), cos_f32(rad),
    };
    return(result);
}

static m4
rotate_x_m4(f32 degree){
    f32 rad = deg_to_rad(degree);
    m4 result = {
        1.0f,  0.0f,         0.0f,         0.0f,
        0.0f,  cos_f32(rad), sin_f32(rad), 0.0f,
        0.0f, -sin_f32(rad), cos_f32(rad), 0.0f,
        0.0f,  0.0f,         0.0f,         1.0f,
    };
    return(result);
}

static m3
rotate_y_m3(f32 degree){
    f32 rad = deg_to_rad(degree);
    m3 result = {
        cos_f32(rad), 0.0f, -sin_f32(rad),
        0.0f,         1.0f,  0.0f,
        sin_f32(rad), 0.0f,  cos_f32(rad),
    };
    return(result);
}

static m4 rotate_z_m4(f32 degree){
    f32 rad = deg_to_rad(degree);
    m4 result = {
        cos_f32(rad), sin_f32(rad), 0.0f, 0.0f,
       -sin_f32(rad), cos_f32(rad), 0.0f, 0.0f,
        0.0f,         0.0f,         1.0f, 0.0f,
        0.0f,         0.0f,         0.0f, 1.0f,
    };
    return(result);
}

static m3
rotate_z_m3(f32 degree){
    f32 rad = deg_to_rad(degree);
    m3 result = {
        cos_f32(rad), sin_f32(rad), 0.0f,
       -sin_f32(rad), cos_f32(rad), 0.0f,
        0.0f,         0.0f,         1.0f
    };
    return(result);
}

static m4
rotate_y_m4(f32 degree){
    f32 rad = deg_to_rad(degree);
    m4 result = {
        cos_f32(rad), 0.0f, -sin_f32(rad), 0.0f,
        0.0f,         1.0f,  0.0f,         0.0f,
        sin_f32(rad), 0.0f,  cos_f32(rad), 0.0f,
        0.0f,         0.0f,  0.0f,         1.0f,
    };
    return(result);
}

static m4 rotate_euler_m4(f32 pitch, f32 yaw, f32 roll){
    m4 result = rotate_z_m4(roll) *
                rotate_x_m4(pitch) *
                rotate_y_m4(yaw);
    return(result);
}

static m3
rotate_euler_m3(f32 pitch, f32 yaw, f32 roll){
    m3 result = rotate_z_m3(roll) *
                rotate_x_m3(pitch) *
                rotate_y_m3(yaw);
    return(result);
}

static m4
rotate_axis_m4(v3 axis, f32 deg){
    f32 rad = deg_to_rad(deg);
    f32 c = cos_f32(rad);
    f32 s = sin_f32(rad);
    f32 t = 1 - cos_f32(rad);

    if(magnitude_v3(axis) > 1.0f){
        axis = normalized_v3(axis);
    }

    m4 result = {
        t * (axis.x * axis.x) + c, t * axis.x * axis.y + s * axis.z, t * axis.x * axis.z - s * axis.y, 0.0f,
        t * axis.x * axis.y - s * axis.z, t * (axis.y * axis.y) + c, t * axis.y * axis.z + s * axis.x, 0.0f,
        t * axis.x * axis.z + s * axis.y, t * axis.y * axis.z - s * axis.x, t * (axis.z * axis.z) + c, 0.0f,
        0.0f,                             0.0f,                             0.0f,                      1.0f,
    };
    return(result);
}

static m3 rotate_axis_m3(v3 axis, f32 deg){
    f32 rad = deg_to_rad(deg);
    f32 c = cos_f32(rad);
    f32 s = sin_f32(rad);
    f32 t = 1 - cos_f32(rad);

    if(magnitude_v3(axis) > 1.0f){
        axis = normalized_v3(axis);
    }

    m3 result = {
        t * (axis.x * axis.x) + c, t * axis.x * axis.y + s * axis.z, t * axis.x * axis.z - s * axis.y,
        t * axis.x * axis.y - s * axis.z, t * (axis.y * axis.y) + c, t * axis.y * axis.z + s * axis.x,
        t * axis.x * axis.z + s * axis.y, t * axis.y * axis.z - s * axis.x, t * (axis.z * axis.z) + c,
    };
    return(result);
}

static v3
multiply_m4_point(v3 point, m4 m){
    v3 result = ZERO_INIT;
    result.x = point.x * m._11 + point.y * m._21 +
               point.z * m._31 + 1.0f * m._41;
    result.y = point.x * m._12 + point.y * m._22 +
               point.z * m._32 + 1.0f * m._42;
    result.z = point.x * m._13 + point.y * m._23 +
               point.z * m._33 + 1.0f * m._43;
    return(result);
}

static v3
multiply_m4_v(v3 v, m4 m){
    v3 result = ZERO_INIT;
    result.x = v.x * m._11 + v.y * m._21 + v.z * m._31 + 0.0f * m._41;
    result.y = v.x * m._12 + v.y * m._22 + v.z * m._32 + 0.0f * m._42;
    result.z = v.x * m._13 + v.y * m._23 + v.z * m._33 + 0.0f * m._43;
    return(result);
}

static v3
multiply_m3_v(v3 v, m3 m){
    v3 result = ZERO_INIT;
    result.x = inner_product_v3(v, (v3){m._11, m._21, m._31});
    result.y = inner_product_v3(v, (v3){m._12, m._22, m._32});
    result.z = inner_product_v3(v, (v3){m._13, m._23, m._33});
    return(result);
}

static m4
transform_euler(v3 scale, v3 rotation, v3 translation){
    m4 result = scale_m4(scale) *
                rotate_euler_m4(rotation.x,
                                rotation.y,
                                rotation.z) *
                translate_m4(translation);
    return(result);
}

static m4
transform(v3 scale, v3 rotation, f32 deg, v3 translation){
    m4 result = scale_m4(scale) *
                rotate_axis_m4(rotation, deg) *
                translate_m4(translation);
    return(result);
}

static m4
view_matrix(v3 position, v3 target, v3 up){
    v3 forward = normalized_v3(target - position);
    v3 right = normalized_v3(cross_product_v3(up, forward));
    v3 new_up = cross_product_v3(forward, right);

    m4 result = {
        right.x, new_up.x, forward.x, 0.0f,
        right.y, new_up.y, forward.y, 0.0f,
        right.z, new_up.z, forward.z, 0.0f,
        -inner_product_v3(right, position),
        -inner_product_v3(new_up, position),
        -inner_product_v3(forward, position), 1.0f
    };
    return(result);
}

static m4
projection(f32 fov, f32 aspect, f32 near, f32 far){
    f32 tan_half_fov = tan_f32(deg_to_rad((fov * 0.5f)));
    f32 fov_y = 1.0f / tan_half_fov;
    f32 fov_x = fov_y / aspect;

    m4 result = ZERO_INIT;
    result._11 = fov_x;
    result._22 = fov_y;
    result._33 = far / (far - near);
    result._34 = 1.0f;
    result._43 = -near * result._33;
    result._44 = 0.0f;
    return(result);
}

static m4
orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far){
    f32 _11 = 2.0f / (right - left);
    f32 _22 = 2.0f / (top - bottom);
    f32 _33 = 1.0f / (far - near);
    f32 _41 = (left + right) / (left - right);
    f32 _42 = (top + bottom) / (bottom - top);
    f32 _43 = near / (near - far);

    m4 result = {
        _11,  0.0f, 0.0f, 0.0f,
        0.0f, _22,  0.0f, 0.0f,
        0.0f, 0.0f, _33,  0.0f,
        _41,  _42,  _43,  1.0f,
    };
    return(result);
}

#endif
