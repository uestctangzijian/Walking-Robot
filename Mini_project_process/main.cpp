/*  
    
    Mini-Project by TANG Zijian 56135491
  
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>

#include "glut.h"
#include "stb_image.h"

//////////////////////////////////////////////////////////////////
// 
// Include the header file of our rotation user-interface.
// 
#include "gsrc.h"
// 
//////////////////////////////////////////////////////////////////

#define PI			3.141592654  // Prime
#define WIN_POSX    100
#define WIN_POSY    100
#define WIN_WIDTH   1280
#define WIN_HEIGHT  720

int    option;                   // specify whether the animation is uniform, accelerating or decelerating
double exponent;                 // control the rate of change of acceleration/decleration
double t_prev;                   // previous time elapsed
double left_upper_arm_angle,left_lower_arm_angle,
	right_upper_arm_angle,right_lower_arm_angle,
	left_upper_leg_angle,right_upper_leg_angle,
	left_lower_leg_angle,right_lower_leg_angle, 
	torso_angle;


void cube ( )
// draw a standard 2 x 2 x 2 cube whose center is at (0, 1, 0)
{

    /* The use of glPushMatrix and glPopMatrix here protects the glRotatef from
	   exercising its influence on the relative transformation beyond this function */
    glPushMatrix ( );

	glutSolidCube (2);

    glPopMatrix  ( );

}

// draw torso of a robot
void torso(void)
{
	glPushMatrix();

	glColor3f(0.63, 0.09, 0.08);
	glScalef(40, 50, 20);
	cube();
	glPopMatrix();

}

// draw head of a robot
void head(void)
{
	glPushMatrix();

	glColor3f(0.89, 0.73, 0.50);
	
	glutSolidSphere(15, 20, 20);

	glPopMatrix();
}

// draw upper arm of a robot
void upper_arm() 
{
	glPushMatrix();
	glColor3f(0.89, 0.73, 0.50);
	glTranslatef(0, -25, 0);
	glScalef(10, 25, 10);
	cube();
	glPopMatrix();

}

// draw lower arm of a robot
void lower_arm()
{
	glPushMatrix();
	glColor3f(0.63, 0.09, 0.08);
	glTranslatef(0, -25, 0);
	glScalef(5, 20, 5);
	cube();
	glPopMatrix();
}

// draw upper leg of a robot
void upper_leg()
{
	glPushMatrix();
	glColor3f(0.89, 0.73, 0.50);
	glTranslatef(0, -12, 0);
	glScalef(13, 28, 13);
	cube();
	glPopMatrix();
}

// draw lower leg of a robot
void lower_leg()
{
	glPushMatrix();
	glColor3f(0.63, 0.09, 0.08);
	glTranslatef(0, -10, 0);
	glScalef(8, 25, 8);
	cube();
	glPopMatrix();
}

// draw ground with texture
void ground() {
	GLfloat materialAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialShininess = 3.0f;

	int width, height, nrChannels;
	unsigned char * data = stbi_load("./resources/floor.bmp", &width, &height, &nrChannels, 0);
	if (data) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);
		glEnable(GL_TEXTURE_2D);
		// assign the full range of texture colors to a quadrilateral
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1000.0f, -1000.0f, 0.0f);
		glTexCoord2f(10.0, 0.0); glVertex3f(1000.0f, -1000.0f, 0.0);
		glTexCoord2f(10.0, 10.0); glVertex3f(1000.0f, 1000.0f, 0.0);
		glTexCoord2f(0.0, 10.0); glVertex3f(-1000.0f, 1000.0f, 0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

}

// add light source
void light(void)
{
	static const GLfloat light0_position[] = { 125.0f, 150.0f, 100.0f, 1.0f };
	static const GLfloat light0_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	static const GLfloat light0_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	static const GLfloat light0_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

}

// draw robot
void robot() {
	// torso and head
	glRotatef(90, 0.0, 1.0, 0.0);
	glPushMatrix();
	torso();
	glTranslatef(0, 60, 0);
	head();
	glPopMatrix();

	// left arm
	glPushMatrix();
	glTranslatef(50, 40, 0);
	glRotatef(left_upper_arm_angle, 1.0, 0.0, 0.0);
	upper_arm();
	glTranslatef(0, -40, 0);
	glRotatef(left_lower_arm_angle, 1.0, 0.0, 0.0);
	lower_arm();
	glPopMatrix();

	// right arm
	glPushMatrix();
	glTranslatef(-50, 40, 0);
	glRotatef(right_upper_arm_angle, 1.0, 0.0, 0.0);
	upper_arm();
	glTranslatef(0, -40, 0);
	glRotatef(right_lower_arm_angle, 1.0, 0.0, 0.0);
	lower_arm();
	glPopMatrix();

	// left leg
	glPushMatrix();
	glTranslatef(20, -55, 0);
	glRotatef(left_upper_leg_angle, 1.0, 0.0, 0.0);
	upper_leg();
	glTranslatef(0, -50, 0);
	glRotatef(left_lower_leg_angle, 1.0, 0.0, 0.0);
	lower_leg();
	glPopMatrix();

	// right leg
	glPushMatrix();
	glTranslatef(-20, -55, 0);
	glRotatef(right_upper_leg_angle, 1.0, 0.0, 0.0);
	upper_leg();
	glTranslatef(0, -50, 0);
	glRotatef(right_lower_leg_angle, 1.0, 0.0, 0.0);
	lower_leg();
	glPopMatrix();
}

