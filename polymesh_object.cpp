/****************************************************************************
*
* krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
*
* I've put a lot of time and effort into this code. For the last decade
* it's been used to introduce hundreds of students at multiple universities
* to raytracing. It forms the basis of your coursework but you are free
* to continue using/developing forever more. However, I ask that you don't
* share the code or your derivitive versions publicly. In order to continue
* to be used for coursework and in particular assessment it's important that
* versions containing solutions are not searchable on the web or easy to
* download.
*
* If you want to show off your programming ability, instead of releasing
* the code, consider generating an incredible image and explaining how you
* produced it.
*/

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <regex>
#include <iterator>
#include <cstring>
#include <array>
#include <algorithm>
#include "polymesh_object.h"

using namespace std;

PolyMesh::PolyMesh(const char* file, bool smooth){
    //BEGIN_STAGE_ONE

    std::ifstream modelStream {file};
    modelStream.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    enum class type {
        vertex,
        triangle
    };

    auto setCount = [&](const type& t) -> int {
        std::string line;
        std::getline(modelStream, line);
        std::string newString = std::string(std::find(line.rbegin(), line.rend(), ' ').base(), line.end());
        int count = std::stoi(newString);
        switch(t){
            case type::vertex: {
                vertex_count = count;
                vertex = new Vertex[vertex_count];
                vertexNormal = new Vector[vertex_count];
                break;
            }
            default : {
                triangle_count = count;
                triangle = new TriangleIndex[triangle_count];
                faceNormals = new Vector[triangle_count];
                break;
            }
        }
        return std::stoi(newString);
    };

    auto extractTokenVector = [&](){
        std::string str;
        std::getline(modelStream, str);
        std::istringstream ss {str};
        std::vector<std::string> tokens((std::istream_iterator<std::string>(ss)), std::istream_iterator<std::string>());
        return tokens;
    };

    setCount(type::vertex);
    setCount(type::triangle);

    vertNormIndex.resize(vertex_count);

    for (int i = 0; i < vertex_count; ++i){
        auto tokens = extractTokenVector();
        vertex[i] = Vertex(std::stof(tokens[0]),std::stof(tokens[1]),std::stof(tokens[2]));
    }

    for (int i = 0 ; i < triangle_count; ++i){
        auto tokens = extractTokenVector();
        const int vertCount = std::stoi(tokens[0]);
        for (int v {}; v < vertCount; ++v){
            auto current = std::stoi(tokens[v+1]);
            triangle[i][v] = current;
            vertNormIndex[current].emplace_back(i);
        }
    }


    //END_STAGE_ONE
    next = 0;
}

void PolyMesh::GenFaceNormals(){
    for (int i = 0 ; i < triangle_count; ++i){
        Vertex v1 {vertex[triangle[i][0]]};
        Vector s1 = vertex[triangle[i][1]] - v1;
        Vector s2 = vertex[triangle[i][2]] - v1;
        Vector fnormal;
        s1.cross(s2, fnormal);
        fnormal.normalise();
        faceNormals[i] = fnormal;
    }

}

void PolyMesh::GenVertexNormals(){
    for (int i = 0; i < vertex_count; ++i){
        auto val = vertNormIndex[i];
        Vector accum {0,0,0};
        for (const auto& item: val){
            /* overload +, = properly to optimise this*/
            accum = accum + faceNormals[item];
        }

        float size = val.size();


        accum * (1.0f / size);

        accum.normalise();

        std::cout << accum << std::endl;
        vertexNormal[i] = accum;
    }
}

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
auto Barycentric(Vertex p, Vertex a, Vertex b, Vertex c)
{
    float u,v,w;
    Vector v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);
    float denom = d00 * d11 - d01 * d01;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
    return std::make_tuple(u,v,w);
}

Hit* PolyMesh::intersection(Ray ray)
{

    Hit* hits = nullptr;
    std::list<Hit> hitList;
//BEGIN_STAGE_ONE
    for (int i = 0; i < triangle_count; ++i){

        Vector faceNormal = faceNormals[i];
        Vertex planePoint = vertex[triangle[i][0]];

        /* this calculates the direction of our ray, if this value is > 0 we are */
        float NdotRayDirection = faceNormal.dot(ray.direction);

        if (std::fabs(NdotRayDirection) > 1e-9) {

            /* we compute the distance of our plane equation which is
             *  D = -(Ax+ By + Cz)
             *  where (A,B,C) is our face normal and (x,y,z) is a point on our plane
             * */
            float distance = -faceNormal.dot(planePoint);

            /* plug t P into the plane equation to solve for t
             * P = O + tD
             * A(P_x) + B(P_y) + C(p_z) + D = 0
             *
             * rearranging this eventually gives
             *
             * t = -((normal.dot(rayOrigin) + distance)/normal.dot(rayDirection))
             * */
            float t = -(faceNormal.dot(ray.position) + distance) / NdotRayDirection;
            if (t > 0) {

                /* use t in the original parameter ray equation to figure out our exact hit point of the plane*/
                Vertex hitVec = ray.position + (t * ray.direction);


                /* test if the hit point is inside the triangle using the inside outside test */
                auto test = [&](int vertIdxA, int vertIdxB) -> bool{
                    Vector Temp;
                    Vector edge = vertex[triangle[i][vertIdxA]] - vertex[triangle[i][vertIdxB]];
                    Vector vp = hitVec - vertex[triangle[i][vertIdxB]];
                    edge.cross(vp,Temp);
                    return (faceNormal.dot(Temp) < 0);
                };

                /* inside outside test for each side */
                if (test(1,0)) continue;
                if (test(2,1)) continue;
                if (test(0,2)) continue;

                /* calculate shading normal using barycentric coordinates*/
                Vertex a = vertex[triangle[i][0]];
                Vertex b = vertex[triangle[i][1]];
                Vertex c = vertex[triangle[i][2]];

                Vector na = vertexNormal[triangle[i][0]];
                Vector nb = vertexNormal[triangle[i][1]];
                Vector nc = vertexNormal[triangle[i][2]];

                auto [u,v,w] = Barycentric(hitVec, a,b,c);
                Vector normal = na * u + nb * v + nc * w;
                normal.normalise();

                /* if we take the dot product of the face normal and the ray direction and its positive
                 * this our normal should be flipped around as we are going into the back of a triangle mesh
                 * */
                if (NdotRayDirection > 0.0) {
                    normal.negate();
                }

                /* register an intersection */
                hitList.push_back(Hit{t, true, nullptr, this, hitVec, normal});
            }
        }
    }

    /* sort by closest t value and update the hits linked list to show this */
    hitList.sort();
    for (const auto &hit: hitList) {
        if (hits != nullptr) {
            hits->next = new Hit{hit};
        } else {
            hits = new Hit{hit};
        }
    }
    //END_STAGE_ONE
    return hits;
}


void PolyMesh::apply_transform(Transform& trans)
{
    //BEGIN_STAGE_ONE
    for (int i = 0; i < vertex_count; ++i){
        trans.apply(this->vertex[i]);
    }
    // END_STAGE_ONE
}