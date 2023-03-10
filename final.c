// TRISTAN ENGLISH
// CSCI 4229
// FINAL PROJECT

#include "CSCIx229.h"


int th=-45;          //  Azimuth of view angle
int ph=30;          //  Elevation of view angle
int axes=0;        //  Display axes
int mode=0;       //  Scene number
int move=1;       //  Move light
int fov=35;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=4.0;   //  Size of world
// Light values
int light     =   1;  // Lighting
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  10;  // Ambient intensity (%)
int diffuse   =  50;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   2.5;  // Elevation of light
// Textures
unsigned int texture[12];  //  Texture names
typedef struct {float x,y,z;} vtx;


/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball with yellow specular
   float yellow[]   = {1.0,1.0,0.0,1.0};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (int ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

static void normal(float Ax, float Ay, float Az, float Bx, float By, float Bz,float Cx, float Cy, float Cz)
{
      //  Planar vector 0
   float dx0 = Ax-Bx;
   float dy0 = Ay-By;
   float dz0 = Az-Bz;
   //  Planar vector 1
   float dx1 = Cx-Ax;
   float dy1 = Cy-Ay;
   float dz1 = Cz-Az;
   //  Normal
   float Nx = dy0*dz1 - dy1*dz0;
   float Ny = dz0*dx1 - dz1*dx0;
   float Nz = dx0*dy1 - dx1*dy0;
   glNormal3f(Nx,Ny,Nz);
}

/*
 * Draw triangle
 */
static void triangle(vtx A,vtx B,vtx C)
{
   //  Planar vector 0
   float dx0 = A.x-B.x;
   float dy0 = A.y-B.y;
   float dz0 = A.z-B.z;
   //  Planar vector 1
   float dx1 = C.x-A.x;
   float dy1 = C.y-A.y;
   float dz1 = C.z-A.z;
   //  Normal
   float Nx = dy0*dz1 - dy1*dz0;
   float Ny = dz0*dx1 - dz1*dx0;
   float Nz = dx0*dy1 - dx1*dy0;
   //  Draw triangle
   glNormal3f(Nx,Ny,Nz);
   glBegin(GL_TRIANGLES);
   glTexCoord2f(.5,.5); glVertex3f(A.x,A.y,A.z);
   glTexCoord2f(0,1); glVertex3f(B.x,B.y,B.z);
   glTexCoord2f(0,0); glVertex3f(C.x,C.y,C.z);
   glEnd();
}

/*
 *  Draw a penguin
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     color (a,b,c)
 */
static void penguin(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double a, double b, double c)
{
      //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   
   // Model was built going -.7 over y axis
   // If you want the penguin to sit at y = 0, multiply .71 by the scale (prevents z-fighting with ground plane).
   if (y == 0)
   {
      y = .71 * dy;
   }

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   /*
   //  BODY
   */
   //glEnable(GL_TEXTURE_2D);
   //glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   //glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   //  Front (white)
   glColor3f(1,1,1);
   normal(-0.5,0.5, 0,0.5,0.5, 0,0.5,0, 0.2);
   glTexCoord2f(0,.5); glVertex3f(-0.5,0.5, 0);
   glTexCoord2f(.5,.5); glVertex3f(0.5,0.5, 0);
   glTexCoord2f(.5,0); glVertex3f(0.5,0, 0.2);
   glTexCoord2f(0,0); glVertex3f(-0.5,0, 0.2);
   glEnd();

   // Front bottom (white)
   glBegin(GL_QUADS);
   glColor3f(1,1,1);
   normal(-0.5,0, 0.2,0.5,0, 0.2,0.5,-0.7, 0);
   glTexCoord2f(0,.5); glVertex3f(-0.5,0, 0.2);
   glTexCoord2f(.5,.5); glVertex3f(0.5,0, 0.2);
   glTexCoord2f(.5,0); glVertex3f(0.5,-0.7, 0);
   glTexCoord2f(0,0); glVertex3f(-0.5,-0.7, 0);
   glEnd();

   // Body side left (blue)
   glBegin(GL_QUADS);
   glColor3f(a,b,c);
   glNormal3f(-1,0,0);
   glTexCoord2f(0,1); glVertex3f(-0.5,0.5, 0);
   glTexCoord2f(1,1); glVertex3f(-0.5,-0.7, 0);
   glTexCoord2f(1,0); glVertex3f(-0.5,-0.7, -0.5);
   glTexCoord2f(0,0); glVertex3f(-0.5,0.5, -0.5);
   glEnd();


   // Body back (blue)
   glBegin(GL_QUADS);
   glColor3f(a,b,c);
   glNormal3f(0,0,-1);
   glTexCoord2f(0,1); glVertex3f(0.5,0.5, -0.5);
   glTexCoord2f(1,1); glVertex3f(-0.5,0.5, -0.5);
   glTexCoord2f(1,0); glVertex3f(-0.5,-0.7, -0.5);
   glTexCoord2f(0,0); glVertex3f(0.5,-0.7, -0.5);
   glEnd();
   //glDisable(GL_TEXTURE_2D);

   // Body side right (blue)
   glBegin(GL_QUADS);
   glColor3f(a,b,c);
   glNormal3f(1,0,0);
   glTexCoord2f(0,1); glVertex3f(0.5,0.5, 0);
   glTexCoord2f(1,1); glVertex3f(0.5,-0.7, 0);
   glTexCoord2f(1,0); glVertex3f(0.5,-0.7, -0.5);
   glTexCoord2f(0,0); glVertex3f(0.5,0.5, -0.5);

   // RIGHT ARM
   // -top-
   
   glColor3f(a,b,c);
   normal(0.5,0.3,-.1,0.5,0.3, -0.4,0.7,-0.3,-.4);
   glTexCoord2f(0,1); glVertex3f(0.5,0.3,-.1);
   glTexCoord2f(1,1); glVertex3f(0.5,0.3, -0.4);
   glTexCoord2f(1,0); glVertex3f(0.7,-0.3,-.4);
   glTexCoord2f(0,0); glVertex3f(0.7,-0.3,-.1);
   

   double offset = 0.05;
   
   // -bottom-
   glColor3f(a,b,c);
   normal(0.5-offset,0.3-offset, -0.4,0.5-offset,0.3-offset,-.1,0.7-offset,-0.3-offset,-.1);
   glTexCoord2f(0,1); glVertex3f(0.5-offset,0.3-offset, -0.4);
   glTexCoord2f(1,1); glVertex3f(0.5-offset,0.3-offset,-.1);
   glTexCoord2f(1,0); glVertex3f(0.7-offset,-0.3-offset,-.1);
   glTexCoord2f(0,0); glVertex3f(0.7-offset,-0.3-offset,-.4);
   
   // -front side-
   glColor3f(a,b,c);
   normal(0.5-offset,0.3-offset,-.1,0.5,0.3,-.1,0.7,-0.3,-.1);
   glTexCoord2f(0,1); glVertex3f(0.5-offset,0.3-offset,-.1);
   glTexCoord2f(1,1); glVertex3f(0.5,0.3,-.1);
   glTexCoord2f(1,0); glVertex3f(0.7,-0.3,-.1);
   glTexCoord2f(0,0); glVertex3f(0.7-offset,-0.3-offset,-.1);
   // -back side-
   glColor3f(a,b,c);
   normal(0.5-offset,0.3-offset,-.4,0.7-offset,-0.3-offset,-.4,0.7,-0.3,-.4);
   glTexCoord2f(0,1); glVertex3f(0.5-offset,0.3-offset,-.4);
   glTexCoord2f(1,1); glVertex3f(0.7-offset,-0.3-offset,-.4);
   glTexCoord2f(1,0); glVertex3f(0.7,-0.3,-.4);
   glTexCoord2f(0,0); glVertex3f(0.5,0.3,-.4);

   // -bottom hand-
   normal(0.7,-0.3,-.1,0.7,-0.3,-.4,0.7-offset,-0.3-offset,-.4);
   glTexCoord2f(0,1); glVertex3f(0.7,-0.3,-.1);
   glTexCoord2f(1,1); glVertex3f(0.7,-0.3,-.4);
   glTexCoord2f(1,0); glVertex3f(0.7-offset,-0.3-offset,-.4);
   glTexCoord2f(0,0); glVertex3f(0.7-offset,-0.3-offset,-.1);

   



   // LEFT ARM
   // -top-
   glColor3f(a,b,c);
   normal(-0.5,0.3, -0.4,-0.5,0.3,-.1,-0.7,-0.3,-.1);
   glTexCoord2f(0,1); glVertex3f(-0.5,0.3, -0.4);
   glTexCoord2f(1,1); glVertex3f(-0.5,0.3,-.1);
   glTexCoord2f(1,0); glVertex3f(-0.7,-0.3,-.1);
   glTexCoord2f(0,0); glVertex3f(-0.7,-0.3,-.4);

   // -bottom-    
   glColor3f(a,b,c);
   normal(-0.5+offset,0.3-offset,-.1,-0.5+offset,0.3-offset, -0.4,-0.7+offset,-0.3-offset,-.4);
   glTexCoord2f(0,1); glVertex3f(-0.5+offset,0.3-offset,-.1);
   glTexCoord2f(1,1); glVertex3f(-0.5+offset,0.3-offset, -0.4);
   glTexCoord2f(1,0); glVertex3f(-0.7+offset,-0.3-offset,-.4);
   glTexCoord2f(0,0); glVertex3f(-0.7+offset,-0.3-offset,-.1);


   // -front side-
   glColor3f(a,b,c);
   normal(-0.7+offset,-0.3-offset,-.1,-0.7,-0.3,-.1,-0.5,0.3,-.1);
   glTexCoord2f(0,1); glVertex3f(-0.7+offset,-0.3-offset,-.1);
   glTexCoord2f(1,1); glVertex3f(-0.7,-0.3,-.1);
   glTexCoord2f(1,0); glVertex3f(-0.5,0.3,-.1);
   glTexCoord2f(0,0); glVertex3f(-0.5+offset,0.3-offset,-.1);

   // -back side-
   glColor3f(a,b,c);
   normal(-0.5+offset,0.3-offset,-.4,-0.5,0.3,-.4,-0.7,-0.3,-.4);
   glTexCoord2f(0,1); glVertex3f(-0.5+offset,0.3-offset,-.4);
   glTexCoord2f(1,1); glVertex3f(-0.5,0.3,-.4);
   glTexCoord2f(1,0); glVertex3f(-0.7,-0.3,-.4);
   glTexCoord2f(0,0); glVertex3f(-0.7+offset,-0.3-offset,-.4);

   // -bottom hand-

   normal(-0.7,-0.3,-.4,-0.7,-0.3,-.1,-0.7+offset,-0.3-offset,-.1);
   glTexCoord2f(0,1); glVertex3f(-0.7,-0.3,-.4);
   glTexCoord2f(1,1); glVertex3f(-0.7,-0.3,-.1);
   glTexCoord2f(1,0);  glVertex3f(-0.7+offset,-0.3-offset,-.1);
   glTexCoord2f(0,0);  glVertex3f(-0.7+offset,-0.3-offset,-.4);



   // Body top (blue)
   glColor3f(a,b,c);
   glNormal3f(0,1,0);
   glTexCoord2f(0,1); glVertex3f(-0.5,0.5, 0);
   glTexCoord2f(1,1); glVertex3f(0.5,0.5, 0);
   glTexCoord2f(1,0); glVertex3f(0.5,0.5, -0.5);
   glTexCoord2f(0,0); glVertex3f(-0.5,0.5, -0.5);

   // Body bottom (white)
   glColor3f(1,1,1);
   glNormal3f(0,-1,0);
   glTexCoord2f(0,2); glVertex3f(0.5,-0.7, 0);
   glTexCoord2f(2,2); glVertex3f(-0.5,-0.7, 0);
   glTexCoord2f(2,0); glVertex3f(-0.5,-0.7, -0.5);
   glTexCoord2f(0,0); glVertex3f(0.5,-0.7, -0.5);


   // RIGHT FOOT
   glColor3f(1,.647,0);
   // bottom
   glNormal3f(0,-1,0);
   glTexCoord2f(0,1); glVertex3f(0.1,-0.7, 0);
   glTexCoord2f(1,1); glVertex3f(0.4,-0.7, 0);
   glTexCoord2f(1,0); glVertex3f(0.4,-0.7, 0.3);
   glTexCoord2f(0,0); glVertex3f(0.1,-0.7, 0.3);

   // top
   glNormal3f(0,1,0);
   glTexCoord2f(0,1); glVertex3f(0.1,-0.65, 0);
   glTexCoord2f(1,1); glVertex3f(0.4,-0.65, 0);
   glTexCoord2f(1,0); glVertex3f(0.4,-0.65, 0.3);
   glTexCoord2f(0,0); glVertex3f(0.1,-0.65, 0.3);

   // front
   glNormal3f(0,0,1);
   glTexCoord2f(0,1); glVertex3f(0.4,-0.65, 0.3);
   glTexCoord2f(1,1); glVertex3f(0.4,-0.7, 0.3);
   glTexCoord2f(1,0); glVertex3f(0.1,-0.7, 0.3);
   glTexCoord2f(0,0); glVertex3f(0.1,-0.65, 0.3);

   // right side
   glNormal3f(1,0,0);
   glTexCoord2f(0,1); glVertex3f(0.4,-0.65, 0.3);
   glTexCoord2f(1,1); glVertex3f(0.4,-0.7, 0.3);
   glTexCoord2f(1,0); glVertex3f(0.4,-0.7, 0);
   glTexCoord2f(0,0); glVertex3f(0.4,-0.65, 0);

   // left side
   glNormal3f(-1,0,0);
   glTexCoord2f(0,1); glVertex3f(0.1,-0.65, 0.3);
   glTexCoord2f(1,1); glVertex3f(0.1,-0.7, 0.3);
   glTexCoord2f(1,0); glVertex3f(0.1,-0.7, 0);
   glTexCoord2f(0,0); glVertex3f(0.1,-0.65, 0);

   

   // LEFT FOOT
   // bottom
   glNormal3f(0,-1,0);
   glTexCoord2f(0,1); glVertex3f(-0.1,-0.7, 0);
   glTexCoord2f(1,1); glVertex3f(-0.4,-0.7, 0);
   glTexCoord2f(1,0); glVertex3f(-0.4,-0.7, 0.3);
   glTexCoord2f(0,0); glVertex3f(-0.1,-0.7, 0.3);

   // top
   glNormal3f(0,1,0);
   glTexCoord2f(0,1); glVertex3f(-0.1,-0.65, 0);
   glTexCoord2f(1,1); glVertex3f(-0.4,-0.65, 0);
   glTexCoord2f(1,0); glVertex3f(-0.4,-0.65, 0.3);
   glTexCoord2f(0,0); glVertex3f(-0.1,-0.65, 0.3);

   // left
   glNormal3f(-1,0,0);
   glTexCoord2f(0,1); glVertex3f(-0.4,-0.65, 0.3);
   glTexCoord2f(1,1); glVertex3f(-0.4,-0.7, 0.3);
   glTexCoord2f(1,0); glVertex3f(-0.4,-0.7, 0);
   glTexCoord2f(0,0); glVertex3f(-0.4,-0.65, 0);

   // right
   glNormal3f(1,0,0);
   glTexCoord2f(0,1); glVertex3f(-0.1,-0.65, 0.3);
   glTexCoord2f(1,1); glVertex3f(-0.1,-0.7, 0.3);
   glTexCoord2f(1,0); glVertex3f(-0.1,-0.7, 0);
   glTexCoord2f(0,0); glVertex3f(-0.1,-0.65, 0);

   // front
   glNormal3f(0,0,1);
   glTexCoord2f(0,1); glVertex3f(-0.4,-0.65, 0.3);
   glTexCoord2f(1,1); glVertex3f(-0.4,-0.7, 0.3);
   glTexCoord2f(1,0); glVertex3f(-0.1,-0.7, 0.3);
   glTexCoord2f(0,0); glVertex3f(-0.1,-0.65, 0.3);

   glEnd();

   glColor3f(1,1,1);
   // RIGHT SIDE (white)
   vtx A = {0.5,0,0.2};
   vtx B = {0.5,0.5,0};
   vtx C = {0.5,-0.7,0};
   triangle(A,B,C);

   // LEFT SIDE (white)
   glColor3f(1,1,1);
   vtx D = {-0.5,0, 0.2};
   vtx E = {-0.5,0.5, 0};
   vtx F = {-0.5,-0.7, 0};
   triangle(E,D,F);

   // HEAD
   glColor3f(a,b,c);
   glTranslated(0,.7,-.15);
   glScaled(.7,.7,.7);
   for (int ph=-90;ph<90;ph+=15)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=15)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   // BEAK
   glColor3f(1,.647,0);
   glScaled(6,6,6);

   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      normal(0.0, 0.0, 0.35,0.05*Cos(th+5),0.05*Sin(th+5),0,0.05*Cos(th),0.05*Sin(th),0);
      glTexCoord2f(0.5,0.5); glVertex3d(0.0, 0.0, 0.3);
      glTexCoord2f(Cos(th+5)+0.5,Sin(th+5)+0.5); glVertex3d(0.05*Cos(th+5),0.05*Sin(th+5),0);
      glTexCoord2f(Cos(th)+0.5,Sin(th)+0.5); glVertex3d(0.05*Cos(th),0.05*Sin(th),0);
      glEnd();
   }
   //glDisable(GL_TEXTURE_2D);

   // RIGHT EYE
   glColor3f(0,0,0);
   glTranslated(.08,.06,.13);
   glScaled(.01,.01,.01);
   for (int ph=-90;ph<90;ph+=15)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=15)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   // LEFT EYE
   glColor3f(0,0,0);
   glTranslated(-15,0,0);
   glScaled(1.1,1.1,1.1);
   for (int ph=-90;ph<90;ph+=15)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=15)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   glPopMatrix();
}

