/*
  Lorenz attractor explorer and animator.
  BY JAY LECAVALIER

  Explore the Lorenz attractor by changing
  its parameters and viewing it from various
  angles. In addition, watch the 3d path taken
  by the lorenz attractor in animation mode.

  Explorer Mode:
  In explorer mode, view the lorenz attractor from
  various angles. You can change all three 
  parameters (s, b, and r) and also change how
  frequently the colors cycle through the rainbow.

  Animation Mode:
  When you switch to animation mode, you can watch
  the path of the lorenz attractor as it is traced
  before your eyes. During the animation, you can
  still change the viewing angle, the attractor
  parameters, and the frequency of the color cycle.
  When the animation finishes, this mode is equivalent
  to explorer mode. Whenever you switch to animation
  mode, the animation will start over from the
  beginning.  

  Key bindings:
  1 	 Explorer Mode
  2      Animation Mode
  q/a    Increase/decrease 's' parameter
  w/s    Increase/decrease 'b' parameter
  e/d    Increase/decrease 'r' parameter
  z/x    Increase/decrease color cycle frequency
  arrows Change viewing angle
  0      Reset viewing angle
  9      Reset attractor parameters
  8      Reset colors
  ESC    Exit program

*/

// We need standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

// We also need OpenGL
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Globals (oh my!)
double dim=2;
int th=0;
int ph=0;
float color_frequency = 0.0100;
int a_mode = 0;
int a_index = 0;
double t_offset = 0;
int window_width;
int window_height;

// Lorenz attractor parameters
double s = 10;
double b = 2.6666;
double r = 28;

