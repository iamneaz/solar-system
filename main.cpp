#include<stdio.h>
#include<stdlib.h>
#include<math.h>


#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int bullet=0;
int drawgrid;
int drawaxes;
double angle;
double angle_c_z=0;
double angle_c_x=0, angle_t_x=0, angle_t_z=0;
double limit=0.0;
struct point
{
	double x,y,z;
};

struct point pos,l,u,r;
double X=0,Z=0;
int Y=-1;
struct point arra[100];
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
		    glColor3f(0.0f,1.0f,0.0f);
			glVertex3f( 1000,0,0);
			glVertex3f(-1000,0,0);

            glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(0,-1000,0);
			glVertex3f(0, 1000,0);

            glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(0,0, 1000);
			glVertex3f(0,0,-1000);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}

void draw_circle_line(double radius,int segments)
{
  int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_LINES);
    {
        for(i=0;i<segments;i++)
        {

            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();
}
void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    glBegin(GL_POLYGON);
    {
        for(i=0;i<segments;i++)
        {

            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);

        }
    }
        glEnd();
}

void draw_cylinder(double radius,double height,int segments)
{
    int i;
     struct point points[2][100];
     for(i=0;i<=segments;i++)
    {
        points[0][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[0][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[0][i].z=0;
        points[1][i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[1][i].y=radius*sin(((double)i/(double)segments)*2*pi);
        points[1][i].z=height;
    }

    for (i=0;i<segments;i++)
    {
        glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f((i)%2,.5,(i)%2);
				glVertex3f(points[0][i].x,points[0][i].y,points[0][i].z);
				glVertex3f(points[0][i+1].x,points[0][i+1].y,points[0][i+1].z);
				glVertex3f(points[1][i+1].x,points[1][i+1].y,points[1][i+1].z);
				glVertex3f(points[1][i].x,points[1][i].y,points[1][i].z);

			}glEnd();


    }
}
void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<int(segments-limit);i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,0,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(i/(double)stacks,1,i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void quad()
{
  // Draw A Quad
        glBegin(GL_QUADS);

            glColor3f(0.0f,1.0f,0.0f);
            glVertex3f(1.0f,1.0f,8.0f);					    // Top front Of The Quad (right)
            glVertex3f(1.0f,1.0f,1.0f);					// Top back Of The Quad (right)
            glVertex3f(1.0f,-1.0f,1.0f);					// Bottom back Of The Quad (right)
            glVertex3f(1.0f,-1.0f,8.0f);					// Bottom front Of The Quad (right)

            glColor3f(1.0f,1.0f,0.0f);
            glVertex3f(-1.0f,1.0f,8.0f);					// Top front Of The Quad (left)
            glVertex3f(-1.0f,1.0f,1.0f);					// Top back Of The Quad (left)
            glVertex3f(-1.0f,-1.0f,1.0f);					// Bottom back Of The Quad (left)
            glVertex3f(-1.0f,-1.0f,8.0f);					// Bottom front Of The Quad (left)


            glColor3f(1.0f,0.0f,1.0f);
            glVertex3f(1.0f,1.0f,8.0f);					    // Right front Of The Quad (top)
            glVertex3f(1.0f,1.0f,1.0f);					    // Right back Of The Quad (top)
            glVertex3f(-1.0f,1.0f,1.0f);				    // Left back Of The Quad (top)
            glVertex3f(-1.0f,1.0f,8.0f);				    // Left front Of The Quad (top)


            glColor3f(0.0f,1.0f,1.0f);
            glVertex3f(1.0f,-1.0f,8.0f);					// Right front Of The Quad (bottom)
            glVertex3f(1.0f,-1.0f,1.0f);					// Right back Of The Quad (bottom)
            glVertex3f(-1.0f,-1.0f,1.0f);				    // Left back Of The Quad (bottom)
            glVertex3f(-1.0f,-1.0f,8.0f);				    // Left front Of The Quad (bottom)


            glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(1.0f,1.0f,1.0f);					 // Top Right Of The Quad (Back)
            glVertex3f(-1.0f,1.0f,1.0f);					// Top Left Of The Quad (Back)
            glVertex3f(-1.0f,-1.0f,1.0f);					// Bottom Left Of The Quad (Back)
            glVertex3f(1.0f,-1.0f,1.0f);					// Bottom Right Of The Quad (Back)


            glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(1.0f,1.0f,8.0f);					    // Top Right Of The Quad (Front)
            glVertex3f(-1.0f,1.0f,8.0f);					// Top Left Of The Quad (Front)
            glVertex3f(-1.0f,-1.0f,8.0f);					// Bottom Left Of The Quad (Front)
            glVertex3f(1.0f,-1.0f,8.0f);			// Bottom Right Of The Quad (Front)

        glEnd();
}

void drawSun(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1,1,i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawMercury(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(.8,.8,.8);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawVenus(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1,1,1);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawEarth(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		if(i>10)
		{
			glColor3f(0,i/(double)stacks,1);
			
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
		}
		else
		{
			glColor3f(i/(double)stacks,1,i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
		}
		
        glColor3f(i/(double)stacks,i/(double)stacks,1);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawMars(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		if(i>5 || i<stacks-5)
		{
				glColor3f(1,.2,i/(double)stacks);
				for(j=0;j<slices;j++)
				{
					glBegin(GL_QUADS);{
						//upper hemisphere
						glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
						glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
						glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
						glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
						//lower hemisphere
						glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
						glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
						glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
						glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
					}glEnd();
				}
		}
		else
		{
			 glColor3f(.8,.8,.8);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
		}
		
       
	}
}

void drawJupiter(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        
		if(i>5 && i<10)
		{
			glColor3f(.64,.16,.16);

			for(j=0;j<slices;j++)
			{
				glBegin(GL_QUADS);{
					
					//upper hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
					//lower hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
				}glEnd();
			}
		}
		else if(i>15 && i<20)
		{
			glColor3f(.64,.16,.16);
			for(j=0;j<slices;j++)
			{
				glBegin(GL_QUADS);{
					
					//upper hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
					//lower hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
				}glEnd();
			}
		}
		else if(i>25 && i<30)
		{
			glColor3f(.64,.16,.16);
			for(j=0;j<slices;j++)
			{
				glBegin(GL_QUADS);{
					
					//upper hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
					//lower hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
				}glEnd();
			}
		}
		else
		{
			glColor3f(1,1,1);
			for(j=0;j<slices;j++)
			{
				glBegin(GL_QUADS);{
					
					//upper hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
					//lower hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
				}glEnd();
			}
		}
		
		
		
	}
}

void drawSaturnRings()
{
	int i;
	for(i=30;i<50;i+=2)
	{
		draw_circle_line(i,40);
	}
	
}

void drawSaturn(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{

		if(i<10)
		{
			glColor3f(1,1,i/(double)stacks);
			for(j=0;j<slices;j++)
			{
				glBegin(GL_QUADS);{
					//upper hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
					//lower hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
				}glEnd();
			}
		}
		else
		{
			glColor3f(1,1,1);
			for(j=0;j<slices;j++)
			{
				glBegin(GL_QUADS);{
					//upper hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
					//lower hemisphere
					glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
					glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
					glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
					glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
				}glEnd();
			}
		}
		
        
	}
}

void drawUranus(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(i/(double)stacks,i/(double)stacks,.3);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawNeptune(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(i/(double)stacks,i/(double)stacks,.5);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawPluto(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(i/(double)stacks,i/(double)stacks,i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void movement3D()
{
    //draw_circle_line(110,40);
    //glColor3f(1,0,0);
    drawSun(40,30,30);

    // glPushMatrix();
    // glRotatef(angle,0,0,1);
    // glTranslatef(110,0,0);
    // glRotatef(2*angle,0,0,1);
    // glColor3f(0,1,0);
    // drawSphere(15,30,30);
    // glPopMatrix();

	//Mercury
    glPushMatrix();
	//glColor3f(1, 0, 0);
    //glColor3f(0.8, 0.8, 0.8);
    glRotatef(angle*47.87,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(80,0,0);
    glRotatef(2*angle,0,0,1);
    //glColor3f(1, 0, 0);
    drawMercury(10,30,30);
    glPopMatrix();

	//Venus
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*35.02,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(130,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawVenus(13,30,30);
    glPopMatrix();

	//Earth
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*29.78,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(190,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawEarth(15,30,30);
    glPopMatrix();

	//Mars
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*24.077,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(270,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawMars(18,30,30);
    glPopMatrix();

	//Jupiter
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*13.07,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(370,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawJupiter(30,30,30);
    glPopMatrix();

	//Saturn
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*9.69,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(480,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
	drawSaturnRings();
    drawSaturn(25,30,30);
    glPopMatrix();

	//Uranus
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*6.81,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(550,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawUranus(15,30,30);
    glPopMatrix();

	//Neptune
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*5.43,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(620,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawNeptune(14,30,30);
    glPopMatrix();

	//Pluto
    glPushMatrix();
    glColor3f(1,0,1);
    glRotatef(angle*3,0,0,1);
    //draw_circle_line(110,40);
    glTranslatef(680,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawPluto(10,30,30);
    glPopMatrix();

}
void drawSS()
{
    // draw_circle_line(110,40);
    // glColor3f(1,0,0);
    // drawSphere(20,30,30);

    // glRotatef(angle,0,0,1);
    // glTranslatef(110,0,0);
    // glRotatef(2*angle,0,0,1);
    // glColor3f(0,1,0);
    // drawSphere(15,30,30);;

    // glPushMatrix();
    // {
    //     glRotatef(angle,0,0,1);
    //     glTranslatef(60,0,0);
    //     glRotatef(2*angle,0,0,1);
    //     glColor3f(0,0,1);
    //     drawSphere(10,30,30);;
    // }
    // glPopMatrix();
    // glColor3f(1,1,1);

    /*glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);*/
}

void keyboardListener(unsigned char key, int xx,int yy){
    double x,y,z;
    double rate = 0.01;
	switch(key){

		case '1':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+1*r.x*sin(rate);
			l.y = l.y*cos(rate)+r.y*sin(rate);
			l.z = l.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-x*sin(rate);
			r.y = r.y*cos(rate)-y*sin(rate);
			r.z = r.z*cos(rate)-z*sin(rate);}
			break;
        case '2':

			{
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+r.x*sin(-rate);
			l.y = l.y*cos(-rate)+r.y*sin(-rate);
			l.z = l.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-x*sin(-rate);
			r.y = r.y*cos(-rate)-y*sin(-rate);
			r.z = r.z*cos(-rate)-z*sin(-rate);
			}
			break;
        case '3':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '4':
            x=l.x;y=l.y;z=l.z;
			l.x = l.x*cos(-rate)+1*u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case '6':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(rate)+u.x*sin(rate);
			r.y = r.y*cos(rate)+u.y*sin(rate);
			r.z = r.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-x*sin(rate);
			u.y = u.y*cos(rate)-y*sin(rate);
			u.z = u.z*cos(rate)-z*sin(rate);
			break;
        case '5':
            x=r.x;y=r.y;z=r.z;
			r.x = r.x*cos(-rate)+u.x*sin(-rate);
			r.y = r.y*cos(-rate)+u.y*sin(-rate);
			r.z = r.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-x*sin(-rate);
			u.y = u.y*cos(-rate)-y*sin(-rate);
			u.z = u.z*cos(-rate)-z*sin(-rate);
			break;
        case 'q':
            if(angle_c_z<45){
                angle_c_z+=1.5;
            }
            break;
        case 'w':
            if(angle_c_z>-45){
                angle_c_z-=1.5;
            }
            break;
        case 'e':
            if(angle_c_x<45){
                angle_c_x+=1.5;
            }
            break;
        case 'r':
            if(angle_c_x>-45){
                angle_c_x-=1.5;
            }
            break;
        case 'a':
            if ( angle_t_x<45){
                    angle_t_x+=1.5;
            }
            break;
        case 's':
            if ( angle_t_x>-45){
                    angle_t_x-=1.5;
            }
            break;
         case 'd':
            if ( angle_t_z<45){
                    angle_t_z+=1.5;
            }
            break;
        case 'f':
            if ( angle_t_z>-45){
                    angle_t_z-=1.5;
            }
            break;



		default:
			break;
	}

}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_LEFT :
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_RIGHT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				/*bullet =1;
				double length = 649;
				//double length = (649/cos(angle_c_x*pi/180))/cos(angle_t_x*pi/180);
                printf("---> %lf\n",length);
                //X = length* sin(angle_c_z*pi/180);
                X= length* tan(angle_c_z*pi/180);

                Z=-100*sin(angle_c_x*pi/180)-(649-100*cos(angle_c_x*pi/180))*tan((angle_t_x+angle_c_x)*pi/180);*/



			}
			break;


		case GLUT_RIGHT_BUTTON:
		    if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
				//(600/cos(angle_c_x*pi/180))/cos(angle_t_x*pi/180);
			}
			break;
			//........

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+l.z,	u.x,u.y,u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	//drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    //drawSS();
    movement3D();
    //drawSphere(50,20,10);
    //quad();
    //drawCone(5,10,50);
    //draw_cylinder(10,10,30);

    //draw_cylinder(2,500,segment);
    //100*sin(angle_c_x)+(500-100*cos(angle_c_x))*sin(angle_t_x);


    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//display();
	angle+=.1;
	//codes for any changes in Models, Camera
	limit+=0.01;
	if(limit>=50.0)
    {
        limit=0;
    }
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	//cameraHeight=150.0;
	//cameraAngle=1.0;
	//angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	5000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
    pos.x=0;
    pos.y=0;
    pos.z=-900;
    l.x=0;u.x=-1;r.x=0;
    l.y=0;u.y=0;r.y=1;
    l.z=1;u.z=0;r.z=0;
	glutInit(&argc,argv);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}





















/*
void drawSphere(double radius,int slices,int stacks,int up)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		if(up==3){
                r= 2*radius- radius*cos(((double)i/(double)stacks)*(pi/2));
		}else{
		     r=  radius*cos(((double)i/(double)stacks)*(pi/2));
		}
		for(j=0;j<=slices;j++)
		{
		    if(up!=3){
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		    }
		    else{
            points[i][j].x= r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=(r*sin(((double)j/(double)slices)*2*pi));
			points[i][j].z=h;

		    }
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f(j%2,j%2,j%2);
			    if(up==1 or up==-1 or up==3)
                {
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                }
                //lower hemisphere
                if(up==0 or up==-1)
                {

                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }


			}glEnd();
		}
	}
}


void offline()
{

    glBegin(GL_QUADS);{
        //upper hemisphere
        glColor3f(.1,.1,.1);
        glVertex3f(200,-650,200);
        glVertex3f(200,-650,-200);
        glVertex3f(-200,-650,-200);
        glVertex3f(-200,-650,200);

    }glEnd();


    if(bullet==1 && abs(X)<200 && abs(Z)<200){
    Y++;
    arra[Y].x=X;
    arra[Y].z = Z;
    printf("%lf %lf %lf %lf\n",X,Z,angle_c_x,angle_c_z);

	bullet =0;

    }
    int j;
    for(j=0;j<=Y;j++){
        glColor3f(1.0,0.0,0.0);

            glBegin(GL_QUADS);{
            glVertex3f(arra[j].x+5 ,-649, arra[j].z+5);
            glVertex3f(arra[j].x+5,-649, arra[j].z-5);
            glVertex3f(arra[j].x-5,-649,arra[j].z-5 );
            glVertex3f(arra[j].x-5, -649, arra[j].z+5);
        }glEnd();
    }

    int segment =30;
    glRotatef(angle_c_z,0,0,1);
    glRotatef(angle_c_x,1,0,0);
    glRotatef(90,1,0,0);
    glTranslatef(0,0,20);
    drawSphere(20,segment,segment,0);
    draw_cylinder(20,100,segment);
    glTranslatef(0,0,100);
    drawSphere(20,segment,segment,1);
    double r,l;
    r=15;
    l=170;
    glTranslatef(0,0,20);

    glRotatef(angle_t_x,1,0,0);
    glRotatef(angle_t_z,0,0,1);

    glTranslatef(0,0,r);
    drawSphere(r,segment,segment,0);
    draw_cylinder(r,l,segment);
    glTranslatef(0,0,l);
    drawSphere(r,segment,segment,3);
}
*/
