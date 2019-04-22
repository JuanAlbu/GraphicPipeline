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

![alt text](https://github.com/JuanAlbu/GraphicPipeline/edit/master/prints/monkey.png)

Com isso, pudemos salvar e manusear os dados contidos nesse obj file, para usar em uma posterior rasterização. Para salvar, manusear e fazer operações em matrizes de forma mais simples, foi utilizada a biblioteca <b>GLM</b>, onde também fizemos produtos vetoriais e cálculo da norma usando as funções <b> cross() </b> e <b> l1Norm() </b>.
 
 
```c
void PutPixel(Pixel paramPixel) {
    FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 0] = paramPixel.getR(); 
    FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 1] = paramPixel.getG(); 
    FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 2] = paramPixel.getB(); 
    FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 3] = paramPixel.getA(); 
}
```

### DrawLine()

A função drawline consiste em rasterizar uma linha atravez de modificação direta da memória de pixel por pixel, onde se tem dois pontos extremos e o Algoritmo de Bresenham diz qual é o pixel certo a ser modificado pare que uma linha mais uniforme possivel seja construída. A tela não possui infinitos pixels para que se desenhe uma linha perfeita, então o Algoritmo de Bresenham toma a decisão para que o desenho construido seja o mais proximo possivel do esperado. A imagem abaixo mostra um exemplo de decisão do algoritmo, onde é decidido pelo pixel mais próximo a linha. 
![alt text](https://3.bp.blogspot.com/-mvcFsWyhnmc/V6pFAr3QdzI/AAAAAAAAAI0/aWCEe16ugu4xkyTJ76QuamhiYUeFDLkzwCLcB/s1600/aula2.png)

Nos foi disponibilizado em aula o algoritmo onde apenas aplica-se ao primeiro octante de um plano, e a maior dificuldade foi poder adaptá-lo a todos os octantes ou condições possíveis. Para isso, precisamos atender a algumas condições

![alt text](https://3.bp.blogspot.com/-Pclf4WPES_Y/V6dTCQR1OGI/AAAAAAAAACM/U_Bwy1Ov0FUGcIDSY4eqi7S-piw-5F_5wCLcB/s640/octantesreal.gif)















https://glm.g-truc.net/0.9.9/index.html