#define LEN 8192 // Maximum length of text string
void Print(const char* format , ...) {
	char buf[LEN];
	char* ch=buf;
	va_list args;
	// Turn the parameters into a character string
	va_start(args, format);
	vsnprintf(buf,LEN,format,args);
	va_end(args);
	// Display the characters one at a time at the current
	// raster position
	while (*ch)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void display() {
  // Now clear the image.
  glClear(GL_COLOR_BUFFER_BIT);
  // Reset previous transformations
  glLoadIdentity();
  // Set viewing angle
  glRotated(ph,1,0,0);
  glRotated(th,0,1,0);
  // The points are 1 pixel and are magenta.
  glColor3f(1,0,1);
  glPointSize(1);
  // Explorer mode.
  if (!a_mode) {
    glBegin(GL_POINTS);
    int i;
  	double x = 1;
  	double y = 1;
  	double z = 1;
  	double dt = 0.001;
    // Calculate 50000 points of the attractor
  	for (i=0;i<50000;i++) {
  	  double dx = s*(y-x);
  	  double dy = x*(r-z)-y;
  	  double dz = x*y - b*z;
  	  x += dt*dx;
  	  y += dt*dy;
  	  z += dt*dz;
  	  // Make the colors really pretty.
  	  glColor3f((sin(i*color_frequency+2)*127+128)/255,
  		        (sin(i*color_frequency+4)*127+128)/255,
  		        (sin(i*color_frequency)*127+128)/255);
  	  // Scale the point to be within the
  	  // small 3d-space we've allocated and
  	  // draw the points.
  	  glVertex3d(x/50,y/50,z/50);
    }
    glEnd();
  }
  // Animation mode.
  else {
  	glBegin(GL_POINTS);
    int i;
  	double x = 1;
  	double y = 1;
  	double z = 1;
  	double dt = 0.001;
  	// We only draw up to the animation index,
  	// which is updated after a certain time
  	// frame.
  	for (i=0;i<a_index;i++) {
  	  double dx = s*(y-x);
  	  double dy = x*(r-z)-y;
  	  double dz = x*y - b*z;
  	  x += dt*dx;
  	  y += dt*dy;
  	  z += dt*dz;
  	  glColor3f((sin(i*color_frequency+2)*127+128)/255,
  		        (sin(i*color_frequency+4)*127+128)/255,
  		        (sin(i*color_frequency)*127+128)/255);
  	  glVertex3d(x/50,y/50,z/50);
    }
    glEnd();
  }
  // Draw axes in white.
  glColor3f(1,1,1);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(1,0,0);
  glVertex3d(0,0,0);
  glVertex3d(0,1,0);
  glVertex3d(0,0,0);
  glVertex3d(0,0,1);
  glEnd();
  // Label the axes
  glRasterPos3d(1,0,0);
  Print("X");
  glRasterPos3d(0,1,0);
  Print("Y");
  glRasterPos3d(0,0,1);
  Print("Z");
  // Display viewing angle
  glWindowPos2i(5,5);
  Print("View Angle=%d,%d",th,ph);
  // Display Lorenz attractor parameters
  glWindowPos2i(5,25);
  Print("s: %.0f, b: %.4f, r: %.0f",s,b,r);
  // Display color frequency
  glWindowPos2i(5,45);
  Print("color frequency: %.4f",color_frequency);
  // Display mode:
  // Explorer mode
  glWindowPos2i(5,65);
  if (!a_mode) {
    Print("Mode: Explorer");
  }
  // Animation mode
  else {
  	Print("Mode: Animation");
  	// Animation ends at 50000 frames.
  	if (a_index == 50000) {
  	  glWindowPos2i(5,85);
  	  Print("Animation complete");
  	}
  	// Tell the user which frame we're on
  	// if the animation hasn't finished yet.
  	else {
  	  glWindowPos2i(5,85);
  	  Print("Animation frame: %d",a_index);
  	}
  }
  // Identifying myself
  glColor3f(0,1,1);
  glWindowPos2i(window_width-130,window_height-20);
  Print("Jay LeCavalier");
  // Flush and swap
  glFlush();
  glutSwapBuffers();
}

void reshape(int width, int height) {
  // Set the global window width and height
  window_height = height;
  window_width = width;
  // Ratio of the width to the height of the window
  double w2h = (height>0) ? (double)width/height : 1;
  // Set the viewport to the entire window
  glViewport(-100,-100,width,height);
  // Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  // Undo previous transformations
  glLoadIdentity();
  // Orthogonal projection box adjusted for the
  // aspect ratio of the window
  glOrtho(.5*(-dim*w2h),.5*(+dim*w2h),.5*(-dim),
  	.5*(+dim),.5*(-dim),.5*(+dim));
  // Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  // Undo previous transformations
  glLoadIdentity();
}

void special(int key,int x,int y) {
  // The arrow keys change the viewing angle
  // In increments of five degrees.
  if (key == GLUT_KEY_RIGHT) {
  	th += 5;
  }
  else if (key == GLUT_KEY_LEFT) {
  	th -= 5;
  }
  else if (key == GLUT_KEY_UP) {
  	ph += 5;
  }
  else if (key == GLUT_KEY_DOWN) {
  	ph -= 5;
  }
  // Angles are in degrees!
  th %= 360;
  ph %= 360;
  // Tell GLUT to redisplay the scene, since
  // the viewing angle was changed.
  glutPostRedisplay();
}

void key(unsigned char ch,int x,int y) {
  // Exit when user presses ESC
  // For some reason, this causes a segfault.
  // If I exit by clicking the 'X' on the window,
  // then I don't get a segfault, but I do when
  // I press ESC...
  if (ch == 27) {
  	exit(0);
  }
  // q,a,w,s,e, and d change attractor parameters
  else if (ch == 'q') {
  	s += 1;
  }
  else if (ch == 'a') {
  	s -= 1;
  }
  else if (ch == 'w') {
  	b += .3333;
  }
  else if (ch == 's') {
  	b -= .3333;
  }
  else if (ch == 'e') {
  	r += 1;
  }
  else if (ch == 'd') {
  	r -= 1;
  }
  // z and x change the color frequency parameter
  else if (ch == 'z') {
  	color_frequency -= .0001;
  }
  else if (ch == 'x') {
  	color_frequency += .0001;
  }
  // 1 and 2 switch between modes.
  // '1' for explorer mode.
  else if (ch == '1') {
  	// reset the animation frame to zero
  	// when we switch back to explorer mode.
  	a_index = 0;
  	a_mode = 0;
  }
  // '2' for animation mode.
  else if (ch == '2') {
  	a_mode = 1;
  	a_index=0;
  	t_offset = glutGet(GLUT_ELAPSED_TIME)/1.06;
  }
  // '0' resets the viewing angle.
  else if (ch == '0') {
  	th = 0;
  	ph = 0;
  }
  // '8' resets the colors.
  else if (ch == '8') {
  	color_frequency = 0.0100;
  }
  // '9' resets attractor parameters.
  else if (ch == '9') {
  	s = 10;
    b = 2.6666;
    r = 28;
  }
  // Tell GLUT to redraw everything on the
  // screen, since we changed the params
  // of the attractor
  glutPostRedisplay();
}

void idle() {
  // The effect of subtracting the elapsed time by the
  // t_offset is that the animation restarts, since
  // the t_offset is the elapsed time when we initialize
  // animation mode.
  double tm = (glutGet(GLUT_ELAPSED_TIME)/1.06) - t_offset;
  // Transition to the next frame of animation
  if (a_index < 50000) {
    a_index = (int)tm;
  }
  // The animation stops at 50000 frames.
  if (a_index > 50000) {
  	a_index = 50000;
  }
  // Now that we've updated the image,
  // we need to redraw the screen.
  glutPostRedisplay();
}

int main(int argc, char* argv[]) {
  // Initialize GLUT and process user parameters
  glutInit(&argc,argv);
  // Request double buffered, true color window
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  // Request 650 X 650 pixel window
  glutInitWindowSize(650,650);
  // Create the window
  glutCreateWindow("Jay LeCavalier - HW #1");
  // Tell GLUT to call the display function whenever
  // it's time to draw the scene
  glutDisplayFunc(display);
  // Tell GLUT to call the reshape function whenever
  // the window is resized.
  glutReshapeFunc(reshape);
  // Tell GLUT to call the special function when
  // the user presses a special key on the keyboard.
  glutSpecialFunc(special);
  // Tell GLUT to call the key function when
  // the user presses a keyboard key
  glutKeyboardFunc(key);
  // Tell GLUT to call the idle function when
  // GLUT isn't doing anything else.
  glutIdleFunc(idle);
  // Pass control to GLUT
  glutMainLoop();
  // Tell the operating system that everything is okay.
  return 0;
}