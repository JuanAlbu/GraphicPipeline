#include "main.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objLoader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

using namespace std;
using namespace glm;
//---------------------------------------------------------------------------------------------
// Iniciação da variavel de rotação
float rotacao = 0.0;

// iniciação dao vetor de vertices do objeto
objLoader *objData;

//---------------------------------------------------------------------------------------------
//Carregamento do modelo

void MyGlDraw(void)
{

	objData = new objLoader();
	objData->load("monkey_head2.obj");	
    vector<vec4> vec_array;


	for(int f = 0; f < objData->faceCount; f++) {
		obj_face* obj = objData->faceList[f];
		for(int i = 0; i < 3; i++) {
			vec_array.push_back(vec4(objData->vertexList[obj->vertex_index[i]]->e[0], objData->vertexList[obj->vertex_index[i]]->e[1], objData->vertexList[obj->vertex_index[i]]->e[2], 1.0));

		}
		
	}

    //Passa os vertices do objeto carregado para um vetor
	vector<vec4> v_objeto = vec_array;
    
    //---------------------------------------------------------------------------------------------
    // 1) Espaço do Objeto → Espaço do Universo 
    
    //Definição de matriz indentidade
    mat4 M_Indentidade = mat4(  vec4(1, 0, 0, 0),
                                vec4(0, 1, 0, 0),
                                vec4(0, 0, 1, 0),
                                vec4(0, 0, 0, 1));

    //Definição de matriz Rotação, que rotaciona em ralação ao valor da variavel rotacao
    mat4 M_Rotacao = mat4(  vec4(cos(rotacao), 0, -sin(rotacao), 0),
                            vec4(0, 1, 0, 0),
                            vec4(sin(rotacao), 0, cos(rotacao), 0),
                            vec4(0, 0, 0, 1));
    
    //incremento da variavel de rotação, rotacionando o objeto em +0,02 graus
    rotacao = rotacao + 0.02;

    //Multiplicação resultante na matriz View
    mat4 M_Model = M_Indentidade * M_Rotacao ;

    //---------------------------------------------------------------------------------------------
    // 2) Espaço do Universo → Espaço da Câmera
    
    //Definição de parametros da camera
 	vec3 camera_pos    =   vec3(0, 0, 5);  
    vec3 camera_lookat =   vec3(0, 0, 0);  
    vec3 camera_up     =   vec3(0, 1, 0);  

    //Definição dos eixos da camera

    vec3 camera_dir = camera_lookat - camera_pos;


 	vec3 camera_z = -(camera_dir) / l1Norm(camera_dir);
    vec3 camera_x = cross(camera_up, camera_z) / l1Norm(cross(camera_up, camera_z));
    vec3 camera_y = cross(camera_z, camera_x);

    //Definiação das matrizes que compoem a matriz View
  	mat4 B = mat4(vec4(camera_x, 0),
                  vec4(camera_y, 0),
                  vec4(camera_z, 0),
                  vec4(0, 0, 0, 1));

    mat4 T = mat4(vec4(1, 0, 0, -camera_pos.x),
                  vec4(0, 1, 0, -camera_pos.y),
                  vec4(0, 0, 1, -camera_pos.z),
                  vec4(0, 0, 0, 1));

    //Multiplicação resultante na matriz View
    mat4 M_View = transpose(B) * T;

    //---------------------------------------------------------------------------------------------
    // 3) Espaço da Câmera → Espaço Projetivo ou de Recorte
    
    //Distancia entre a camera e o view plane
    double d = -0.35f;

    //Definiação ds matriz de Projeção
    mat4 M_Projecao = mat4( vec4(1, 0, 0, 0),
                            vec4(0, 1, 0, 0),
                            vec4(0, 0, 1, -1/d),
                            vec4(0, 0, d, 0));


    mat4 M_MVP = M_Model * M_View * M_Projecao;

    //---------------------------------------------------------------------------------------------
    // 4) Espaço de Recorte → Espaço Canônico 

    //Dividindo as coordenadas dos vértices no espaço de recorte pela sua coordenada homogênea.
    for(int i = 0; i < v_objeto.size(); i++)
    {
        v_objeto[i] = v_objeto[i] * M_MVP;
        v_objeto[i] = v_objeto[i] / v_objeto[i].w;
    }

    //---------------------------------------------------------------------------------------------
    // 5) Espaço Canônico → Espaço de Tela 

    int w = 512;
    int h = 512;

    mat4 S1 = mat4(vec4(1, 0, 0, 0),
                   vec4(0,-1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(0, 0, 0, 1));
    

    //Matriz pra redimencionar a escala do ojeto no tamanho da tela
    mat4 T1 = mat4(vec4(w/2, 0, 0, 0),
                   vec4(0, h/2, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(1, 1, 0, 1));
    

    mat4 S2 = mat4(vec4(1, 0, 0, 0),
                   vec4(0, 1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4((w-1)/2, (h-1)/2, 0, 1));
    
    mat4 M_ViewPort = S2 * T1 * S1;

    for(unsigned int i = 0; i < v_objeto.size(); i++)
    {
        v_objeto[i] = round(M_ViewPort * v_objeto[i]);
    }

    //Limpando 
    memset(FBptr, 0, w*h*4);

    //---------------------------------------------------------------------------------------------
    //Rasterização feita na T1

	for(unsigned int i = 0; i < v_objeto.size(); i+=3)
    {

		Pixel Vertice_1(v_objeto[i][0], v_objeto[i][1], 255, 0, 0, 255);
		Pixel Vertice_2(v_objeto[i+1][0], v_objeto[i+1][1], 0, 255, 0, 255);
		Pixel Vertice_3(v_objeto[i+2][0], v_objeto[i+2][1], 0, 0, 255, 255);

		DrawTriangle(Vertice_1, Vertice_2, Vertice_3);
    }

} 
//---------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	DrawFunc = MyGlDraw;	

	glutMainLoop();

	return 0;
}