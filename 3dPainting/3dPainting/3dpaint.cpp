
#include <stdlib.h>
#include<glui.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

enum Objects
{
	SPHERE,
	BOX,
	CONE,
	TORUS,
	DODECAHEDRON,
	OCTAHEADRON,
	TETRAHEDRON,
	ICOSAHEDRON,
	TEAPOT
};
enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};


#define XMIN -500
#define XMAX 500
#define YMIN -500
#define YMAX 500
#define ZMIN -500
#define ZMAX 500
GLuint	Lists[10];
bool select=0,switchTranslate=0;
GLsizei wh=500,ww=500;
GLfloat pos_x = 0, pos_y = 0;
int objno=0,mainWindow;
int	WhichColor,changeColor=0;
float vx=0.0f,vy=0.0f,vz=5.0f,vcx=0.0f,vcy=0.0f,vcz=-50.0f,ovcx=0.0f,ovcy=0.0f,ovcz=0.0f,Scale2=1.0;
float xrot=0.0,yrot=0.0,angle = 0.0f,lx=0.0f,lz=-1.0f;
GLfloat	RotMatrix[4][4];
GLUI *Glui;
// the color definitions:
// this order must match the radio button order

const GLfloat Colors[8][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};
struct oblist {
  int type;
  int obi;
  float xc;
  float yc;
  float zc;
  float R,G,B;
  float setp;
  float scale;
  struct oblist *next;
  struct oblist *prev;
  float	ObjRotMatrix[4][4];
  float x[25],y[25],z[25];
  int index;
};
 struct oblist *obj;  
 struct oblist *tempobj;

    /* Now root points to a node struct */
