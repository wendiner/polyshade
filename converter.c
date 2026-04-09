// converter.c
// by wendiner, 2026
// GNU General Public License v3.0
//
// Converts .obj files to an array of line structs,
// readable by wireframe.c.
// By nature, only supports "v" and "f" entries.

#include <stdio.h>
#include <stdlib.h>
#include "polyshade.h"

struct vertex {
  struct xyz pos;
  struct vertex* next;
};

struct vertex* seek(struct vertex* start, unsigned int ind) {
  struct vertex* ptr = start;
  for (unsigned int i = 0; i < ind; i++)
    ptr = ptr->next;

  return ptr;
}

struct plane_ll {
  struct plane actual;
  struct plane_ll* next;
};

int main(int argc, char** argv) {
  FILE* fp_inp = fopen(argv[1], "r");
  FILE* fp_out = fopen(argv[2], "w+");

  struct vertex* start = NULL;
  struct vertex* old;
  struct vertex* new;
  struct plane_ll *tri_start, *tri_old, *tri_new;
  unsigned int numPlanes;
  
  char prefix;
  double num1, num2, num3;

  while (1) {
    fscanf(fp_inp, "%c %lf %lf %lf\n", &prefix, &num1, &num2, &num3);

    switch (prefix) {
      case 'v':
        if (!start) {
          start = (struct vertex*) malloc(sizeof(struct vertex));
          start->pos.x = num1;
          start->pos.y = num2;
          start->pos.z = num3;
          new = start;
        } else {
          old = new;
          new = (struct vertex*) malloc(sizeof(struct vertex));
          old->next = new;
          new->pos.x = num1;
          new->pos.y = num2;
          new->pos.z = num3;
        }

        break;
      case 'f':
        struct vertex* vert1 = seek(start, (unsigned int) (num1 - 1));
        struct vertex* vert2 = seek(start, (unsigned int) (num2 - 1));
        struct vertex* vert3 = seek(start, (unsigned int) (num3 - 1));

        struct plane_ll* current = (struct plane_ll*) malloc(sizeof(struct plane_ll));

        if (tri_new) {
          tri_old = tri_new;
          tri_old->next = current;
        } else {
          tri_start = current;
        }

        current->actual.vert1 = vert1->pos;
        current->actual.vert2 = vert2->pos;
        current->actual.vert3 = vert3->pos;

        tri_new = current;
        numPlanes++;
    }

    if (feof(fp_inp))
      break;
  }

  fprintf(fp_out, "MODEL");
  fwrite(&numPlanes, sizeof(unsigned int), 1, fp_out);
  struct plane_ll* ptr = tri_start;
  while (ptr) {
    fwrite(&(ptr->actual), sizeof(struct plane), 1, fp_out);
    ptr = ptr->next;
  }

  return 0;
}