// draw shadow
void shadow(void) {
	GLfloat M[16];
	for (int i = 0; i < 16; i++) {
		M[i] = 0;
	}
	M[0] = M[5] = M[10] = 1;
	M[7] = -1.0 / (139 + 150);

	robot();
	
	glPushMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	
	glTranslatef(125, 150, 100);
	glMultMatrixf(M);
	glTranslatef(-125, -150, -100);
	
	glColor3f(0.0, 0.0, 0.0);
	robot();
	
	glPopMatrix();

}


void drawscene (void)     
{


    //////////////////////////////////////////////////////////////////
    // 
    // Setup perspective projection and the rotation
    // 
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport ); // viewport is by default the display window
    glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective( 45, double(viewport[2])/viewport[3], 0.1, 1000 );
    glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt( 400, 200, 200, 0, 0, 0, 0,1,0 );
      glMultMatrixf( gsrc_getmo() );  // get the rotation matrix from the rotation user-interface
    //
    //////////////////////////////////////////////////////////////////


	/*  Enable Z buffer method for visibility determination. */
	//  Z buffer code starts
	
        glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// The Blend Factor
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable (GL_DEPTH_TEST);
	
	// Z buffer code ends */

	glEnable(GL_NORMALIZE);   // normalize normals
	glEnable(GL_TEXTURE_2D);			// texture

	// set light
	light();
	
	glClearColor (0.764, 0.854, 1.0, 0.0);	// Set display-window color to white.
	glClear (GL_COLOR_BUFFER_BIT);		// Clear display window.

	// draw ground with texture
	glPushMatrix();
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glTranslatef(0, -140, 0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	ground();
	glPopMatrix();

	// draw robot with shadow
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	shadow();
	glPopMatrix();

	glutSwapBuffers();
}


void animate ( void )
// this animation function will swing theta from 0 deg. to 90 deg. in 1 seconds, then stop at 90 deg.
{
	 double	t; 
	 double swing_time  = 1000.0;				 // 1000 ms

	 double forward_angle = 45.0;
	 double backward_angle = -45.0;
	 double lower_arm_angle = 90.0;
	 double lower_leg_angle = 30;

	 bool decelerating_step = true;
	 bool accelerating_step = false;
	 
	 t = glutGet (GLUT_ELAPSED_TIME);            // return elapsed time in ms since the last call  

	 // walking animation
	 if (true)
	 {
		 // Decelerating 
		 if (decelerating_step) 
		 {
			left_upper_arm_angle = forward_angle * pow(sin(PI * t / (2 * swing_time)), exponent);
			right_upper_arm_angle = backward_angle * pow(sin(PI * t / (2 * swing_time)), exponent);
			
			
			if (left_upper_arm_angle > 0)
			{
				left_lower_arm_angle = lower_arm_angle * pow(sin(PI * t / (2 * swing_time)), 2 * exponent);
				right_upper_leg_angle = forward_angle * pow(sin(PI * t / (2 * swing_time)), exponent);
				right_lower_leg_angle = -lower_leg_angle * pow(sin(PI * t / (2 * swing_time)), exponent);
			}
			if (right_upper_arm_angle > 0)
			{
				right_lower_arm_angle = lower_arm_angle * pow(sin(PI * t / (2 * swing_time)), 2 * exponent);
				left_upper_leg_angle = backward_angle * pow(sin(PI * t / (2 * swing_time)), exponent);
				left_lower_leg_angle = lower_leg_angle * pow(sin(PI * t / (2 * swing_time)), exponent);
				
			}
			
			if (left_upper_arm_angle == forward_angle || left_upper_arm_angle == - forward_angle)
				accelerating_step = true;
		}
		 // Accelerating
		 else if (accelerating_step)
		 {
			 left_upper_arm_angle = forward_angle * (1 - pow(cos(PI * t / (2 * swing_time)), exponent));
			 right_upper_arm_angle = backward_angle * (1 - pow(cos(PI * t / (2 * swing_time)), exponent));
			 left_upper_leg_angle = forward_angle * pow(sin(PI * t / (2 * swing_time)), exponent);

			 if (left_upper_arm_angle == 0)
				 decelerating_step = true;
		 }
	 }
		 
	 glutPostRedisplay ( );
}


void main (int argc, char** argv)
{
     
		glutInit (&argc, argv);			                      // Initialize GLUT
		glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB |  GLUT_DEPTH); // Set display mode
		
		glutInitWindowPosition( WIN_POSX, WIN_POSY );         // Set display-window position at (WIN_POSX, WIN_POSY) 
                                                              // where (0, 0) is top left corner of monitor screen
        glutInitWindowSize( WIN_WIDTH, WIN_HEIGHT );		  // Set display-window width and height.

		glutCreateWindow ("Mini Project --  Walking Robot" );					  // Create display window.

		left_upper_arm_angle = 0; left_lower_arm_angle = 0; 
		right_upper_arm_angle = 0; right_lower_arm_angle = 0; 
		left_upper_leg_angle = 0; right_upper_leg_angle = 0; 
		left_lower_leg_angle = 0; right_lower_leg_angle = 0;
		exponent = 1;

	    printf ("This is Mini Project, by TANGZijian 56135491"); 
		
		glutIdleFunc (animate);

		//////////////////////////////////////////////////////////////////
	    // 
		// Register mouse-click and mouse-move glut callback functions
		// for the rotation user-interface.
		// 
		glutMouseFunc( gsrc_mousebutton );
		glutMotionFunc( gsrc_mousemove );
		//
		//////////////////////////////////////////////////////////////////
		
        glutDisplayFunc (drawscene);   // put everything you wish to draw in drawscene

		glutMainLoop ( );
	
}