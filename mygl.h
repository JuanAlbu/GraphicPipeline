#ifndef _MYGL_H_
#define _MYGL_H_
#include "definitions.h"
#include "math.h"
#include <vector>
using namespace std;
//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************
class Pixel {
		int x;
		int y;
		int R;
		int G;
		int B;
		int A;
	public:
		Pixel(int cord_x, int cord_y, int cor_r, int cor_g, int cor_b, int opac_a);

		void setX(int cord_x){
			x = cord_x;
		}
		void setY(int cord_y){
			y = cord_y;
		}
		void setR(int cor_r){
			R = cor_r;
		}
		void setG(int cor_g){
			G = cor_g;
		}
		void setB(int cor_b){
			B = cor_b;
		}
		void setA(int opac_a){
			A = opac_a;
		}
		int getX(){
			return x;
		}
		int getY(){
			return y;
		}
		int getR(){
			return R;
		}
		int getG(){
			return G;
		}
		int getB(){
			return B;
		}
		int getA(){
			return A;
		}
};

Pixel::Pixel(int cord_x, int cord_y, int cor_r, int cor_g, int cor_b, int opac_a){
	x = cord_x;
	y = cord_y;	
	R = cor_r;
	G = cor_g;
	B = cor_b;
	A = opac_a;
}


void PutPixel(Pixel paramPixel)
{
    FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 0] = paramPixel.getR(); // componente R
	FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 1] = paramPixel.getG(); // componente G
	FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 2] = paramPixel.getB(); // componente B
	FBptr[4*paramPixel.getX() + 4*paramPixel.getY()*IMAGE_WIDTH + 3] = paramPixel.getA(); // componente A
}

void Treatment(int &x, int &y, int x_number, int y_number, int &dx, int &dy, int &d, int &incr_e, int &incr_ne){
	if( (abs(dx)>abs(dy)) && (dy > 0) && (dx > 0)){ //1 OCTANTE
		x = x_number;
		y = y_number;
		d = 2 * dy - dx;
		incr_e = 2 * dy;
		incr_ne = 2 * (dy - dx);		
	} else if((abs(dx)>abs(dy)) && (dy < 0) && (dx < 0)){ //5 OCTANTE
		x = x_number;
		y = y_number;
		dy = abs(dy);
		dx = abs(dx);
		d = 2 * dy - dx;
		incr_e = 2 * dy;
		incr_ne = 2 * (dy - dx);	
	} else if((abs(dx)>abs(dy)) && (dy < 0) && (dx > 0)){ //8 OCTANTE
		x = x_number;
		y = y_number;
		dy = abs(dy);
		d = 2 * dy + dx;
		incr_e = 2 * dy;
		incr_ne = 2 * (dy - dx);	
	} else if((abs(dx)>abs(dy)) && (dy > 0) && (dx < 0)){ //4 OCTANTE
		x = x_number;
		y = y_number;
		dx = abs(dx);
		d = 2 * dy + dx;
		incr_e = 2 * dy;
		incr_ne = 2 * (dy - dx);	
	} else if((abs(dy)>abs(dx)) && (dy > 0) && (dx > 0)){ //2 OCTANTE
		x = x_number;
		y = y_number;
		dy = abs(dy);
		dx = abs(dx);
		d = 2 * dx - dy;
		incr_e = 2 * dx;
		incr_ne = 2 * (dx - dy);	
	} else if((abs(dy)>abs(dx)) && (dy < 0) && (dx < 0)){ //6 OCTANTE
		x = x_number;
		y = y_number;
		dy = abs(dy);
		dx = abs(dx);
		d = 2 * dx - dy;
		incr_e = 2 * dx;
		incr_ne = 2 * (dx - dy);	
	} else if((abs(dy)>abs(dx)) && (dy < 0) && (dx > 0)){ //7 OCTANTE
		x = x_number;
		y = y_number;
		dy = abs(dy);
		d = 2 * dx + dy;
		incr_e = 2 * dx;
		incr_ne = 2 * (dx - dy);	
	} else if((abs(dy)>abs(dx)) && (dy > 0) && (dx < 0)){ //3 OCTANTE
		x = x_number;
		y = y_number;
		dx = abs(dx);
		d = 2 * dx + dy;
		incr_e = 2 * dx;
		incr_ne = 2 * (dx - dy);	
	} else if((dy == 0) && (dx != 0)){
		x = x_number;
		y = y_number;
		dx = abs(dx);
	} else if((dx == 0) && (dy != 0)){
		x = x_number;
		y = y_number;
		dy = abs(dy);
	} else if(abs(dx) == abs(dy)){
		x = x_number;
		y = y_number;
		d = 2 * dx + dy;
		incr_e = 2 * dx;
		incr_ne = 2 * (dx - dy);
	}
}