/*
 *  Draw a door
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 * 
 * */
static void door(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[8]);

   glBegin(GL_QUADS);
   glColor3d(1,1,1);
   glNormal3d(0,0,1);
   glTexCoord2d(0,1); glVertex3d(-1,4,0);
   glTexCoord2d(1,1); glVertex3d(1,4,0);
   glTexCoord2d(1,0); glVertex3d(1,0,0);
   glTexCoord2d(0,0); glVertex3d(-1,0,0);

   glNormal3d(1,0,0);
   glTexCoord2d(0,.1); glVertex3d(1,0,0);
   glTexCoord2d(1,.1); glVertex3d(1,4,0);
   glTexCoord2d(1,0); glVertex3d(1,4,-0.2);
   glTexCoord2d(0,0); glVertex3d(1,0,-0.2);

   glNormal3d(-1,0,0);
   glTexCoord2d(0,.1); glVertex3d(-1,0,0);
   glTexCoord2d(1,.1); glVertex3d(-1,4,0);
   glTexCoord2d(1,0); glVertex3d(-1,4,-0.2);
   glTexCoord2d(0,0); glVertex3d(-1,0,-0.2);

   glNormal3d(0,0,-1);
   glTexCoord2d(0,1); glVertex3d(-1,4,-0.2);
   glTexCoord2d(1,1); glVertex3d(1,4,-0.2);
   glTexCoord2d(1,0); glVertex3d(1,0,-0.2);
   glTexCoord2d(0,0); glVertex3d(-1,0,-0.2);

   glNormal3d(0,1,0);
   glTexCoord2d(0,.1); glVertex3d(-1,4,0);
   glTexCoord2d(0,0); glVertex3d(-1,4,-0.2);
   glTexCoord2d(1,0); glVertex3d(1,4,-0.2);
   glTexCoord2d(1,.1); glVertex3d(1,4,0);

   glNormal3d(0,-1,0);
   glTexCoord2d(0,.1); glVertex3d(-1,0,0);
   glTexCoord2d(0,0); glVertex3d(-1,0,-0.2);
   glTexCoord2d(1,0); glVertex3d(1,0,-0.2);
   glTexCoord2d(1,.1); glVertex3d(1,0,0);


      glEnd();

   glColor3f(.2,.2,2);
   glTranslated(0.75,1.8,.1);
   glScaled(.1,.1,.1);
   glColor3f(.83,.69,.21);
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   glTranslated(0,0,-4);
   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }


   glDisable(GL_TEXTURE_2D);
   glPopMatrix();

}

