# Pipeline gráfico

### Implementando um algoritmo de pipeline gráfico
##### DUPLA: SUANNY FABYNE E JUAN ALBUQUERQUE

<br><br>



### Uma breve introdução
 
  <p>A atividade foi construida para ocorrer uma familiarização com a estrutura e o funcionamento do
pipeline gráfico através da implementação de um pipeline completo, capaz de transformar vértices
descritos no espaço do objeto em primitivas rasterizadas no espaço de tela.</p>
  
Em resumo, as etapas normalmente encontradas ao longo de um pipeline gráfico são:

<ol>
  <li>Transformação: Espaço do Objeto → Espaço do Universo</li>
  <li>Transformação: Espaço do Universo → Espaço da Câmera</li>
  <li>Transformação: Espaço da Câmera → Espaço Projetivo ou de Recorte</li>
  <li>Transformação: Espaço de Recorte → Espaço “Canônico”</li>
  <li>Transformação: Espaço Canônico → Espaço de Tela</li>
  <li>Rasterização</b></li>
  
</ol>

<br>

### 1. Carregamento do modelo

Foi disponibilizado um código de carregador de malhas 3D pelo professor, onde foi utilizado para fazer o carregamento de modelo do projeto em questão. Também foi disponibilizado o <b>obj file</b> de Suzanne, a macaca do blender, onde usamos como parâmetro de comparação.

![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/monkey.jpg)

Com isso, pudemos salvar e manusear os dados contidos nesse obj file, para usar em uma posterior rasterização. Para salvar, manusear e fazer operações em matrizes de forma mais simples, foi utilizada a biblioteca <b>GLM</b>, onde também fizemos produtos vetoriais e cálculo da norma usando as funções <b> cross() </b> e <b> l1Norm() </b>.

### 2. Transformação: Espaço do Objeto → Espaço do Universo

Após o carregamento do objeto em um vetor denominado v_objeto, começamos a transformação dos vértices do objeto, passando eles do espaço do objeto para o espaço do universo. Para isso, fizemos a matriz Model, que é a resultante de todas as transformações lineares que for necessária para o objeto, neste caso só foi necessária uma multiplicação entre a matriz identidade e uma matriz de rotação. Essa matriz de rotação foi necessária para que o objeto rotacionasse em torno do seu próprio eixo Y continuamente. Isso acontece de acordo com uma variável <b>rotação</b> que definimos e incrementamos a cada frame, e com isso obtemos uma matriz model diferente para cada frame. 

```c

    //Definição de matriz indentidade.
    mat4 M_Indentidade = mat4(  vec4(1, 0, 0, 0),
                                vec4(0, 1, 0, 0),
                                vec4(0, 0, 1, 0),
                                vec4(0, 0, 0, 1));

    //Definição de matriz rotação, que rotaciona em relação ao valor da variável rotação.
    mat4 M_Rotacao = mat4(  vec4(cos(rotacao), 0, -sin(rotacao), 0),
                            vec4(0, 1, 0, 0),
                            vec4(sin(rotacao), 0, cos(rotacao), 0),
                            vec4(0, 0, 0, 1));
    
    //Incremento da variável de rotação, rotacionando o objeto incrementando um valor pequeno, escolhido como 0.02;
    rotacao = rotacao + 0.02;

    //Multiplicação resultante na matriz Model.
    mat4 M_Model = M_Indentidade * M_Rotacao;

```

### 2. Transformação: Espaço do Universo → Espaço da Câmera





```c
    //Definição dos eixos da câmera.
    vec3 camera_dir = camera_lookat - camera_pos;

    vec3 camera_z = -(camera_dir) / l1Norm(camera_dir);
    vec3 camera_x = cross(camera_up, camera_z) / l1Norm(cross(camera_up, camera_z));
    vec3 camera_y = cross(camera_z, camera_x);

    //Definição das matrizes que compõe a matriz View.
  	 mat4 B = mat4(vec4(camera_x, 0),
                  vec4(camera_y, 0),
                  vec4(camera_z, 0),
                  vec4(0, 0, 0, 1));
                  
    mat4 T = mat4(vec4(1, 0, 0, -camera_pos.x),
                  vec4(0, 1, 0, -camera_pos.y),
                  vec4(0, 0, 1, -camera_pos.z),
                  vec4(0, 0, 0, 1));

    //Multiplicação resultante na matriz View.
    mat4 M_View = transpose(B)*T;
```


### 3. Transformação: Espaço da Câmera → Espaço Projetivo ou de Recorte


```c
    //Distancia entre a câmera e o view plane.
    double d = -0.45f;

    //Definição da matriz de projeção.
    mat4 M_Projecao = mat4( vec4(1, 0, 0, 0),
                            vec4(0, 1, 0, 0),
                            vec4(0, 0, 1, -1/d),
                            vec4(0, 0, d, 0));


    mat4 M_MVP = M_Model * M_View * M_Projecao;
    
    // Multiplicação de transformação direta do espaço do objeto para o espaço de recorte
   
    for(int i = 0; i < v_objeto.size(); i++) {
        v_objeto[i] = v_objeto[i] * M_MVP;
    }
```

### 4. Transformação: Espaço de Recorte → Espaço “Canônico”

```c
    //Dividindo as coordenadas dos vértices no espaço de recorte pela sua coordenada homogênea.
    for(int i = 0; i < v_objeto.size(); i++) {
        v_objeto[i] = v_objeto[i] / v_objeto[i].w;
    }
    
```


### 5. Transformação: Espaço Canônico → Espaço de Tela
```c
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
```

```c
    for(unsigned int i = 0; i < v_objeto.size(); i++) {
        v_objeto[i] = round(M_ViewPort * v_objeto[i]);
    }
```

### 6. Rasterização


```c
for(unsigned int i = 0; i < v_objeto.size(); i+=3) {
     Pixel Vertice_1(v_objeto[i][0], v_objeto[i][1], 255, 0, 0, 255);
     Pixel Vertice_2(v_objeto[i+1][0], v_objeto[i+1][1], 0, 255, 0, 255);
     Pixel Vertice_3(v_objeto[i+2][0], v_objeto[i+2][1], 0, 0, 255, 255);

     DrawTriangle(Vertice_1, Vertice_2, Vertice_3);
}

```

### 7.Comparações

 Com o objeto já rasterizado, é hora de compararmos com uma aplicação feita em OpenGL:
 
 
    
    
https://glm.g-truc.net/0.9.9/index.html
