/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2018.
 *
 * Do what you like with this code as long as you retain this comment.
 */

// Phong is a child class of Material and implement the simple Phong
// surface illumination model.

#pragma once

#include "material.h"

class Phong : public Material {
public:
    Colour ambient_;
    Colour diffuse_;
    Colour specular_;
    float ambientCoefficient_ {1};
    float diffuseCoefficient_ {1};
    float specularCoefficient_ {1};
    float specularIntensity_{40};
    bool check {false};

//BEGIN_STAGE_ONE

	Phong(Colour p_ambient, Colour p_diffuse, Colour p_specular, float p_power);

//END_STAGE_ONE
	Phong()
	{
	}
	Colour compute_once(Ray& viewer, Hit& hit, int recurse);
	Colour compute_per_light(Vector& viewer, Hit& hit, Vector& ldir);

};