/*
 *  Draw a sphere
 *     at (x,y,z)
 *     color (cx,cy,cz)
 *     dimensions (dx,dy,dz)
 */
static void sphere(double x, double y, double z, double dx, double dy, double dz)
{
   glPushMatrix();

   glTranslated(x,y,z);
   glScaled(dx,dy,dz);
   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }

   glPopMatrix();
}

/*
 *  Draw a cuboid
 *     at (x,y,z)
 *     color (cx,cy,cz)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     with tex texture index
 *     and s repetetion factor
 */
static void cuboid(double x,double y,double z, double dx,double dy,double dz,double th, int tex, double s)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glBindTexture(GL_TEXTURE_2D,texture[tex]);
   glBegin(GL_QUADS);

   // front
   glNormal3d(0,0,1);
   glTexCoord2d(0,s); glVertex3d(0,1,0);
   glTexCoord2d(s,s); glVertex3d(1,1,0);
   glTexCoord2d(s,0); glVertex3d(1,0,0);
   glTexCoord2d(0,0); glVertex3d(0,0,0);

   // right
   glNormal3d(1,0,0);
   glTexCoord2d(0,0); glVertex3d(1,0,0);
   glTexCoord2d(s,0); glVertex3d(1,1,0);
   glTexCoord2d(s,s); glVertex3d(1,1,-1);
   glTexCoord2d(0,s); glVertex3d(1,0,-1);

   // left
   glNormal3d(-1,0,0);
   glTexCoord2d(0,0); glVertex3d(0,0,0);
   glTexCoord2d(s,0); glVertex3d(0,1,0);
   glTexCoord2d(s,s); glVertex3d(0,1,-1);
   glTexCoord2d(0,s); glVertex3d(0,0,-1);

   // back
   glNormal3d(0,0,-1);
   glTexCoord2d(0,s); glVertex3d(0,1,-1);
   glTexCoord2d(s,s); glVertex3d(1,1,-1);
   glTexCoord2d(s,0); glVertex3d(1,0,-1);
   glTexCoord2d(0,0); glVertex3d(0,0,-1);

   // top
   glNormal3d(0,1,0);
   glTexCoord2d(0,0); glVertex3d(0,1,0);
   glTexCoord2d(0,s); glVertex3d(0,1,-1);
   glTexCoord2d(s,s); glVertex3d(1,1,-1);
   glTexCoord2d(s,0); glVertex3d(1,1,0);

   // bottom
   glNormal3d(0,-1,0);
   glTexCoord2d(0,0); glVertex3d(0,0,0);
   glTexCoord2d(0,s); glVertex3d(0,0,-1);
   glTexCoord2d(s,s); glVertex3d(1,0,-1);
   glTexCoord2d(s,0); glVertex3d(1,0,0);


   glEnd();
   //glDisable(GL_TEXTURE_2D);


   glPopMatrix();

}


