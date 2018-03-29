#include "models.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE 64
// Lines shouldn't be long in OBJ
struct model* load_obj(const char* name) {
    struct model* output = calloc(1, sizeof(struct model));

    if (access(name, F_OK|R_OK) == -1) {
        fprintf(stderr, "File doesn't exist\n");
        return output;
    }
    FILE* fp = fopen(name, "r");	// Open a file for reading
    if (fp == NULL) {
        fprintf(stderr, "File couldn't be opened\n");
        return output;
    }
    printf("Loading file %s...\n", name);
    int numverts = 0;
    int numfaces = 0;
    char f[MAX_LINE];
	while (fgets(f, MAX_LINE, fp) != NULL) {
		if (strncmp(f, "v", 1)==0)
            numverts++;
        else if (strncmp(f, "f", 1)==0)
            numfaces++;
	}
    printf("%d verts; %d faces\n", numverts, numfaces);
    output->vertices = calloc(numverts, sizeof(Vec));
    output->normals = calloc(numverts, sizeof(Vec));
    output->vertexIndex = calloc(numfaces * 3, sizeof(int));

    rewind(fp);
    double x = 0;
    double y = 0;
    double z = 0;

    int i=0;
    int j=0;

    int k=0;
    int l=0;

    int m=0;
    int n=0;

    int vert = 0;
    int norm = 0;
    int face = 0;
    while (fgets(f, MAX_LINE, fp) != NULL) {
        if (strncmp(f, "vn", 2)==0) {
            sscanf(f, "vn %lf %lf %lf", &x, &y, &z);
            output->normals[norm++] = (Vec) {x, y, z};
        }
		else if (strncmp(f, "v", 1)==0) {
            sscanf(f, "v %lf %lf %lf", &x, &y, &z);
            output->vertices[vert++] = (Vec) {x, y, z};
        }
        else if (strncmp(f, "f", 1)==0) {
            sscanf(f, "f %d//%d %d//%d %d//%i", &i,&j, &k,&l, &m,&n);
            output->vertexIndex[face++] = i;
            output->vertexIndex[face++] = j;

            output->vertexIndex[face++] = k;
            output->vertexIndex[face++] = l;

            output->vertexIndex[face++] = m;
            output->vertexIndex[face++] = n;
        }
	}
	fclose(fp);
    printf("Finished loading model...\n");
    output->nverts = numverts;
    output->nfaces = numfaces;
    return output;
}