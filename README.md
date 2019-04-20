# rasterization

### Implementando um algoritmo de rasterização
##### DUPLA: SUANNY FABYNE E JUAN ALBUQUERQUE

<br><br>



### Uma breve introdução
 
  <p>A atividade é baseada na realização de um algoritmo de rasterização de pontos e linhas, sendo necessário triângulos desenhados através de construção de arestas (linhas rasterizadas). As cores atribuidas a cada vértice formam um efeito suave ao longo da linha que conecta os mesmos através da técnica de interpolação, obtendo-se um efeito degradê caso os vértices (ou pontos extremos) tenham cores distintas. A rasterização é feita através da escrita direta na memória, utilizando um framework para simular o acesso à memória de vídeo.</p>
  
Em resumo, há a implementação de 3 funções principais:

<ol>
  <li><b>PutPixel</b>: Rasteriza um ponto na memória de vídeo.</li>
  <li><b>DrawLine</b>: Função que rasteriza uma linha na tela com interpolação linear das cores dos vértices.</li>
  <li><b>DrawTriangle</b>: Desenha as arestas de um triângulo na tela</li>
</ol>

<br>

### PutPixel()

Essa função gera um ponto no espaço cuja sua localidade é determinada pela coordenada (x,y), como também necessita de valores RGBA para determinar a cor e a opacidade deste ponto. Deve-se salientar que o ponto (0,0) fica localizado no canto superior esquerdo da tela. Podemos ver na imagem abaixo um ponto rasterizado na origem com os componentes RGBA(255,0,0,255):
<br><br>

``` 
In: Pixel paramPixel(0,0,255,0,0,255);
    PutPixel(paramPixel);
Out:   
```
![alt text](https://github.com/suannyfabyne/rasterization/blob/master/prints/putpixel.png)


Já o código abaixo mostra a função pronta, onde recebe um objeto da classe Pixel como paramêtro. Nela, contém as coordenadas x e y e os valores R, G, B e A, onde FBptr é um ponteiro que aponta para o primeiro byte do color buffer simulado, onde seu índice representa a posição na tela, e o conteúdo a componente RGBA.


 
 
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