/*
 *  Draw a dresser
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void dresser(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   double cx = .63;
   double cy = .50;
   double cz = .48;

   double kx = .83;
   double ky = .69;
   double kz = .21;

   // Main Dresser
   glColor3d(cx,cy,cz);
   cuboid(x,y,z,2,3,1,0,4,.5);

   // Shelves
   cuboid(x+.15,y+2.3,z+.15,1.7,.5,.25,0,4,.5);
   cuboid(x+.15,y+1.6,z+.15,1.7,.5,.25,0,4,.5);
   cuboid(x+.15,y+0.9,z+.15,1.7,.5,.25,0,4,.5);
   cuboid(x+.15,y+0.2,z+.15,1.7,.5,.25,0,4,.5);
   cuboid(x-.05,y+3,z+.15,2.1,.1,1.3,0,4,.5);

   // Knobs
   glColor3f(kx,ky,kz);
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   sphere(x+.15+.25,y+2.55,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+2.55,z+.19,.05,.05,.05);

   sphere(x+.15+.25,y+1.85,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+1.85,z+.19,.05,.05,.05);

   sphere(x+.15+.25,y+1.15,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+1.15,z+.19,.05,.05,.05);

   sphere(x+.15+.25,y+0.45,z+.19,.05,.05,.05);
   sphere(x+.15+1.45,y+0.45,z+.19,.05,.05,.05);


   //trapezoid(0,3,0,1,1,1,.5);
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();

}

/*
 *  Draw a nightstand
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void nightstand(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   double cx = .43;
   double cy = .30;
   double cz = .28;

   double kx = .83;
   double ky = .69;
   double kz = .21;

   // Main Dresser
   glColor3d(cx,cy,cz);
   cuboid(x,y,z,1,1.6,1,th,4,1);
   cuboid(x-.05,y+1.6,z+.15,1.1,.1,1.3,0,4,1);

   // Shelves
   cuboid(x+.15,y+0.9,z+.15,0.7,.5,.25,0,4,1);
   cuboid(x+.15,y+0.2,z+.15,0.7,.5,.25,0,4,1);

   // Knobs
   glColor3d(kx,ky,kz);
   glBindTexture(GL_TEXTURE_2D,texture[6]);
   sphere(x+.5,y+1.15,z+.19,.05,.05,.05);

   sphere(x+.5,y+0.45,z+.19,.05,.05,.05);

   glDisable(GL_TEXTURE_2D);  
   glPopMatrix();

}

/*
 *  Draw a pillow
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void pillow(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th
                 )
{
   //  Save transformation
   glPushMatrix();

   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);
   glScaled(.5,4,1);
   glColor3d(.8,.8,.8);

   // Cylinder
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),2.5);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),0);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),2.5);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),0);
      glEnd();
   }

   // Cylinder Bottom
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(.2*Cos(th),.2*Sin(th),0);
      glVertex3d(.2*Cos(th),.2*Sin(th),0);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),0);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }


   // Cylinder Top
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(.2*Cos(th),.2*Sin(th),2.5);
      glVertex3d(.2*Cos(th),.2*Sin(th),2.5);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),2.5);
      glVertex3d(.2*Cos(th+5),.2*Sin(th+5),2.5);
      glNormal3f(0,0,1);
      glVertex3d(0,0,2.5);
      glEnd();
   }

   glPopMatrix();
   glPopMatrix();

}

/*
 *  Draw a bed
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void bed(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th
                 )
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   // Draw pillows
   pillow(.4,.6,-3.3,.5,.5,.5,75);
   pillow(.4,.6,-1.6,.5,.5,.5,75);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   // Draw bed frame
   cuboid(0,0.01,0,4,.5,3.5,0,5,1);
   glColor3d(0,0,.5);
   cuboid(-.1,0,.1,4.2,.4,3.7,0,4,.5);
   cuboid(-.1,0,.1,.1,1.3,3.7,0,4,.5);

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

/*
 *  Draw a lamp
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 */
static void lamp(double x,double y,double z,
                 double dx,double dy,double dz)
{
   //  Save transformation
   glPushMatrix();

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[11]);

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(-90,1,0,0);
   glScaled(dx,dy,dz);
   glColor3d(.3,.3,.5);

   // Lamp Shade
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(2*Cos(th),2*Sin(th),0);
      glTexCoord2d(0,th/90.0);glVertex3d(2*Cos(th),2*Sin(th),0);
      glNormal3f(Cos(th),Sin(th),3);
      glTexCoord2d(1,th/90.0);glVertex3d(Cos(th),Sin(th),3);
      glNormal3f(Cos(th+5),Sin(th+5),3);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(Cos(th+5),Sin(th+5),3);
      glNormal3f(2*Cos(th+5),2*Sin(th+5),0);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(2*Cos(th+5),2*Sin(th+5),0);
      glEnd();
   }
   glColor3d(1,1,1);
   glPushMatrix();
   glTranslated(0,0,-3.5);
   glBindTexture(GL_TEXTURE_2D,texture[6]);

   // Pole
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(.2*Cos(th),.2*Sin(th),0);
      glTexCoord2d(0,th/90.0);glVertex3d(.2*Cos(th),.2*Sin(th),0);
      glTexCoord2d(1,th/90.0);glVertex3d(.2*Cos(th),.2*Sin(th),5);
      glNormal3f(.2*Cos(th+5),.2*Sin(th+5),0);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(.2*Cos(th+5),.2*Sin(th+5),5);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(.2*Cos(th+5),.2*Sin(th+5),0);
      glEnd();
   }

   // Base
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(1.5*Cos(th),1.5*Sin(th),0);
      glTexCoord2d(0,th/90.0);glVertex3d(1.5*Cos(th),1.5*Sin(th),0);
      glTexCoord2d(1,th/90.0);glVertex3d(1.5*Cos(th),1.5*Sin(th),.3);
      glNormal3f(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),.3);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glEnd();
   }

   // Base Bottom
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(1.5*Cos(th),1.5*Sin(th),0);
      glTexCoord2d(Cos(th),Sin(th));glVertex3d(1.5*Cos(th),1.5*Sin(th),0);
      glNormal3f(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glTexCoord2d(Cos(th+5),Sin(th+5));glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glTexCoord2d(0,0);glVertex3d(0,0,0);
      glEnd();
   }

   // Base Top
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(1.5*Cos(th),1.5*Sin(th),.3);
      glTexCoord2d(Cos(th),Sin(th));glVertex3d(1.5*Cos(th),1.5*Sin(th),.3);
      glNormal3f(1.5*Cos(th+5),1.5*Sin(th+5),.3);
      glTexCoord2d(Cos(th+5),Sin(th+5));glVertex3d(1.5*Cos(th+5),1.5*Sin(th+5),.3);
      glNormal3f(0,0,1);
      glTexCoord2d(0,0);glVertex3d(0,0,.3);
      glEnd();
   }
   glPopMatrix();

   glColor3d(1,1,.9);
   glDisable(GL_LIGHTING);
   glDisable(GL_TEXTURE_2D);
   // Lightbulb
   sphere(0,0,2,.6,.6,.6);
   if(light)
      glEnable(GL_LIGHTING);

   glPopMatrix();


}

