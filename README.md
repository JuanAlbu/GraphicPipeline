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

```c
    // 1) Espaço do Objeto → Espaço do Universo 
    
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

    //Multiplicação resultante na matriz View.
    mat4 M_Model = M_Indentidade * M_Rotacao ;

```











https://glm.g-truc.net/0.9.9/index.html
