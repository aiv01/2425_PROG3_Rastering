#pragma once

struct Vector2i {
    int x;
    int y;
};

struct Vector2f {
    float x;
    float y;
};

struct Vector3f {
    float x;
    float y;
    float z;

    Vector3f operator-(const Vector3f& other) const;
};

/// @brief Get the min value among the 3 values.
int min3(int a, int b, int c);

/// @brief Get the max value among the 3 values.
int max3(int a, int b, int c);

int det(Vector2i p, Vector2i v1, Vector2i v2);