/*
 *  Draw a luxo lamp
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void luxo(double x,double y,double z,
                 double dx,double dy,double dz, double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glColor3d(.8,.8,.8);

   glPushMatrix();
   glRotated(-20,1,0,0);

   // Lamp Shade
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(2*Cos(th),2*Sin(th),0);
      glTexCoord2d(0,th/90.0); glVertex3d(2*Cos(th),2*Sin(th),0);
      glNormal3f(Cos(th),Sin(th),2.5);
      glTexCoord2d(1,th/90.0);glVertex3d(Cos(th),Sin(th),2.5);
      glNormal3f(Cos(th+5),Sin(th+5),2.5);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(Cos(th+5),Sin(th+5),2.5);
      glNormal3f(2*Cos(th+5),2*Sin(th+5),0);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(2*Cos(th+5),2*Sin(th+5),0);
      glEnd();
   }

   glPushMatrix();
   glTranslated(0,0,2.5);


   // Lamp Shade Top Sides
   double r = 1;
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(0,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(1,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),1);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),1);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }

   // Lamp Shade Top Top
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),1);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th),r*Sin(th),1);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),1);
      glTexCoord2d(Cos(th+5),Sin(th+5)); glVertex3d(r*Cos(th+5),r*Sin(th+5),1);
      glNormal3f(0,0,1);
      glTexCoord2d(0,0); glVertex3d(0,0,1);
      glEnd();
   }
   
   // Lamp Shade Top Bottom
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th),r*Sin(th),0);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glTexCoord2d(Cos(th+5),Sin(th+5)); glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glTexCoord2d(0,0);glVertex3d(0,0,0);
      glEnd();
   }

   r = .3;

   glTranslated(0,0,1);

   // Small knob at top sides
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),0);
      glVertex3d(r*Cos(th),r*Sin(th),.2);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),.2);
      glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }

   // Small knob at top top
   double h = .2;
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),h);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th),r*Sin(th),h);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),h);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th+5),r*Sin(th+5),h);
      glNormal3f(0,0,1);
      glTexCoord2d(0,0); glVertex3d(0,0,h);
      glEnd();
   }

   glPopMatrix();
   glPopMatrix();

   glPushMatrix();

   glTranslated(0,0,2.5);
   glRotated(60,1,0,0);

   r = .5;
   // Pole
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(0,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(1,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }

   // Pole Top
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),4);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),4);
      glTexCoord2d(Cos(th+5),Sin(th+5)); glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glNormal3f(0,0,1);
      glTexCoord2d(0,0); glVertex3d(0,0,4);
      glEnd();
   }
   glPopMatrix();
   glPushMatrix();
   glTranslated(0,-6.5,2.5);
   glRotated(-60,1,0,0);
   // Pole
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(0,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(1,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }

   // Pole Top
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),4);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th),r*Sin(th),4);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),4);
      glTexCoord2d(Cos(th+5),Sin(th+5)); glVertex3d(r*Cos(th+5),r*Sin(th+5),4);
      glNormal3f(0,0,1);
      glTexCoord2d(0,0); glVertex3d(0,0,4);
      glEnd();
   }

   glPopMatrix();

   // Connecting Joints
   sphere(0,-3.1,4.3,.7,.7,.7);
   sphere(0,-6.7,2.5,.7,.7,.7);

   glPushMatrix();
   glTranslated(0,-7.5,2.5);
   glRotated(-90,1,0,0);
   glColor3d(.8,.8,.8);

   //Base
   // Sides
   r = 2.5;
   h = .3;
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(0,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(1,th/90.0);glVertex3d(r*Cos(th),r*Sin(th),h);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glTexCoord2d(1,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),h);
      glTexCoord2d(0,(th+5)/90.0);glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glEnd();
   }

   // Top
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),h);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th),r*Sin(th),h);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),h);
      glTexCoord2d(Cos(th+5),Sin(th+5)); glVertex3d(r*Cos(th+5),r*Sin(th+5),h);
      glNormal3f(0,0,1);
      glTexCoord2d(0,0); glVertex3d(0,0,h+1);
      glEnd();
   }
   
   // Bottom
   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glTexCoord2d(Cos(th),Sin(th)); glVertex3d(r*Cos(th),r*Sin(th),0);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glTexCoord2d(Cos(th+5),Sin(th+5)); glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glTexCoord2d(0,0); glVertex3d(0,0,0);
      glEnd();
   }
   glPopMatrix();

   // Lightbulb
   glColor3d(1,1,.9);
   glDisable(GL_LIGHTING);
   sphere(0,.35,1,1.3,1.3,1.4);
   if(light)
      glEnable(GL_LIGHTING);


   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

/*
 *  Draw a rug
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the x axis
 */
