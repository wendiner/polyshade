// polyshade.h
// by wendiner, 2026
// GNU General Public License v3.0
//
// 3D rasterizer with support for wireframe
// projection, flat shading, and UV mapping.

#ifndef _POLYSHADE_H_
#define _POLYSHADE_H_

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

struct xyz { // for convenience
  double x, y, z;
};

struct plane { // three vertices (points) define a plane (triangle)
  struct xyz vert1, vert2, vert3;
};

struct model { // represents a geometric model
  struct xyz pos, rot; // position and rotation of model
  double scale; // scale for all of model's points
  unsigned int numPlanes; // number of planes (triangles) in model
  struct plane* mesh; // triangle mesh
};

struct scene { // represents a scene to be rendered
  struct xyz camPos, camRot, dispOff; // camera position & rotation, and display surface offset
  struct ALLEGRO_COLOR bg_color; // background color
  unsigned int numModels; // number of geometric models
  struct model* models; // array of models
};

// bit flags
#define WIREFRAME 0x00000001

struct drawScene_opts { // options for the drawScene function
  unsigned int flags; // 32-bit packed field
};

extern void project(double* result, const struct xyz* a, const struct xyz* c, const struct xyz* t, const struct xyz* e);
// projects a 3D vertex into 2D space
extern void rotate(const struct xyz* inp, struct xyz* out, double rx, double ry, double rz);
// rotates a 3D vertex along the origin's axes
extern struct model* loadModel(char* filename);
// loads a model from a special .bin file, which itself was created from an .obj file by the "converter" program
extern void drawScene(const struct scene* scene, struct drawScene_opts* opts, ALLEGRO_BITMAP* target);
// renders a scene and all of its models

#endif
