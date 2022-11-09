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

// Full camera allows a camera to be placed in space with a lookat and up direction
// as well as the field of view. It loops over the pixels in a framebuffer and computes
// a ray that is then passed to the environment.

#ifndef _FULL_CAMERA_H_
#define _FULL_CAMERA_H_

#include "camera.h"

class FullCamera: public Camera{
public:
  float width;
  float height;
  float fov;

  Vector u;
  Vector w;
  Vector v;

  Vector position;

  static constexpr float scale {0.01};

  FullCamera();

  FullCamera(float p_f, const Vertex& p_position, const Vector& p_lookat, const Vector &p_up);
	
  void get_ray_offset(int p_x, int p_y, float p_ox, float p_oy, Ray &p_ray);

  void get_ray_pixel(int p_x, int p_y, Ray &ray);

  void setCameraMatrix(Vertex eye, Vector lookAt, Vector up);

  void render(Environment& env, FrameBuffer& fb) override;
};

#endif