static void rug(double x,double y,double z,
                 double dx,double dy,double dz, double th)
{
   glPushMatrix();

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[7]);

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(dx,dy,dz);
   glColor3d(1,1,1);

   double r = 1;

   for (int th=0;th<=360;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(r*Cos(th),r*Sin(th),0);
      glTexCoord2f(.5*Cos(th)+0.5,.5*Sin(th)+0.5); glVertex3d(r*Cos(th),r*Sin(th),0);
      glNormal3f(r*Cos(th+5),r*Sin(th+5),0);
      glTexCoord2f(.5*Cos(th+5)+0.5,.5*Sin(th+5)+0.5); glVertex3d(r*Cos(th+5),r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glTexCoord2d(0.5,0.5); glVertex3d(0,0,0);
      glEnd();
   }

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

/*
 *  Draw a room
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 */
static void room(double x, double y, double z, double dx, double dy, double dz)        
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z+dx);
   glScaled(dx,dy,dz);

   glColor3d(1,1,1);
   

   // left
   if ((th < 10 && th > -190) || th > 170)
   {  
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      glBegin(GL_QUADS);
      glNormal3d(1,0,0);
      glTexCoord2d(0,0); glVertex3d(0,0,0);
      glTexCoord2d(2,0); glVertex3d(0,1,0);
      glTexCoord2d(2,2); glVertex3d(0,1,-1);
      glTexCoord2d(0,2); glVertex3d(0,0,-1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
   }

   // right
   if((th > -15 && th < 195) || th < -165)
   {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      glBegin(GL_QUADS);
      glNormal3d(-1,0,0);
      glTexCoord2d(0,0); glVertex3d(1,0,0);
      glTexCoord2d(1,0); glVertex3d(1,1,0);
      glTexCoord2d(1,1); glVertex3d(1,1,-1);
      glTexCoord2d(0,1); glVertex3d(1,0,-1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
   }


   // front
   if((th < -80 && th > -280) || (th > 80 && th < 280))
   {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      glBegin(GL_QUADS);
      glNormal3d(0,0,-1);
      glTexCoord2d(0,1); glVertex3d(0,1,0);
      glTexCoord2d(1,1); glVertex3d(1,1,0);
      glTexCoord2d(1,0); glVertex3d(1,0,0);
      glTexCoord2d(0,0); glVertex3d(0,0,0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
   }
   

   // back
   if ((th > -100 && th < 100) || (th > 260) || (th < -260))
   {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      glBegin(GL_QUADS);
      glNormal3d(0,0,1);
      glTexCoord2d(0,1.5); glVertex3d(0,1,-1);
      glTexCoord2d(1.5,1.5); glVertex3d(1,1,-1);
      glTexCoord2d(1.5,0); glVertex3d(1,0,-1);
      glTexCoord2d(0,0); glVertex3d(0,0,-1);
      glEnd();
      glDisable(GL_TEXTURE_2D);
   }

   if (ph > 0)
   {
      // bottom
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[1]);
      glBegin(GL_QUADS);
      glNormal3d(0,1,0);
      glTexCoord2d(0,0); glVertex3d(0,0,0);
      glTexCoord2d(0,1.5); glVertex3d(0,0,-1);
      glTexCoord2d(1.5,1.5); glVertex3d(1,0,-1);
      glTexCoord2d(1.5,0); glVertex3d(1,0,0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
   }

   if (ph < 15)
   {
      // top
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      glBegin(GL_QUADS);
      glNormal3d(0,-1,0);
      glTexCoord2d(0,0); glVertex3d(0,1,0);
      glTexCoord2d(0,1); glVertex3d(0,1,-1);
      glTexCoord2d(1,1); glVertex3d(1,1,-1);
      glTexCoord2d(1,0); glVertex3d(1,1,0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
   }

   glPopMatrix();

}

/*
 *  Draw a flag
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 */
static void flag(double x, double y, double z, double dx, double dy, double dz)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z+dx);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glColor3d(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f(0,0,1);
   glTexCoord2d(0,0); glVertex3d(0,0,0);
   glTexCoord2d(0,1); glVertex3d(0,.5,0);
   glTexCoord2d(1,1); glVertex3d(1,.5,0);
   glTexCoord2d(1,0); glVertex3d(1,0,0);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();
}

/*
 *  Draw a window
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void window(double x, double y, double z, double dx, double dy, double dz, double th)
{

   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z+dx);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   glColor3d(1,1,1);
   glBegin(GL_QUADS);
   glNormal3f(-1,0,1);
   glTexCoord2d(0,0); glVertex3d(0,0,0);
   glTexCoord2d(0,1); glVertex3d(0,.5,0);
   glTexCoord2d(1,1); glVertex3d(1,.5,0);
   glTexCoord2d(1,0); glVertex3d(1,0,0);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   glPopMatrix();
}

/*
 *  Draw a desk
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void desk(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   
   glColor3d(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   // Desk top
   cuboid(0,1,0,3,.1,1.5,0,10,1);
   glColor3d(.1,.1,.1);
   // front left leg
   cuboid(0.15,.2,-0.08,.1,.8,.1,0,10,2);
   // front right leg
   cuboid(2.7,.2,-0.08,.1,.8,.1,0,10,2);
   // back left leg
   cuboid(0.15,.2,-1.3,.1,.8,.1,0,10,2);
   // back right leg
   cuboid(2.7,.2,-1.3,.1,.8,.1,0,10,2);

   glScaled(.9,1.1,.9);
   // Bottom tabletop
   cuboid(.15,.85,-.08,3,.1,1.5,0,10,2);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);  

}

/*
 *  Draw a shelf
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void shelf(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

   // Shelf
   glColor3d(.2,.2,.2);
   cuboid(0,0,0,2,.5,5,0,6,1);


   glDisable(GL_TEXTURE_2D);  
   glPopMatrix();

}

/*
 *  Draw a cushon
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the z axis
 */
static void cushon(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);

   double r = 1;
   double cx = 4;
   double cy= 1;
   // Half Cylinder Top
   for (int th=0;th<=175;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(cx*r*Cos(th),cy*r*Sin(th),2.5);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),2.5);
      glNormal3f(cx*r*Cos(th+5),cy*r*Sin(th+5),2.5);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),2.5);
      glNormal3f(0,0,1);
      glVertex3d(0,0,2.5);
      glEnd();
   }

   // Half Cylinder
   for (int th=0;th<=175;th+=5)
   {
      glBegin(GL_QUADS);
      glNormal3f(cx*r*Cos(th),cy*r*Sin(th),0);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),0);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),2.5);
      glNormal3f(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),2.5);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glEnd();
   }

   // Half Cylinder Bottom
   for (int th=0;th<=175;th+=5)
   {
      glBegin(GL_TRIANGLE_STRIP);
      glNormal3f(cx*r*Cos(th),cy*r*Sin(th),0);
      glVertex3d(cx*r*Cos(th),cy*r*Sin(th),0);
      glNormal3f(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glVertex3d(cx*r*Cos(th+5),cy*r*Sin(th+5),0);
      glNormal3f(0,0,-1);
      glVertex3d(0,0,0);
      glEnd();
   }

   // Bottom
   glBegin(GL_QUADS);
   glNormal3f(0,-1,0);
   glVertex3d(cx*r*Cos(180),cy*r*Sin(180),0);
   glVertex3d(cx*r*Cos(0),cy*r*Sin(0),0);
   glVertex3d(cx*r*Cos(0),cy*r*Sin(0),2.5);
   glVertex3d(cx*r*Cos(180),cy*r*Sin(180),2.5);
   glEnd();


   glDisable(GL_TEXTURE_2D);  
   glPopMatrix(); 
}

