//#include <iostream>
//#include <SDL2/SDL.h>
//#include <gl/GL.h>
//#include <gl/glu.h>
//#include "camera.h"
//#include "functions.h"
//#include <vector>
//
//camera cam;
//std::vector<std::vector<float> > heights;
//
//void loadHeightmap(const char* name)
//{
//	SDL_Surface* img=SDL_LoadBMP(name);
//	if(!img)
//	{
//		std::cout << "image is not loaded" << std::endl;
//		return;
//	}
//	std::vector<float> tmp;
//	for(int i=0;i<img->h;i++)
//	{
//		tmp.clear();
//		for(int j=0;j<img->w;j++)
//		{
//			Uint32 pixel=((Uint32*)img->pixels)[i*img->pitch/4 + j];
//			Uint8 r,g,b;	//unsigned char
//			SDL_GetRGB(pixel,img->format,&r,&g,&b);
//			tmp.push_back((float)r/255.0);	//0.0,1.0
//		}
//		heights.push_back(tmp);
//	}
//}
//
//void renderHeightmap(float size,float h)
//{
//	for(int i=0;i<heights.size()-1;i++)
//		for(int j=0;j<heights[0].size()-1;j++)
//		{
//			glBegin(GL_TRIANGLE_STRIP);
//				glColor3f(heights[i][j],heights[i][j],heights[i][j]);
//				glVertex3f(i*size,heights[i][j]*h,j*size);
//				glVertex3f((i+1)*size,heights[i+1][j]*h,j*size);
//				glVertex3f(i*size,heights[i][j+1]*h,(j+1)*size);
//				glVertex3f((i+1)*size,heights[i+1][j+1]*h,(j+1)*size);
//			glEnd();
//		}
//}
//
//void init(float angle)
//{
//	glClearColor(0,0,0,1);
//	glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		gluPerspective(angle,640.0/480.0,1,1000);
//	glMatrixMode(GL_MODELVIEW);
//	initskybox();
//	glEnable(GL_DEPTH_TEST);
//	loadHeightmap("heightmap.bmp");
//}
//
//void display()
//{
//	glLoadIdentity();
//	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//	cam.Control();
//	drawSkybox(50);
//	
//	cam.UpdateCamera();
//	renderHeightmap(0.01,0.4);
//	
//}
//
//int main()
//{
//	SDL_Init(SDL_INIT_EVERYTHING);
//	SDL_SetVideoMode(640,480,32,SDL_OPENGL);
//	Uint32 start;
//	SDL_Event event;
//	bool running=true;
//	float angle=50;
//	init(angle);
//	bool b=false;
//	while(running)
//	{
//		start=SDL_GetTicks();
//		while(SDL_PollEvent(&event))
//		{
//			switch(event.type)
//			{
//				case SDL_QUIT:
//					running=false;
//					break;
//				case SDL_KEYDOWN:
//					switch(event.key.keysym.sym)
//					{
//						case SDLK_ESCAPE:
//							running=false;
//							break;
//					}
//					break;
//				case SDL_MOUSEBUTTONDOWN:
//					cam.mouseIn(true);
//					break;
//					
//			}
//		}
//		display();
//		SDL_GL_SwapBuffers();
//		if(1000.0/30>SDL_GetTicks()-start)
//			SDL_Delay(1000.0/30-(SDL_GetTicks()-start));
//	}
//	killskybox();
//	SDL_Quit();
//}