void CalculaCor(float MudarCor[], float DeltaCor[], int dif){

	MudarCor[0] = (DeltaCor[0])/dif;
	MudarCor[1] = (DeltaCor[1])/dif;
	MudarCor[2] = (DeltaCor[2])/dif;
	MudarCor[3] = (DeltaCor[3])/dif;

}


void Interpolacao(Pixel Vertice_1, Pixel Vertice_2, Pixel &paramPixel, int x, int y) {
	float DeltaCor[4];
	float MudarCor[4];
	int pos;
	DeltaCor[0] = Vertice_2.getR() - Vertice_1.getR();
	DeltaCor[1] = Vertice_2.getG() - Vertice_1.getG();
	DeltaCor[2] = Vertice_2.getB() - Vertice_1.getB();
	DeltaCor[3] = Vertice_2.getA() - Vertice_1.getA();

	int dx = Vertice_2.getX() - Vertice_1.getX();
	int dy = Vertice_2.getY() - Vertice_1.getY();

	if(abs(dx) == abs(dy) || abs(dx) > abs(dy)){
		pos = x - Vertice_1.getX();
		CalculaCor(MudarCor,DeltaCor, dx);
	} else if(abs(dx) < abs(dy) || dx == 0){
		pos = y - Vertice_1.getY();
		CalculaCor(MudarCor,DeltaCor, dy);
	}

	paramPixel.setR(Vertice_1.getR() + MudarCor[0]*pos);
	paramPixel.setG(Vertice_1.getG() + MudarCor[1]*pos);
	paramPixel.setB(Vertice_1.getB() + MudarCor[2]*pos);
	paramPixel.setA(Vertice_1.getA() + MudarCor[3]*pos);

}



