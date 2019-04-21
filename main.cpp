#include "main.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objLoader.h"

using namespace std;
using namespace glm;
//-----------------------------------------------------------------------------

float rotateY = 0.0;
objLoader *objData;

void MyGlDraw(void)
{

	objData = new objLoader();
	objData->load("monkey_head2.obj");	
    std::vector<vec4> vec_array;


	for(int f = 0; f < objData->faceCount; f++) {
		obj_face* obj = objData->faceList[f];
		
		for(int i = 0; i < 3; i++) {
			vec_array.push_back(vec4(objData->vertexList[obj->vertex_index[i]]->e[0], objData->vertexList[obj->vertex_index[i]]->e[1], objData->vertexList[obj->vertex_index[i]]->e[2], 1.0));

		}
		
	}


	std::vector<vec4> model = vec_array;


    mat4 identity_Matrix = mat4(vec4(1, 0, 0, 0),
                                vec4(0, 1, 0, 0),
                                vec4(0, 0, 1, 0),
                                vec4(0, 0, 0, 1));


    mat4 rotate_Matrix = mat4(vec4(cos(rotateY), 0, -sin(rotateY), 0),
                              vec4(0, 1, 0, 0),
                              vec4(sin(rotateY), 0, cos(rotateY), 0),
                              vec4(0, 0, 0, 1));



    rotateY += 0.01;
    mat4 model_Matrix = identity_Matrix * rotate_Matrix ;

 	vec3 camera_pos    =   vec3(0, 0, 5);  
    vec3 camera_target =   vec3(0, 0, 0);  
    vec3 camera_up     =   vec3(0, 1, 0);  


 	vec3 camera_z = -normalize(camera_pos - camera_target);
    vec3 camera_x = normalize(cross(camera_up, camera_z));
    vec3 camera_y = normalize(cross(camera_z, camera_x));

  	mat4 B = mat4(vec4(camera_x, 0),
                  vec4(camera_y, 0),
                  vec4(camera_z, 0),
                  vec4(0, 0, 0, 1));

    mat4 T = mat4(vec4(1, 0, 0, -camera_pos.x),
                  vec4(0, 1, 0, -camera_pos.y),
                  vec4(0, 0, 1, -camera_pos.z),
                  vec4(0, 0, 0, 1));

    mat4 view_Matrix = transpose(B) * T;

    float d = 0.5f;

    mat4 projection_Matrix = mat4(vec4(1, 0, 0, 0),
                                  vec4(0, 1, 0, 0),
                                  vec4(0, 0, 1, -1/d),
                                  vec4(0, 0, d, 0));

    mat4 MVPmatrix = model_Matrix * view_Matrix * projection_Matrix;

    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = model[i] * MVPmatrix;
    }

    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = model[i] / model[i].w;
    }

    int w = 512;
    int h = 512;

    mat4 S1 = mat4(vec4(1, 0, 0, 0),
                   vec4(0,1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(0, 0, 0, 1));
    mat4 T1 = mat4(vec4(w/2, 0, 0, 0),
                   vec4(0, h/2, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(1, 1, 0, 1));
    mat4 S2 = mat4(vec4(1, 0, 0, 0),
                   vec4(0, 1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4((w-1)/2, (h-1)/2, 0, 1));
    mat4 viewport_Matrix = S2 * T1 * S1;

    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = round(viewport_Matrix * model[i]);
    }


    memset(FBptr, 0, w*h*4);


	for(unsigned int i = 0; i < model.size(); i+=3)
    {

		Pixel Vertice_1(model[i][0], model[i][1], 255, 0, 0, 255);
		Pixel Vertice_2(model[i+1][0], model[i+1][1], 0, 255, 0, 255);
		Pixel Vertice_3(model[i+2][0], model[i+2][1], 0, 0, 255, 255);

		DrawTriangle(Vertice_1, Vertice_2, Vertice_3);
    }

} 
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	DrawFunc = MyGlDraw;	

	glutMainLoop();

	return 0;
}

