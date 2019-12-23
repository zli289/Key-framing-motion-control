#include "stdafx.h"
// standard
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include<iostream>
// glut
#include <GL/glut.h>
// source
#include "Source/math/vec3.h"
#include "Source/Quanternion.h"
#include "Source/Model.h"
#include "Source/Interpolation.h"
// global variables

// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;
// frame index
int g_frameIndex = 0;
// angle for rotation
int g_angle = 40;
//model
Model g_model;
Model start;
//eye position
float eye_x= 0.0f;
float eye_y= 0.0f;
float eye_z= 8.0f;
//center 
float pos_x=0.0f;
float pos_y=0.0f;
float pos_z=0.0f;

void init( void ) {
	// init something before main loop...
	g_model.LoadModel( "D files/duck.d.txt" );
	start=g_model;
	KeyFrames();
}
	
// update
void update( void ){
	// do something before rendering...
	if(t<1)
	{
		Model rotated;
		if(rotation==2)
			rotated=Quanternion(qs[0],qs[1+k_index],start,t);
		else if(rotation==1)
			rotated=FixedAngles(fs[k_index]*t,start);

		if(translation==1)
			g_model=Translation(oneset,rotated,t,catmull_rom);
		else if(translation==2)
			g_model=Translation(oneset,rotated,t,b_spline);	
		//
		t+=deltat;
	}
	else 
	{
		if(rotation==2)	
			start=Quanternion(qs[0],qs[1+k_index],start,1);
		else if(rotation==1)
			start=FixedAngles(fs[k_index],start);

		if(k_index!=keyframes.size()-4)
			k_index++;
		else
			k_index=0;
	
		t=0;
		oneset=std::vector<vec3>(keyframes.begin()+k_index,keyframes.begin()+k_index+4);
	}
}
// render
void render( void ) {
	// clear buffer
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth (1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[]	= { 0.11f, 0.06f, 0.11f, 1.0f };
	GLfloat material_Kd[]	= { 0.43f, 0.47f, 0.54f, 1.0f };
	GLfloat material_Ks[]	= { 0.33f, 0.33f, 0.52f, 1.0f };
	GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
	GLfloat material_Se		= 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
	glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);

	// modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	//glTranslatef (pos_x, pos_y, pos_z);
	gluLookAt(
		eye_x, eye_y, eye_z, // eye
		pos_x, pos_y, pos_z, // center
		0, 1, 0  // up
	);
	// render objects
	//glutSolidTeapot(1.0);
	// draw model
	//glLineWidth( 1 );
	//glColor4f( 1.0, 1.0, 1.0, 1.0 );
	g_model.DrawEdges();
	// swap back and front buffers
	glutSwapBuffers();
}
// keyboard input
void keyboard( unsigned char key, int x, int y ) {
	switch (key) {
	case 'w':
		eye_y+=0.5;
		break;
	case 'a':
		eye_x-=0.5;
		break;
	default:
		break;
    }
}
// reshape : update viewport and projection matrix when the window is resized
void reshape( int w, int h ) {
	// screen size
	g_screenWidth  = w;
	g_screenHeight = h;		
	// viewport
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
}
// timer : triggered every 16ms ( about 60 frames per second )
void timer( int value ) {	
	// increase frame index
	g_frameIndex++;
	update();
	// render
	glutPostRedisplay();
	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );
}
int main( int argc, char** argv ) {
	std::cout<<"Rotation: 1. FixedAngles; 2. Quanternion \n";
	std::cin>>rotation;
	std::cout<<"Translation: 1. Catmull_rom; 2. B_spline \n";
	std::cin>>translation;

	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( 600, 600 ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( argv[0] );
	// init
	init();
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( keyboard );
	glutTimerFunc( 16, timer, 0 );
	// main loop
	glutMainLoop();
	return 0;
}