void InitLists( void );
void draw_reference_room();
void checkLimits(float x,float y,float z);
void Reset( void );
void lightsOn();
void DrawGrid();
void translateObj();
void display (void) {
	int len=0,i=0,j=0;
	oblist *listobjs;
	listobjs=obj;
	
	
    glClearColor (1.0,1.0,1.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	//gluLookAt(vx,vy,vz,vcx,vcy,vcz,0,1,0);
	//glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on teh  x-axis (left and right)
    //glRotatef(yrot,0.0,1.0,0.0); 
	
    gluLookAt(vx,vy,vz,vx+lx,vy,vz+lz,0,1,0);
	
	//glMultMatrixf( (const GLfloat *) RotMatrix );
   /*  
    glTranslatef(0,0,-25);
    glutSolidTeapot(5.0);*/
	glTranslatef(0,0,-25);
	glPushMatrix();
	draw_reference_room();
	glPopMatrix();
	if(switchTranslate)
	{
		glPushMatrix();
		DrawGrid();
		glPopMatrix();
		glPushMatrix();
		 glLineWidth(4.0);
		  while ( listobjs->next != 0)
		  {
		     if(listobjs->index!=0)
				 {
					 int myindex=0;
					 //gluLookAt(0,0,0,0,0,-50,0,1,0);
					 glColor3f(0.0,1.0,0.0);
					for(myindex=0;myindex<listobjs->index-1;myindex++)
					{
					 glBegin(GL_LINES);
					    glVertex3f(listobjs->x[myindex],listobjs->y[myindex],listobjs->z[myindex]);
						glVertex3f(listobjs->x[myindex+1],listobjs->y[myindex+1],listobjs->z[myindex+1]);
					 glEnd();
					}
				 }
			 listobjs=listobjs->next;
		  }
		  if(listobjs->index!=0)
				 {
					 int myindex=0;
					 //gluLookAt(0,0,0,0,0,-50,0,1,0);
					 glColor3f(0.0,1.0,0.0);
					for(myindex=0;myindex<listobjs->index-1;myindex++)
					{
					 glBegin(GL_LINES);
					    glVertex3f(listobjs->x[myindex],listobjs->y[myindex],listobjs->z[myindex]);
						glVertex3f(listobjs->x[myindex+1],listobjs->y[myindex+1],listobjs->z[myindex+1]);
					 glEnd();
					}
				 }
			 listobjs=listobjs->next;
		glLineWidth(1.0);
		glPopMatrix();
	}
	listobjs=obj;
	//glColor3f(1.0,0.0,0.0);
	//lightsOn();
	if ( listobjs != 0 ) {
        while ( listobjs->next != 0)
        {
			
			
            glPushMatrix();
			
			glTranslatef(listobjs->xc,listobjs->yc,listobjs->zc);
			glScalef( (GLfloat)listobjs->scale, (GLfloat)listobjs->scale, (GLfloat)listobjs->scale );
			glColor3f(listobjs->R,listobjs->G,listobjs->B);
			glMultMatrixf( (const GLfloat *) listobjs->ObjRotMatrix );
			if(len==objno && select==1)
			{
				
				//glMultMatrixf( (const GLfloat *) RotMatrix );
				for(i=0;i<4;i++)
					for(j=0;j<4;j++)
				listobjs->ObjRotMatrix[i][j]=RotMatrix[i][j];

				listobjs->scale=Scale2;
				glColor3f(0.0,1.0,0.0);
				//glColor3fv( Colors[WhichColor] );
				if(changeColor){
				listobjs->R=Colors[WhichColor][0];
				listobjs->G=Colors[WhichColor][1];
				listobjs->B=Colors[WhichColor][2];
				changeColor=0;
				select=0;
				objno=0;
				Reset();
	
	            Scale2=1.0f;
	            Glui->sync_live();
	            glutSetWindow( mainWindow );
				glutPostRedisplay();
				}
			}
			glCallList( Lists[listobjs->type] );
			
			glPopMatrix();
            listobjs = listobjs->next;
			len++;
        }
    }
	//gluLookAt(0,0,0,0,0,-25,0,1,0);
     
	       
           glPushMatrix();
		   
			glTranslatef(listobjs->xc,listobjs->yc,listobjs->zc);
			glColor3f(listobjs->R,listobjs->G,listobjs->B);
			glMultMatrixf( (const GLfloat *) listobjs->ObjRotMatrix );
			glScalef( (GLfloat)listobjs->scale, (GLfloat)listobjs->scale, (GLfloat)listobjs->scale );
			
			if(len==objno && select==1)
			{
				//glMultMatrixf( (const GLfloat *) RotMatrix );
				for(i=0;i<4;i++)
					for(j=0;j<4;j++)
				listobjs->ObjRotMatrix[i][j]=RotMatrix[i][j];
				listobjs->scale=Scale2;
				glColor3f(0.0,1.0,0.0);
				//glColor3fv( Colors[WhichColor] );
				if(changeColor){
				listobjs->R=Colors[WhichColor][0];
				listobjs->G=Colors[WhichColor][1];
				listobjs->B=Colors[WhichColor][2];
				changeColor=0;
				select=0;
				objno=0;
				Reset();
	
	            Scale2=1.0f;
	            Glui->sync_live();
	            glutSetWindow( mainWindow );
				
				glutPostRedisplay();
				}
			}
			glCallList( Lists[listobjs->type] );
			glPopMatrix();
          //  glDisable(GL_LIGHTING);
	glutSwapBuffers();

}

void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
    glMatrixMode (GL_MODELVIEW);
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);
	wh=h;
	ww=w;
}