void DrawLine(Pixel Vertice_1, Pixel Vertice_2){
	int y0 = Vertice_1.getY();
	int x0 = Vertice_1.getX();
	int y1 = Vertice_2.getY();
	int x1 = Vertice_2.getX();


	Pixel paramPixel(x0, y0, Vertice_1.getR(), Vertice_1.getG(), Vertice_1.getB(), Vertice_1.getA());

	int dx = x1 - x0; 
	int dy = y1 - y0; 
	int d, incr_e, incr_ne;
	int x, y;

	if(abs(dx)>abs(dy) && dx!=0 && dy!=0){ //LIMITA 1 E 5 & 8 E 4
		if(dy > 0 && dx > 0){ //LIMITA 1
			Treatment(x, y, x0, y0, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (x < x1) {
				if (d <= 0) {
					d += incr_e;
					x++; 
				} else {
					d += incr_ne;
					x++;
					y++;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}
		} else if(dy < 0 && dx < 0){ //LIMITA 5
			Treatment(x, y, x1, y1, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (x < x0) {
				if (d <= 0) {
					d += incr_e;
					x++;
				} else {
					d += incr_ne;
					x++;
					y++;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}
		} else if(dy < 0 && dx > 0){ //LIMITA O 8
			Treatment(x, y, x0, y0, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (x < x1) {
				if (d <= 0) {
					d += incr_e;
					x++;
				} else {
					d += incr_ne;
					x++;
					y--;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}

		} else if(dy > 0 && dx < 0){ //LIMITA O 4
			Treatment(x, y, x1, y1, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (x < x0) {
				if (d <= 0) {
					d += incr_e;
					x++;
				} else {
					d += incr_ne;
					x++;
					y--;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}
		}
	} else if(abs(dy)>abs(dx) && dx!=0 && dy!=0){ //LIMITA O 2
		if(dy > 0 && dx > 0){
			Treatment(x, y, x0, y0, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (y < y1) {
				if (d <= 0) {
					d += incr_e;
					y++;
				} else {
					d += incr_ne;
					x++;
					y++;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}
		} else if(dy < 0 && dx < 0){ //LIMITA O 6
			Treatment(x, y, x1, y1, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (y < y0) {
				if (d <= 0) {
					d += incr_e;
					y++;
				} else {
					d += incr_ne;
					x++;
					y++;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}

		} else if(dy < 0 && dx > 0){ //LIMITA O 7
			Treatment(x, y, x0, y0, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (x < x1) {
				if (d <= 0) {
					d += incr_e;
					y--;
				} else {
					d += incr_ne;
					x++;
					y--;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}

		}
		else if(dy > 0 && dx < 0){ //LIMITA O 3
			Treatment(x, y, x1, y1, dx, dy, d, incr_e, incr_ne);
			paramPixel.setX(x);
			paramPixel.setY(y);
			PutPixel(paramPixel);
			while (x < x0) {
				if (d <= 0) {
					d += incr_e;
					y--;
				} else {
					d += incr_ne;
					x++;
					y--;
				}
				paramPixel.setX(x);
				paramPixel.setY(y);
				Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
				PutPixel(paramPixel);
			}

		}

	} else if(dy == 0 && dx != 0){
		int aux;
		if(dx > 0) {
			Treatment(x, y, x0, y0, dx, dy, d, incr_e, incr_ne);
			aux = x1;
		} else {
			Treatment(x, y, x1, y1, dx, dy, d, incr_e, incr_ne);
			aux = x0;			
		}
		paramPixel.setX(x);
		paramPixel.setY(y);
		PutPixel(paramPixel);
		while (x < aux) {
			if(dx > 0) x++;
			else x--;
			paramPixel.setX(x);
			paramPixel.setY(y);
			Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
			PutPixel(paramPixel);
		}
	} else if (dx == 0 && dy != 0){
		int aux;
		if(dy > 0) {
			Treatment(x, y, x0, y0, dx, dy, d, incr_e, incr_ne);
			aux = y1;
		} else {
			Treatment(x, y, x1, y1, dx, dy, d, incr_e, incr_ne);
			aux = y0;			
		}
		paramPixel.setX(x);
		paramPixel.setY(y);
		PutPixel(paramPixel);
		while (y < aux) {
			if(dy > 0) y++;
			else y--;
			paramPixel.setX(x);
			paramPixel.setY(y);
			Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
			PutPixel(paramPixel);
		}
	} else if (abs(dy) == abs(dx)){ //dy = dx
		Treatment(x, y, x0, y0, dx, dy, d, incr_e, incr_ne);
		paramPixel.setX(x);
		paramPixel.setY(y);
		PutPixel(paramPixel);
		if(dy==-dx) {
			while (x < x1) {
			if (d <= 0) {
				d += incr_e;
				y--;
			} else {
				d += incr_ne;
				x++;
				y--;
			}
			paramPixel.setX(x);
			paramPixel.setY(y);
			Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
			PutPixel(paramPixel);
			}
		} else {
		while (x < x1) {
			if (d <= 0) {
				d += incr_e;
				y++;
			} else {
				d += incr_ne;
				x++;
				y++;
			}
			paramPixel.setX(x);
			paramPixel.setY(y);
			Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
			PutPixel(paramPixel);
		}
		}
	} else {
		Interpolacao(Vertice_1, Vertice_2, paramPixel, x, y);
		PutPixel(paramPixel);
	}
}

void DrawTriangle(Pixel Vertice_1, Pixel Vertice_2, Pixel Vertice_3){

	DrawLine(Vertice_1, Vertice_2);
	DrawLine(Vertice_2, Vertice_3);
	DrawLine(Vertice_1, Vertice_3);

}
#endif // _MYGL_H_
