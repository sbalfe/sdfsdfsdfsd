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

#include "full_camera.h"

FullCamera::FullCamera()
{
//BEGIN_STAGE_ONE
//END_STAGE_ONE
}

FullCamera::FullCamera(float f, const Vertex& p_position, const Vector& p_lookat, const Vector& p_up): fov{f}, position {p_position}
{
    setCameraMatrix(p_position, p_lookat, p_up);
}

void FullCamera::setCameraMatrix(Vertex eye, Vector lookAt, Vector up){
    w =  (eye - lookAt) * (1 / (eye - lookAt).length());
    up.cross(w).normalise();
    u = up;
    u.cross(w,v);
}

void FullCamera::get_ray_offset(int p_x, int p_y, float p_ox, float p_oy, Ray& p_ray)
{
//BEGIN_STAGE_ONE
// END_STAGE_ONE
}

void FullCamera::get_ray_pixel(int p_x, int p_y, Ray &ray)
{
//BEGIN_STAGE_ONE
    float y_v = (((float)p_y + 0.5f) / (float)width);
    float x_v = (((float)p_x + 0.5f)  / (float)height);

    y_v = 0.5f - y_v;
    x_v = x_v - 0.5f;

    ray.direction = (x_v * u) + (y_v * v) - (fov * w);
    ray.direction.normalise();

    ray.position.x = position.x;
    ray.position.y = position.y;
    ray.position.z = position.z;
    ray.position.w = 1;


//END_STAGE_ONE
}

float scale(float scale, float val){ return val * scale;}

void FullCamera::render(Environment& env, FrameBuffer& fb)
{
    width = fb.width;
    height = fb.height;

    for (int y = 0; y < height; y += 1)
    {
        for (int x = 0; x < width; x += 1)
        {

            Ray ray;
            get_ray_pixel(x, y, ray);
            Colour colour;
            float depth;

            env.raytrace(ray, 5, colour, depth);
            fb.plotPixel(x, y, colour.r, colour.g, colour.b);
            fb.plotDepth(x, y, depth);

        }
       std::cerr << "#" << flush;
    }

    std::cout << "test" << std::endl;
}
