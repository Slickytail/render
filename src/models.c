#include "models.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE 64
// Lines shouldn't be long in OBJ
struct model* load_obj(const char* name) {
    struct model* output = malloc(sizeof(struct model));

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
    int numnorms = 0;
    int numfaces = 0;
    int numtex = 0;
    char f[MAX_LINE];
    enum fformat fo = NONE;
	while (fgets(f, MAX_LINE, fp) != NULL) {
        if (strncmp(f, "vn ", 3)==0)
            numnorms++;
		else if (strncmp(f, "v ", 2)==0)
            numverts++;
        else if (strncmp(f, "vt ", 2)==0)
            numtex++;
        else if (strncmp(f, "f ", 2)==0) {
            numfaces++;

            if (fo == NONE) {
                // Parse format
                fo = VERT;
                int slash = 0;
                for (int s=3; f[s+1]!=' '; s++) {
                    if (f[s] == '/' && f[s+1] == '/') {
                        fo = VNORM;
                        break; //= f xyz//pdq
                    }
                    else if (f[s] == '/') {
                        slash++;
                        if (f[s+1] == ' ' && slash) {
                            fo = VTEX;
                            break; //= f xyz/pdq/
                        }
                        else if (slash) {
                            fo = VTEXNORM;
                            break; //= f xyz/pdq/abc
                        }
                    }
                }
            }
        }
	}
    printf("%d verts; %d textcords, %d normals, %d faces\n", numverts, numtex, numnorms, numfaces );
    output->nverts = numverts;
    output->nfaces = numfaces;
    output->ntex = numtex;

    output->vertices = calloc(numverts, sizeof(Vec));
    output->textures = calloc(numtex, sizeof(UV));
    output->normals = calloc(numnorms, sizeof(Vec));

    output->vertexIndex = calloc(numfaces * 3, sizeof(int));
    output->texIndex = calloc(numfaces * 3, sizeof(int));
    output->normIndex = calloc(numfaces * 3, sizeof(int));

    output->f = fo;
    rewind(fp);
    double x = 0;
    double y = 0;
    double z = 0;

    int i=0;
    int j=0;
    int k=0;

    int vert = 0;
    int norm = 0;
    int face = 0;
    int tex = 0;
    while (fgets(f, MAX_LINE, fp) != NULL) {
        if (strncmp(f, "vn ", 3)==0) {
            sscanf(f, "vn %lf %lf %lf", &x, &y, &z);
            output->normals[norm++] = (Vec) {x, y, z};
        }
		else if (strncmp(f, "v ", 2)==0) {
            sscanf(f, "v %lf %lf %lf", &x, &y, &z);
            output->vertices[vert++] = (Vec) {x, y, z};
        }
        else if (strncmp(f, "vt ", 3)==0) {
            sscanf(f, "vt %lf %lf", &x, &y);
            output->textures[tex++] = (UV) {x, y};
        }
        else if (strncmp(f, "f ", 2)==0) {
            
            switch (fo) {
                case VERT:
                    sscanf(f, "f %d %d %d", &i, &j, &k);
                    output->vertexIndex[face] = i-1;
                    output->vertexIndex[face+1] = j-1;
                    output->vertexIndex[face+2] = k-1;
                    break;
                
                case VTEX:
                    sscanf(f, "f %d/%*d/ %d/%*d/ %d/%*d/", &i, &j, &k);
                    output->vertexIndex[face] = i-1;
                    output->vertexIndex[face+1] = j-1;
                    output->vertexIndex[face+2] = k-1;

                    sscanf(f, "f %*d/%d/ %*d/%d/ %*d/%d/", &i, &j, &k);
                    output->texIndex[face] = i-1;
                    output->texIndex[face+1] = j-1;
                    output->texIndex[face+2] = k-1;
                    break;
                
                case VNORM:
                    sscanf(f, "f %d//%*d %d//%*d %d//%*d", &i, &j, &k);
                    output->vertexIndex[face] = i-1;
                    output->vertexIndex[face+1] = j-1;
                    output->vertexIndex[face+2] = k-1;

                    sscanf(f, "f %*d//%d %*d//%d %*d//%d", &i, &j, &k);
                    output->normIndex[face] = i-1;
                    output->normIndex[face+1] = j-1;
                    output->normIndex[face+2] = k-1;
                    break;
                
                case VTEXNORM:
                    sscanf(f, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &i, &j, &k);
                    output->vertexIndex[face] = i-1;
                    output->vertexIndex[face+1] = j-1;
                    output->vertexIndex[face+2] = k-1;

                    sscanf(f, "f %*d/%d/%*d %*d/%d/%*d %*d/%d/%*d", &i, &j, &k);
                    output->texIndex[face] = i-1;
                    output->texIndex[face+1] = j-1;
                    output->texIndex[face+2] = k-1;

                    sscanf(f, "f %*d/%*d/%d %*d/%*d/%d %*d/%*d/%d", &i, &j, &k);
                    output->normIndex[face] = i-1;
                    output->normIndex[face+1] = j-1;
                    output->normIndex[face+2] = k-1;
                    break;

                default:
                    fprintf(stderr, "Unknown polygon format");
            }
           face += 3;
        }
	}
    printf("%d faces, %d face vert indices\n", numfaces, face);
	fclose(fp);
    printf("Finished loading model!\n");
    return output;
}

void free_model(struct model* model) {
    if (model->hasTexture)
        free_image(model->texture);
    free(model->vertices);
    free(model->normals);
    free(model->textures);
    free(model->vertexIndex);
    free(model->texIndex);
    free(model->normIndex);

    free(model);
}