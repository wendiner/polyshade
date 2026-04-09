// polyshade.c
// by wendiner, 2026
// GNU General Public License v3.0
//
// 3D rasterizer with support for wireframe
// projection, flat shading, and UV mapping.

#include <math.h>
#include "polyshade.h"

void project(double* result, const struct xyz* a, const struct xyz* c, const struct xyz* t, const struct xyz* e) {
  // we first perform the "camera transform" to describe the target point relative to the camera
  struct xyz diff = {a->x - c->x, a->y - c->y, a->z - c->z};
  struct xyz cost = {cos(t->x), cos(t->y), cos(t->z)};
  struct xyz sint = {sin(t->x), sin(t->y), sin(t->z)};
  struct xyz d = {
    cost.y * (sint.z * diff.y + cost.z * diff.x) - sint.y * diff.z,
    sint.x * (cost.y * diff.z + sint.y * (sint.z * diff.y + cost.z * diff.x)) + cost.x * (cost.z * diff.y - sint.z * diff.x),
    cost.x * (cost.y * diff.z + sint.y * (sint.z * diff.y + cost.z * diff.x)) - sint.x * (cost.z * diff.y - sint.z * diff.x)
  };

  // then we find the intersection between the target vector and the 2D plane
  result[0] = e->z / d.z * d.x + e->x;
  result[1] = e->z / d.z * d.y + e->y;
}

void rotate(const struct xyz* inp, struct xyz* out, double rx, double ry, double rz) {
  // rotation matrices for each axis
  double rmx[3][3] = {
    {1, 0, 0},
    {0, cos(rx), -sin(rx)},
    {0, sin(rx), cos(rx)}
  };
  double rmy[3][3] = {
    {cos(ry), 0, sin(ry)},
    {0, 1, 0},
    {-sin(ry), 0, cos(ry)}
  };
  double rmz[3][3] = {
    {cos(rz), -sin(rz), 0},
    {sin(rz), cos(rz), 0},
    {0, 0, 1}
  };

  struct xyz old, new;
  
  old.x = inp->x;
  old.y = inp->y;
  old.z = inp->z;

  new.x = old.x * rmx[0][0] + old.y * rmx[0][1] + old.z * rmx[0][2];
  new.y = old.x * rmx[1][0] + old.y * rmx[1][1] + old.z * rmx[1][2];
  new.z = old.x * rmx[2][0] + old.y * rmx[2][1] + old.z * rmx[2][2];

  old.x = new.x;
  old.y = new.y;
  old.z = new.z;
  
  new.x = old.x * rmy[0][0] + old.y * rmy[0][1] + old.z * rmy[0][2];
  new.y = old.x * rmy[1][0] + old.y * rmy[1][1] + old.z * rmy[1][2];
  new.z = old.x * rmy[2][0] + old.y * rmy[2][1] + old.z * rmy[2][2];
  
  old.x = new.x;
  old.y = new.y;
  old.z = new.z;
  
  new.x = old.x * rmz[0][0] + old.y * rmz[0][1] + old.z * rmz[0][2];
  new.y = old.x * rmz[1][0] + old.y * rmz[1][1] + old.z * rmz[1][2];
  new.z = old.x * rmz[2][0] + old.y * rmz[2][1] + old.z * rmz[2][2];

  out->x = new.x;
  out->y = new.y;
  out->z = new.z;
}

struct model* loadModel(char* filename) {
  FILE* fp = fopen(filename, "rb");
  if (!fp) // if file could not be opened, return null
    return NULL;

  for (int i = 0; i < 5; i++) // skip magic number ("MODEL")
    fgetc(fp);

  struct model proto = { // we use a separate struct to make initialization easier
    .pos = {0},
    .rot = {0},
    .scale = 1.0
  };

  fread(&(proto.numPlanes), sizeof(unsigned int), 1, fp); // read the number of planes in file and copy to prototype
  proto.mesh = (struct plane*) malloc(proto.numPlanes * sizeof(struct plane)); // allocate memory for mesh
  fread(proto.mesh, sizeof(struct plane), proto.numPlanes, fp); // read planes from file into mesh

  struct model* final = (struct model*) malloc(sizeof(struct model)); // allocate memory for actual returned model
  final* = proto;
  return final;
}

void drawScene(const struct scene* scene, struct drawScene_opts* opts) {
  al_clear_to_color(scene->bg_color);

}
