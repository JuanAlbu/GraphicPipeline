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
![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/pipeline.png) 

<br>

### 1. Carregamento do modelo

Foi disponibilizado um código de carregador de malhas 3D pelo professor, onde foi utilizado para fazer o carregamento de modelo do projeto em questão. Também foi disponibilizado o <b>obj file</b> de Suzanne, a macaca do blender, onde usamos como parâmetro de comparação.

![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/monkey.jpg)

Com isso, pudemos salvar e manusear os dados contidos nesse obj file, para usar em uma posterior rasterização. Para salvar, manusear e fazer operações em matrizes de forma mais simples, foi utilizada a biblioteca <b>GLM</b>, onde também fizemos produtos vetoriais e cálculo da norma usando as funções <b> cross() </b> e <b> l1Norm() </b>.

Uma dificuldade encontrada foi fazer as operações com matrizes, e as de produto vetorial e norma. Foi visto o código do pipeline em matlab disponibilizado em sala e tentamos convertê-lo diretamente. Depois de dificuldades, demos uma chance a biblioteca GLM e foi visto que era possível fazer tudo o que precisamos (de operações em matrizes) por lá, de maneira bem mais simples.

### 2. Transformação: Espaço do Objeto → Espaço do Universo


![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/space_world.png)


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

![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/universo_camera.png)

Em seguida da criação da matriz model, eleboramos a matriz view, sendo ela é responsavel por levar o objeto do espaço do objeto para o espaço da câmera. Isso foi feito através da combinação de duas matrizes, uma matriz de rotação e outra de translação, sendo a Matriz B que serve para representar os pontos no Espaço da Câmera e uma Matriz T usada para transladar todos os vértices com objetivo de posicionar a câmera na origem. Esta translação e esta rotação são definidas a partir das informações da câmera que são: posição, direção e up.


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
    double d = 2f;

    //Definição da matriz de projeção.
    mat4 M_Projecao = mat4( vec4(1, 0, 0, 0),
                            vec4(0, 1, 0, 0),
                            vec4(0, 0, 1, d),
                            vec4(0, 0, -1/d, 0));


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
    

    //Matriz pra redimensionar a escala do ojeto no tamanho da tela.
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
    for(int i = 0; i < v_objeto.size(); i++) {
        v_objeto[i] = round(M_ViewPort * v_objeto[i]);
    }
```

### 6. Rasterização

Para ocorrer a rasterização, foi utilizado o mesmo algoritmo na primeira atividade, onde são criados objetos Vertices, que contém sua coordenada x, y e o valor RGBA como atributos. Carregamos as coordenadas que estão em v_objeto, pegando de três em três vértices a formar um triângulo. No fim, temos o modelo do macaco rasterizado.

```c
for(int i = 0; i < v_objeto.size(); i+=3) {
     Pixel Vertice_1(v_objeto[i][0], v_objeto[i][1], 255, 0, 0, 255);
     Pixel Vertice_2(v_objeto[i+1][0], v_objeto[i+1][1], 0, 255, 0, 255);
     Pixel Vertice_3(v_objeto[i+2][0], v_objeto[i+2][1], 0, 0, 255, 255);

     DrawTriangle(Vertice_1, Vertice_2, Vertice_3);
}

```

 ![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/rasterizaçao.png)

Uma dificuldade encontrada foi que, ao fazer a rotação, a imagem gerada ficava dessa maneira:

 ![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/dificuldade.png)

Ou seja, após a rasterização de um novo frame, o frame antigo ainda continuava aparecendo na tela. O modo de resolver isso foi limpando o color buffer.

### 7.Comparações

 Com o objeto já rasterizado e o pipeline gráfico implementado, é hora de compararmos com uma aplicação feita em OpenGL.
 
 A imagem abaixo é a original, com o pipeline gráfico em OpenGL:
 
 ![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/opengl.png)

E a imagem abaixo é a resultante da nossa atividade:

 ![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/original.png)



 ![alt text](https://github.com/JuanAlbu/GraphicPipeline/blob/master/prints/monkey.gif)


 ### 8.Referências
    
  https://glm.g-truc.net/0.9.9/index.html
