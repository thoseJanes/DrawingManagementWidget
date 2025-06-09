#version 330 core
layout (lines) in;
layout (line_strip, max_vertices = 2) out; // 输出为裁剪后的线段

in vec3 originalPosition[];

uniform mat4 mvp;//变换矩阵
uniform vec3 minBounds; // 裁剪区域最小边界
uniform vec3 maxBounds; // 裁剪区域最大边界

vec3 calIntersection(vec3 p1, vec3 p2, bool p2InsideArray[6]) {
    vec3 intersection;
    bool intersectionInside;
    for (int dir = 0; dir < 3; dir++) {
        //注意，side的两边是冲突的，如果一边超出了范围，另一边就不可能超出范围。
        for (int side = 0; side < 2; side++) {
            if (!p2InsideArray[side + dir * 2]) {
                if (side == 0) {
                    intersection[dir] = minBounds[dir];
                }
                else if (side == 1) {
                    intersection[dir] = maxBounds[dir];
                }
                intersectionInside = true;
                for (int dir2 = 0; dir2 < 3; dir2++) {
                    if (dir2 == dir) { continue; }
                    intersection[dir2] = (intersection[dir] - p1[dir]) / (p2[dir] - p1[dir]) * (p2[dir2] - p1[dir2]) + p1[dir2];
                    intersectionInside = intersectionInside && intersection[dir2] >= minBounds[dir2] && intersection[dir2] <= maxBounds[dir2];
                }
                if (intersectionInside) {

                    return intersection;
                }
            }
        }
    }
    return p1;
}


void main() {
    bool p1InsideArray[6];
    bool p2InsideArray[6];
    bool p1Inside;
    bool p2Inside;
    vec3 p1;
    vec3 p2;

    p1 = originalPosition[0]; // 当前点
    p1InsideArray[0] = p1.x >= minBounds.x;
    p1InsideArray[1] = p1.x <= maxBounds.x;
    p1InsideArray[2] = p1.y >= minBounds.y;
    p1InsideArray[3] = p1.y <= maxBounds.y;
    p1InsideArray[4] = p1.z >= minBounds.z;
    p1InsideArray[5] = p1.z <= maxBounds.z;
    p1Inside = p1InsideArray[0] && p1InsideArray[1] &&
        p1InsideArray[2] && p1InsideArray[3] &&
        p1InsideArray[4] && p1InsideArray[5];

    p2 = originalPosition[1]; // 下一个点
    p2InsideArray[0] = p2.x >= minBounds.x;
    p2InsideArray[1] = p2.x <= maxBounds.x;
    p2InsideArray[2] = p2.y >= minBounds.y;
    p2InsideArray[3] = p2.y <= maxBounds.y;
    p2InsideArray[4] = p2.z >= minBounds.z;
    p2InsideArray[5] = p2.z <= maxBounds.z;
    p2Inside = p2InsideArray[0] && p2InsideArray[1] &&
        p2InsideArray[2] && p2InsideArray[3] &&
        p2InsideArray[4] && p2InsideArray[5];

    if (p1Inside && p2Inside) {
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        gl_Position = gl_in[1].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
    else if (p1Inside && !p2Inside) {
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        vec3 intersection = calIntersection(p1, p2, p2InsideArray);
        gl_Position = mvp * vec4(intersection, 1.0);
        EmitVertex();
        EndPrimitive();
    }
    else if (p2Inside && !p1Inside) {
        vec3 intersection = calIntersection(p2, p1, p1InsideArray);//该函数默认第二个位置是在外面的点。
        gl_Position = mvp * vec4(intersection, 1.0);
        EmitVertex();
        gl_Position = gl_in[1].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
}