/*
 *  Draw a chair
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void chair(double x, double y, double z, double dx, double dy, double dz, double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   // Back rest
   cushon(0,0,0,0.2,0.25,.6,0);
   // Bottom cushon
   cushon(1.1,0.97,0,0.25,0.25,.6,75);
   // Leg 1
   cuboid(.8,-1,0,.1,1,.1,180,5,2);
   // Leg 2
   cuboid(-.7,-1,0,.1,1,.1,180,5,2);
   // Leg 3
   cuboid(-.7,-1,1.4,.1,1,.1,180,5,2);
   // Leg 4
   cuboid(.8,-1,1.4,.1,1,.1,180,5,2);


   glPopMatrix();


}

/*
 *  Draw a baseball
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the z axis
 */
static void baseball(double x, double y, double z, double dx, double dy, double dz, double th)
{
   //  Save transformation
   glPushMatrix();

   // Scale, rotate, translate
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glRotated(-90,1,0,0);
   glScaled(dx,dy,dz);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texture[9]);

   glColor3d(1,1,1);

   // Draw a sphere
   for (int ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=5)
      {
         Vertex(th,ph);
         Vertex(th,ph+15);
      }
      glEnd();
   }


   glDisable(GL_TEXTURE_2D);  
   glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //glClearColor(.67,.84,.9,0);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();

   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);


   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
      //  Translate intensity to color vectors
      float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
      float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
      float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
      //  Light position
      float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2],  0.1);
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  Location of viewer for specular calculations
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      //  Enable light 0
      glEnable(GL_LIGHT0);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
      glDisable(GL_LIGHTING);

   // Scene switches
   switch(mode)
   {
      case 1:
         glColor3d(.3,.3,.3);
         chair(0,0,0,1,1,1,180);
         break;
      case 2:
         luxo(0,1,0,.2,.2,.2,-90);
         break;
      case 3:
         nightstand(0,0,0,.7,.7,.7,0);
         break;
      case 4:
         dresser(0,0,0,.5,.5,.5,0);
         break;
      case 5:
         bed(0,0,0,.5,.5,.5,-90);
         break;
      case 6:
         lamp(0,0,0,.5,.5,.5);
         break;
      case 7:
         penguin(0,-.5,0,1,1,1,0,0,0,1);
         break;
      case 8:
         baseball(0,0,0,1,1,1,90);
         break;
      case 9:
         desk(-1.5,-.5,0,1,1,1,0);
         break;
      case 10:
         door(0,0,0,.5,.5,.5,0);
         break;
   }
   
   if (mode == 0)
   {
      glPushMatrix();

      // Offset these first objects (started working before shifting the room over)
      glTranslated(-1.5,0,-1.5);

      // Draw the walls
      room(0,0,0,3,1.8,3);

      // Draw a dresser
      dresser(1.0,0,3.7,.27,.27,.27,180);

      // Draw 2 nightstands
      nightstand(1.5,0,0.25,.25,.25,.25,0);
      nightstand(.25,0,.25,.25,.25,.25,0);

      // Draw 2 lamps
      lamp(2,.6,0.17,.05,.05,.05);
      lamp(0.4,.6,0.17,.05,.05,.05);
      
      // Draw a bed
      bed(.7,0,.1,.3,.3,.3,-90);

      // If it should be visible, draw a flag
      if (((th > -100 && th < 100) || (th > 260) || (th < -260)))
      {
         flag(.725,.5,-.999,1,1,1);
      }

      // If it should be visible, draw the window
      if ((th < 10 && th > -190) || th > 170)
      {
         // Window should not have lighting
         glDisable(GL_LIGHTING);
         window(0.01,0.1,0.1,2.8,3,2.8,90);
         // If lighting is enabled, draw everything else with lighting
         if (light)
            glEnable(GL_LIGHTING);
         glPopMatrix();
         glColor3d(.5,.5,.5);

         // Draw the blinds
         cuboid(-1.5,1.6,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,1.5,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,1.4,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,1.3,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,1.2,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,1.1,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,1,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.9,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.8,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.7,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.6,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.5,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.4,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.3,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.2,1.4,.05,0.01,2.8,0,5,2);
         cuboid(-1.5,.1,1.4,.05,0.01,2.8,0,5,2);

      }
      else
      {
         glPopMatrix();
      }
      
      // Draw a luxo lamp
      luxo(1.3,.75,-.6,0.04,0.04,0.04,140);

      // Draw a desk
      desk(1.05,-.1,-0.9,.4,.5,.3,-90);

      // If it should be visible, draw the door
      if((th < -80 && th > -280) || (th > 80 && th < 280))
      {
        door(1,0,1.46,.3,.3,.3,180);
      }


      glPushMatrix();
      glTranslated(0,-.2,0);

      // Draw 2 shelves
      shelf(-.1,1,1.5,.1,.1,.1,90);
      shelf(.4,.7,1.5,.1,.1,.1,90);

      // Draw 3 penguins
      penguin(-.2,1.12,1.4,.1,.1,.1,210,.1,.1,1);
      penguin(.25,.8,1.4,.07,.07,.07,180,.1,1,.1);
      penguin(.05,.79,1.4,.06,.06,.06,150,1,.1,.1);
      
      // Draw a baseball
      baseball(-.45,1.1,1.4,.06,.06,.06,90);
      glPopMatrix();


      glDisable(GL_TEXTURE_2D);

      // If it should be visible, draw a rug
      if (ph > 0)
         rug(.3,0.001,.65,.8,.8,.8,90);

      glColor3d(.15,.15,.15);
      // Draw a chair
      chair(.8,.2,-.2,.2,.2,.2,150);

   }

   // Draw axes - no more lighting from here on out
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0;  //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Light=%s",
     th,ph,dim,fov,light?"On":"Off");

   glWindowPos2i(5,45);

   // Print the name of the scene we are viewing
   switch(mode)
   {
      case 0:
         Print("Scene=Main");
         break;
      case 1:
         Print("Scene=Chair");
         break;
      case 2:
         Print("Scene=Luxo");
         break;
      case 3:
         Print("Scene=Nightstand");
         break;
      case 4:
         Print("Scene=Dresser");
         break;
      case 5:
         Print("Scene=Bed");
         break;
      case 6:
         Print("Scene=Lamp");
         break;
      case 7:
         Print("Scene=Penguin");
         break;
      case 8:
         Print("Scene=Baseball");
         break;
      case 9:
         Print("Scene=Desk");
         break;
      case 10:
         Print("Scene=Door");
         break;
   }

   if (light)
   {
      glWindowPos2i(5,25);
      Print("Light Distance=%d Light Elevation=%.1f",distance,ylight);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
   {
      ph += 5;
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
   {
      ph -= 5;
   }
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   // Reproject
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 't')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   // Chance scene
   else if (ch == 'c' || ch == 'C')
      mode+=1;
      if (mode > 10)
         mode = 0;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Reproject
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(fov,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{

   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Tristan English - FINAL PROJECT");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);

   glutIdleFunc(idle);
   //  Load textures
   
   texture[0] = LoadTexBMP("flag.bmp");
   texture[1] = LoadTexBMP("floor.bmp");
   texture[2] = LoadTexBMP("wall.bmp");
   texture[3] = LoadTexBMP("window.bmp");
   texture[4] = LoadTexBMP("wood.bmp");
   texture[5] = LoadTexBMP("sheets.bmp");
   texture[6] = LoadTexBMP("metal.bmp");
   texture[7] = LoadTexBMP("rug.bmp");
   texture[8] = LoadTexBMP("door.bmp");
   texture[9] = LoadTexBMP("baseball.bmp");
   texture[10] = LoadTexBMP("desk.bmp");
   texture[11] = LoadTexBMP("fabric.bmp");
   
   ErrCheck("init");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