void lightsOn()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void checkLimits(float *x,float *y,float *z)
{
	if(*x<XMIN)*x=XMIN;
	if(*x>XMAX)*x=XMAX;
	if(*y<YMIN)*y=YMIN;
	if(*y>YMAX)*y=YMAX;
	if(*z<ZMIN)*z=ZMIN;
	if(*z>ZMAX)*z=ZMAX;


}
void keyboard (unsigned char key, int x, int y)
{
	oblist *dobjs;
	//tempobj->next= (struct oblist *) malloc( sizeof(struct oblist) );
	dobjs=obj;
	
	int STEP=1.0,i=0,j=0;
	float fraction = 5.0f;
	switch(key)
	{
	case 'w':vx += lx * fraction;
              vz += lz * fraction;
		break;
	case 's':vx -= lx * fraction;
             vz -= lz * fraction;

		break;
	case 'a':        angle -= 0.06f;
                     lx = sin(angle);
                     lz = -cos(angle);
                     break;
	case 'd':angle += 0.06f;
                     lx = sin(angle);
                     lz = -cos(angle);

		break;
	case 'l':if(switchTranslate)
			 {
				 for(i=0;i<objno;i++)dobjs=dobjs->next;
				 dobjs->x[dobjs->index]=vx;
				 dobjs->y[dobjs->index]=vy;
				 dobjs->z[dobjs->index]=vz;
				 
				 dobjs->index=dobjs->index+1;
				 ovcx=vx;
				 ovcy=vy;
				 ovcz=vz;
			 }
			 break;
	case 'q':vy +=STEP;
		break;
	case 'e':vy -=STEP;
		break;
	case 'n':if(select)objno++;
		for(i=0;i<objno;i++)dobjs=dobjs->next;
		for(i=0;i<4;i++)
					for(j=0;j<4;j++)
				RotMatrix[i][j]=dobjs->ObjRotMatrix[i][j];
		 Scale2=dobjs->scale;
		//Reset();
		break;
	case 'm':if(select)objno--;
		for(i=0;i<objno;i++)dobjs=dobjs->next;
		for(i=0;i<4;i++)
					for(j=0;j<4;j++)
				RotMatrix[i][j]=dobjs->ObjRotMatrix[i][j];
		Scale2=dobjs->scale;
		//Reset();
		break;
	case 'i':translateObj();
		break;
	case 'r':if(select)
			 {
				 for(i=0;i<objno;i++)dobjs=dobjs->next;
	
				 if(i==0){
					 if(dobjs->next==0)
					 {
					 dobjs->next= (struct oblist *) malloc( sizeof(struct oblist) );
					 dobjs=dobjs->next;
					 dobjs->type = 0;
	                 dobjs->xc = 0;
	                 dobjs->yc = 0;
	                 dobjs->zc = -25;
                     dobjs->prev=0;
	                 dobjs->next= 0;
					 obj=dobjs;
					 }
					 else
					 {
						 obj=dobjs->next;
					 }
					 //select=0;
				 }
				 else{
					 if(dobjs->next==0)
					 {
						 dobjs=dobjs->prev;
					     dobjs->next=0;
					 }
					 else{
					 (dobjs->prev)->next=dobjs->next;
					 dobjs=dobjs->prev;
					 }
				 while((dobjs->prev)!=0)dobjs=dobjs->prev;
				 obj=dobjs;
				 }
			
				 select=0;
				 objno=0;

			 }
			 break;
     checkLimits(&vx,&vy,&vz);


	}
    glutPostRedisplay();

}
void Lightson()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void translateObj()
{
	//int i=0;
	printf(" from idle\n");
	float lxc=0.0,lyc=0.0,lzc=0.0;
	struct oblist *listobjs1;
	listobjs1=obj;
	if ( listobjs1 != 0  ) {
        while ( listobjs1->next != 0)
        {
			if(listobjs1->index!=0)
			{
				if(listobjs1->setp>=1.0f)
				{
					listobjs1->setp=0.0f;
					listobjs1->obi++;
					if(listobjs1->obi>=listobjs1->index)listobjs1->obi=0;
				}
				//x3 = (x2 - x1)·t + x1
                lxc=(listobjs1->x[listobjs1->obi+1]-listobjs1->x[listobjs1->obi])*listobjs1->setp+listobjs1->x[listobjs1->obi];
				lyc=(listobjs1->y[listobjs1->obi+1]-listobjs1->y[listobjs1->obi])*listobjs1->setp+listobjs1->y[listobjs1->obi];
				lzc=(listobjs1->z[listobjs1->obi+1]-listobjs1->z[listobjs1->obi])*listobjs1->setp+listobjs1->z[listobjs1->obi];
				listobjs1->xc=lxc;
				listobjs1->yc=lyc;
				listobjs1->zc=lzc;
				listobjs1->setp=listobjs1->setp+0.02f;
			}
			listobjs1=listobjs1->next;
		}
		if(listobjs1->index!=0)
			{
				if(listobjs1->setp>=1.0f)
				{
					listobjs1->setp=0.0f;
					listobjs1->obi++;
					if(listobjs1->obi>=listobjs1->index)listobjs1->obi=0;
				}
				//x3 = (x2 - x1)·t + x1
                lxc=(listobjs1->x[listobjs1->obi+1]-listobjs1->x[listobjs1->obi])*listobjs1->setp+listobjs1->x[listobjs1->obi];
				lyc=(listobjs1->y[listobjs1->obi+1]-listobjs1->y[listobjs1->obi])*listobjs1->setp+listobjs1->y[listobjs1->obi];
				lzc=(listobjs1->z[listobjs1->obi+1]-listobjs1->z[listobjs1->obi])*listobjs1->setp+listobjs1->z[listobjs1->obi];
				listobjs1->xc=lxc;
				listobjs1->yc=lyc;
				listobjs1->zc=lzc;
				listobjs1->setp=listobjs1->setp+0.02f;
			}
			listobjs1=listobjs1->next;
	}
	glutPostRedisplay();

}
void main_menu(int index){
	int i=0,j=0;
	oblist *dobjs;
	//tempobj->next= (struct oblist *) malloc( sizeof(struct oblist) );
	dobjs=obj;
	switch(index)
	{
	case 1 :select=1;
		Scale2=obj->scale;
		for(i=0;i<4;i++)
					for(j=0;j<4;j++)
				RotMatrix[i][j]=obj->ObjRotMatrix[i][j];
		glutPostRedisplay();

		break;
	case 2:
		if(select)
		{
		switchTranslate=1;
		for(i=0;i<objno;i++)dobjs=dobjs->next;
		dobjs->index=0;
		glutPostRedisplay();
		}
		break;
	case 3:
		select=0;
		objno=0;
		switchTranslate=0;
		//idle();
		glutPostRedisplay();
		break;
	case 4:select=0;
		switchTranslate=0;
		objno=0;
		//Reset();
		//Scale2=1.0;
		glutPostRedisplay();
		break;
	}
}
void objects_menu(int index){
	int i=0,j=0;
	select=0;
	objno=0;
	Reset();
	tempobj=obj;
	struct oblist *llobj;
	 while ( tempobj->next != 0)
        {
            tempobj = tempobj->next;
        }
	 
    tempobj->next= (struct oblist *) malloc( sizeof(struct oblist) );
	llobj=tempobj;
	tempobj=tempobj->next;
	tempobj->type = index;
	tempobj->xc = 0;
	tempobj->yc = 0;
	tempobj->zc = -25;
    tempobj->prev=llobj;
	tempobj->scale=1.0f;
	tempobj->next= 0;
	tempobj->index=0;
	tempobj->obi=0;
	tempobj->setp=0.0f;
	//tempobj=tempobj->next;
	for(i=0;i<4;i++)
					for(j=0;j<4;j++)
				tempobj->ObjRotMatrix[i][j]=RotMatrix[i][j];
	glutPostRedisplay();
}

void myMotion(int x,int y)
{
	float dx,dy;
	int i=0;
	oblist *listobjs;
	listobjs=obj;

	{
	if(select)
		/*y=(wh)/2-y;
		x=x-(wh)/2;*/
		y=wh-y;
		while(listobjs->type!=i)listobjs=listobjs->next;
		listobjs->xc=x;
		listobjs->yc=y;
		glutPostRedisplay();

	}
}
void Reset( void )
{
	                  RotMatrix[0][1] = RotMatrix[0][2] = RotMatrix[0][3] = 0.;
	RotMatrix[1][0]                   = RotMatrix[1][2] = RotMatrix[1][3] = 0.;
	RotMatrix[2][0] = RotMatrix[2][1]                   = RotMatrix[2][3] = 0.;
	RotMatrix[3][0] = RotMatrix[3][1] = RotMatrix[3][3]                   = 0.;
	RotMatrix[0][0] = RotMatrix[1][1] = RotMatrix[2][2] = RotMatrix[3][3] = 1.;
}
void specialKeys( int key, int x, int y ) 
{
	int i=0;
	oblist *listobjs;
	listobjs=obj;
	for(i=0;i<objno;i++)
	{
		listobjs=listobjs->next;
	}
	//while(listobjs->type!=i)listobjs=listobjs->next;
    const float step = 1;
    if (key == GLUT_KEY_RIGHT)
        listobjs->xc += step;
    else if (key == GLUT_KEY_LEFT)
        listobjs->xc -= step;
    else if (key == GLUT_KEY_UP)
        listobjs->yc += step;
    else if (key == GLUT_KEY_DOWN)
       listobjs->yc -= step;
	else if (key == GLUT_KEY_PAGE_UP)
       listobjs->zc -= step;
	else if (key == GLUT_KEY_PAGE_DOWN)
       listobjs->zc += step;
	checkLimits(&(listobjs->xc),&(listobjs->yc),&(listobjs->zc));
    glutPostRedisplay();
}
void updateButton(int i)
{
	changeColor=1;
	
	//select=0;
}
void INITGLUI(int mainWindow)
{
	GLUI_Panel *panel, *bigpanel;
    GLUI_RadioGroup *group;
	GLUI_Rotation *rot;
	GLUI_Translation *trans, *scale;

	


	    	Glui = GLUI_Master.create_glui( "GLUI", 0 );
     Glui->add_statictext( "Simple GLUI Example" );

     //	Glui->add_checkbox( "Change Color Of Selected Object", &changeColor );	
	 panel = Glui->add_panel( "Object Color" );
		group = Glui->add_radiogroup_to_panel( panel, &WhichColor );
			Glui->add_radiobutton_to_group( group, "Red" );
			Glui->add_radiobutton_to_group( group, "Yellow" );
			Glui->add_radiobutton_to_group( group, "Green" );
			Glui->add_radiobutton_to_group( group, "Cyan" );
			Glui->add_radiobutton_to_group( group, "Blue" );
			Glui->add_radiobutton_to_group( group, "Magenta" );
			Glui->add_radiobutton_to_group( group, "White" );
			Glui->add_radiobutton_to_group( group, "Black" );
    

	 panel = Glui->add_panel( "Object Transformation" );

		rot = Glui->add_rotation_to_panel( panel, "Rotation", (float *) RotMatrix );

		Glui->add_column_to_panel( panel, false );
		scale = Glui->add_translation_to_panel( panel, "Scale",  GLUI_TRANSLATION_Y , &Scale2 );
		scale->set_speed( 0.005f );
  Glui->add_button_to_panel( panel, "Update Color", 0, (GLUI_Update_CB) updateButton );
		// allow the object to be spun via the glui rotation widget:

		rot->set_spin( 1.0 );
     	Glui->add_button_to_panel( panel, "Reset", 0, (GLUI_Update_CB) Reset );
    /*Glui->add_statictext( "KeyBoard Controlls" );*/
	  panel = Glui->add_panel( "KeyBoard Controlls" );
	  Glui->add_statictext_to_panel(panel,"w:Forward Movement of User");
	   Glui->add_statictext_to_panel(panel,"s:Backword Movement of User");
	    Glui->add_statictext_to_panel(panel,"a:Left Movement of User");
		 Glui->add_statictext_to_panel(panel,"d:Right Movement of User");
		  Glui->add_statictext_to_panel(panel,"Page Up arrow:Forward Movement of Selected object");
		   Glui->add_statictext_to_panel(panel,"Page dn:Backword Movement of Selected object");
		    Glui->add_statictext_to_panel(panel,"Right:Right Movement of Selected object");
			 Glui->add_statictext_to_panel(panel,"Left:Left Movement of Selected object");
			  Glui->add_statictext_to_panel(panel,"n:Select Next Object");
			   Glui->add_statictext_to_panel(panel,"m:Select Previous object");
	/* GLUI_EditText *edittext =
    glui->add_edittext( "Text:", GLUI_EDITTEXT_TEXT, text );*/
	 /*glui->add_button( "Quit", 0,(GLUI_Update_CB)exit );*/
	// glui->add_button( "Quit", 40, takeinput );
	 /** Tell GLUI window which other window to recognize as the main gfx window **/
       Glui->set_main_gfx_window( mainWindow );
/** Register the Idle callback with GLUI (instead of with GLUT) **/
GLUI_Master.set_glutIdleFunc( NULL );
}
int main () {
	int o_menu,i,j;
	 obj = (struct oblist *) malloc( sizeof(struct oblist) ); 
	 obj->next=0;
	 obj->type=0;
	 obj->xc=0;
	 obj->yc=0;
	 obj->zc=-25;
	 obj->prev=0;
	 obj->scale=1.0f;
	 obj->index=0;
	 obj->obi=0;
	 obj->setp=0.0f;
	 Reset();
	 for(i=0;i<4;i++)
					for(j=0;j<4;j++)
				obj->ObjRotMatrix[i][j]=RotMatrix[i][j];
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    mainWindow=glutCreateWindow ("3D Painting");
	//glutIdleFunc(idle);
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glEnable(GL_DEPTH_TEST);
    
	
    glutKeyboardFunc (keyboard);
	glutSpecialFunc(specialKeys);
   // lightsOn();
	InitLists();
	o_menu=glutCreateMenu(objects_menu);
	glutAddMenuEntry("Sphere",0);
	glutAddMenuEntry("Cube",1);
	glutAddMenuEntry("Cone",2);
	glutAddMenuEntry("Torus",3);
	glutAddMenuEntry("Dodecahedron",4);
    glutAddMenuEntry("Octahedron",5);
	glutAddMenuEntry("Tetrahedron",6);
	glutAddMenuEntry("Icosahedron",7);
	glutAddMenuEntry("Teapot",8);

	glutCreateMenu(main_menu);
	
	 glutAddSubMenu("Draw objects",o_menu);
	 glutAddMenuEntry("Select object",1);
	 glutAddMenuEntry("Set Path",2);
	 glutAddMenuEntry("Enable Translation",3);
	 glutAddMenuEntry("Dselect object",4);
	 glutAddMenuEntry("quit",5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//glutMotionFunc(myMotion);
	//glutPassiveMotionFunc(MouseView);
	INITGLUI(mainWindow);

/** Now call the regular GLUT main loop **/
glutMainLoop();
    return 0;
} 
void
InitLists( void )
{
	// create the objects:

	Lists[0] = glGenLists( 1 );
	glNewList( Lists[0], GL_COMPILE );
	glutWireSphere( 7.0, 40, 40 );
	glEndList();

	Lists[1] = glGenLists( 1 );
	glNewList( Lists[1], GL_COMPILE );
	glutWireCube( 5 );
	glEndList();

	Lists[2] = glGenLists( 1 );
	glNewList( Lists[2], GL_COMPILE );
		glutSolidCone( 5, 1.5, 40, 40 );
	glEndList();

	Lists[3] = glGenLists( 1 );
	glNewList( Lists[3], GL_COMPILE );
		glutWireTorus( 3, 0.75, 40, 40 );
	glEndList();

	Lists[4] = glGenLists( 1 );
	glNewList( Lists[4], GL_COMPILE );
		glPushMatrix();
			glScalef( 3, 3, 3 );
			glutWireDodecahedron();
		glPopMatrix();
	glEndList();

	Lists[5] = glGenLists( 1 );
	glNewList( Lists[5], GL_COMPILE );
		glutWireOctahedron();
	glEndList();

	Lists[6] = glGenLists( 1 );
	glNewList( Lists[6], GL_COMPILE );
		glutWireTetrahedron();
	glEndList();

	Lists[7] = glGenLists( 1 );
	glNewList( Lists[7], GL_COMPILE );
		glutWireIcosahedron();
	glEndList();

	Lists[8] = glGenLists( 1 );
	glNewList( Lists[8], GL_COMPILE );
		glutWireTeapot( 3.0 );
	glEndList();


	// create the axes:

	/*AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glColor3fv( AXES_COLOR );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList();*/
}
void draw_reference_room()
{
	int STEP=500,s=250;
	 glBegin(GL_QUADS);

  /* Every four calls to glVertex, a quad is drawn */
	/* glColor3f(1.0,0.0,0.0);
   glVertex3f(-STEP, -STEP+s, -STEP-25);
   glVertex3f(-STEP, -STEP+s,  STEP-25);
   glVertex3f(-STEP,  STEP-s,  STEP-25);
   glVertex3f(-STEP,  STEP-s, -STEP-25);

    glColor3f(0.0,1.0,0.0);
   glVertex3f( STEP, -STEP+s, -STEP-25);
   glVertex3f( STEP, -STEP+s,  STEP-25);
   glVertex3f( STEP,  STEP-s,  STEP-25);
   glVertex3f( STEP,  STEP-s, -STEP-25);*/
   
   glColor3f(0.0,0.0,1.0);
   glVertex3f(-STEP, -STEP+s, -STEP-25);
   glVertex3f(-STEP, -STEP+s,  STEP-25);
   glVertex3f( STEP, -STEP+s,  STEP-25);
   glVertex3f( STEP, -STEP+s, -STEP-25);

    glColor3f(1.0,1.0,0.0);
  glVertex3f(-STEP,  STEP-s, -STEP-25);
  glVertex3f(-STEP,  STEP-s,  STEP-25);
  glVertex3f( STEP,  STEP-s,  STEP-25);
  glVertex3f( STEP,  STEP-s, -STEP-25);

 /*  glColor3f(1.0,0.0,1.0);
  glVertex3f(-STEP, -STEP+s, -STEP-25);
  glVertex3f(-STEP,  STEP-s, -STEP-25);
  glVertex3f( STEP,  STEP-s, -STEP-25);
  glVertex3f( STEP, -STEP+s, -STEP-25);

   glColor3f(0.0,1.0,1.0);
  glVertex3f(-STEP, -STEP+s,  STEP-25);
  glVertex3f(-STEP,  STEP-s,  STEP-25);
   glVertex3f( STEP,  STEP-s,  STEP-25);
   glVertex3f( STEP, -STEP+s,  STEP-25);*/

  /* No more quads */
  glEnd(); 

}
void DrawGrid()
{
	int STEP=490,s=240,inc;
	glTranslatef(0.0,0.0,0.0);
	
	glColor3f(1.0,0.0,0.0);
	
	
	for(s=240;s>-240;s=s-20){
	glBegin(GL_LINE_LOOP);
   glVertex3f(-STEP, -s, -STEP-25);
   glVertex3f(-STEP, -s,  STEP-25);
   glVertex3f( STEP, -s,  STEP-25);
   glVertex3f( STEP, -s, -STEP-25);
   glEnd();
	}
	
	s=240,STEP=490;
    
	
	for(inc=10;inc<980;inc=inc+20){
    glBegin(GL_LINE_LOOP);
   glVertex3f(-STEP+inc, -STEP+s, -STEP-25);
   glVertex3f(-STEP+inc, -STEP+s,  STEP-25);
   glVertex3f(-STEP+inc,  STEP-s,  STEP-25);
   glVertex3f(-STEP+inc,  STEP-s, -STEP-25);
      glEnd();
	}
	
}