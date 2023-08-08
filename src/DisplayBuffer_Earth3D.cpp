#include <string>
#include <iostream>
#include <cmath>

#define HELPERGL_EXTERN_GL_FUNC_IMPLEMENTATION
#include <helper_gl.h>
#if defined(__APPLE__) || defined(MACOSX)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// CUDA system and GL includes
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

// Helper functions
#include <helper_functions.h> // CUDA SDK Helper functions
#include <helper_cuda.h>      // CUDA device initialization helper functions

#include <DisplayBuffer_Earth3D.h>
#include <Utilities.h>

using namespace std;
using namespace Utilities;

extern char * result_string_pointer;

//*************************************************
// void DisplayBuffer_Earth3D()
//*************************************************
DisplayBuffer_Earth3D::DisplayBuffer_Earth3D(int _width, int _height, string _title) : DisplayBuffer(_width, _height, _title), 
	_gDispLevel(0.15f), _num_lon(180), _num_lat(180), _gCamAngle(0.0f), _gCamDist(5.0f), _gEarthAngle(0.0f), _gEarthAngleSun(0.0f), _buttonState(0), _offsetx(100), _offsety(100), _textureMapData(NULL), _heightMapData(NULL)
{
    glutInitWindowPosition(_offsetx, _offsety);
    glutInitWindowSize(width, height);
    _glWinID = glutCreateWindow(_title.c_str());

    for(int i=0; i < 6; i++)
    {
       _cubeMapData[i] = NULL;
    }

#ifdef _WIN32
	if (!(_lock = CreateMutex(NULL, FALSE, NULL)))
	{
		stringstream ss;
		ss << "Error: Mutex init failed\n";
		logger.Write(ERROR, std::string(ss.str()).c_str());

		cerr << RED << ss.str() << WHT << flush;
	}
#else
    /* Create the mutex */
    if( pthread_mutex_init(&_lock, NULL) != 0 )
    {
       stringstream ss;
       ss << "Error: Mutex init failed\n";
       logger.Write(ERROR, std::string(ss.str()).c_str());

       cerr << RED << ss.str() << WHT << flush;
       return;
    }
#endif

#ifdef _DEBUG
    cout << "Adding display buffer to handler.\n" << flush;
#endif
    DisplayHandler::AddDisplayBuffer(this);

    _gCamDir = new vector<float>(3, 0.0f);
    _gEarthPos = new vector<float>(3, 0.0f);

    Init();
}

//*************************************************
// void ~DisplayBuffer_Earth3D()
//*************************************************
DisplayBuffer_Earth3D::~DisplayBuffer_Earth3D(void)
{
    delete _gCamDir;
    delete _gEarthPos;
}

//*************************************************
// void Reshape()
//*************************************************
void DisplayBuffer_Earth3D::Reshape(int w, int h) 
{
    w = w < 1 ? 1 : w;
    h = h < 1 ? 1 : h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, double(w)/h, 0.1f, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//*************************************************
// void Mouse()
//*************************************************
void DisplayBuffer_Earth3D::Mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
       _buttonState |= 1<<button;
    }
    else if (state == GLUT_UP)
    {
       _buttonState = 0;
    }

    _ox = x;
    _oy = y;
}

//*************************************************
// void Motion()
//*************************************************
void DisplayBuffer_Earth3D::Motion(int x, int y)
{
#if 0
    float dx, dy;
    dx = (float)(x - ox);
    dy = (float)(y - oy);

    if( buttonState == 4 )  // Right mouse button (Zoom)
    {
       gCamDist += dy / 100.0f;
       printf("gCamDist = %f", gCamDist);
    }  
    else if( buttonState == 2)  // Middle mouse button (Translate)
    {
       gCamDirX += dx / 100.0f;
       gCamDirZ -= dy / 100.0f;
       printf("gCamDir = (%f,%f)\n", gCamDirX, gCamDirZ);
    }
    else if( buttonState == 1)
    {
       gEarthAngle += dy / 100.0f;  // Change earth angle (UP/DOWN)
       gDispLevel  += dx / 100.0f;  // Change displacement level (LEFT/RIGHT)
       printf("gDispLevel = %f\n", gDispLevel);
       printf("gEarthAngle = %f\n", gEarthAngle);
    }

    ox = x;
    oy = y;
    glutPostRedisplay();
#endif
}

//*************************************************
// void Keyboard()
//*************************************************
void DisplayBuffer_Earth3D::Keyboard(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:  // Escape
         exit(0); 
         break; 
      case 'a': // rotate camera around the Earth clockwise
	 _gCamDist -= 0.01f;
	 break;
      case 'd': // rotate camera around the Earth counterclockwise
         _gCamDist += 0.01f;
	 break;
      case '-': // decrease displacement level of the height map
	 _gDispLevel -= 0.01f;
	 break;
      case '+':	// increase displacement level of the height map
	 _gDispLevel += 0.01f;
	 break;
      
      default:  
         break;
    }
}

//*************************************************
// void Init()
//*************************************************
void DisplayBuffer_Earth3D::Init(void) 
{
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glShadeModel(GL_SMOOTH);

    InitGLBuffers();
    
    initTextures();
    initShaders();      
    
    // initialize Cubemap
    initCubeMap(&_gVerticesVBO, &_gIndicesVBO);
    
    // TODO: initialize the Earth.
    initEarthMap(&_gEarthVerticesVBO, &_gEarthIndicesVBO,&_gEarthTexCoordsVBO,&_normalVBO,_radius,_num_lon,_num_lat);

    (*_gCamDir)[0] = 0.0f;
    (*_gCamDir)[1] = 1.0f;
    (*_gCamDir)[2] = 0.0f;

    sdkCreateTimer(&_timer);
}

//*************************************************
// void computeFPS()
//*************************************************	
void DisplayBuffer_Earth3D::computeFPS()
{
   _frameCount++;
   _fpsCount++;

   if (_fpsCount == _fpsLimit)
   {
      float ifps = 1.f / (sdkGetAverageTimerValue(&_timer) / 1000.f);
      ostringstream ss;
      ss << _title << ": " << std::fixed << std::setprecision(1) << ifps;

      string fps = ss.str();
      glutSetWindowTitle(fps.c_str());
      _fpsCount = 0;

      sdkResetTimer(&_timer);
   }
}

//*************************************************
// void Display()
//*************************************************
void DisplayBuffer_Earth3D::Display()
{
   vector<float> *camPos = NULL;

   sdkStartTimer(&_timer);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
   glLoadIdentity();

#ifdef _WIN32
   DWORD res = WaitForSingleObject(_lock, INFINITE);
   if (res == WAIT_OBJECT_0)
   {
	   camPos = new vector<float>(*_gCamDir);
	   _gCamDist = 2.5f;
	   ReleaseMutex(_lock);
   }
   else
   {
	   camPos = new vector<float>(3, 0);
	   (*camPos)[1] = 1.0f;
   }
#else
   if( !pthread_mutex_lock(&_lock) )
   {
      camPos = new vector<float>(*_gCamDir);
      _gCamDist = 2.5f;
      pthread_mutex_unlock(&_lock);
   }
   else
   {
      camPos = new vector<float>(3, 0);
      (*camPos)[1] = 1.0f;
   }
#endif

   gluLookAt((*camPos)[0]*_gCamDist, (*camPos)[1]*_gCamDist, (*camPos)[2]*_gCamDist, 
             0, 0, 0, 
             0.0f, 1.0f, -1.0f);
	
   // Stars -- Cubemap
   glPushMatrix();
       glScalef(300,100,300);
       glUseProgram(_gProgramCubeMap);
		
       glBindBuffer(GL_ARRAY_BUFFER, _gVerticesVBO);
       glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, 0);
		
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _gIndicesVBO);
       glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT,  0);
   glPopMatrix();
		
   // Earth
   glPushMatrix();
      GLfloat matrix[16]; 
      glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
      //_gEarthAngle += 1;
		
      // TODO: Display the Earth	
      glUseProgram(_gProgramEarth);
      glEnableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, _gEarthVerticesVBO);
      glVertexPointer(3, GL_FLOAT, 0, 0);
	
      glEnableClientState(GL_NORMAL_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
      glNormalPointer(GL_FLOAT, 0, 0);
		
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, _gEarthTexCoordsVBO);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
      glUniform1i(glGetUniformLocation(_gProgramEarth, "texturemap"), 1);
      glBindTexture(GL_TEXTURE_2D,_gTextureColorMap);
      glActiveTexture(GL_TEXTURE1);
		
      glUniform1f(glGetUniformLocation(_gProgramEarth, "displevel"), _gDispLevel);
      glUniform1f(glGetUniformLocation(_gProgramEarth, "ex"), (*_gEarthPos)[0] + (*camPos)[0]*_gCamDist);
      glUniform1f(glGetUniformLocation(_gProgramEarth, "ez"), (*_gEarthPos)[2] + (*camPos)[2]*_gCamDist);
		
      glUniformMatrix4fv(glGetUniformLocation(_gProgramEarth, "MW"), 1, GL_FALSE, &matrix[0]);
	
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _gEarthIndicesVBO);
      glDrawElements(GL_TRIANGLES,6*(_num_lat-1)*(_num_lon-1)+6*(_num_lat-1) , GL_UNSIGNED_INT,  0);	
		
   glPopMatrix();
   glutSwapBuffers();

   sdkStopTimer(&_timer);
   computeFPS();
}

//*************************************************
// void Cleanup()
//*************************************************
void DisplayBuffer_Earth3D::Cleanup(void)
{
   //freeTexture();
   checkCudaErrors(cudaGraphicsUnregisterResource(_cuda_pbo_resource));

   glDeleteBuffers(1, &_pbo);

#ifdef _USE_BUFFER_TEX
   glDeleteTextures(1, &_bufferTex);
   glDeleteProgramsARB(1, &_fprog);
#else
   glDeleteTextures(1, &_displayTex);
#endif

   sdkDeleteTimer(&_timer);
}

//*************************************************
// void initTextures()
//*************************************************
void DisplayBuffer_Earth3D::initTextures()
{	

#ifdef HIRES
   string texFilename("./res/world.topo.bathy.200412.3x21600x10800.ppm");
   string heightFilename("./res/gebco_08_rev_elev_21600x10800.pgm");
#else
   string texFilename("C:\\Users\\67180\\source\\agast\\locuss_main\\data\\texturemap.ppm");
   string heightFilename("C:\\Users\\67180\\source\\agast\\locuss_main\\data\\heightmap.pgm");
#endif

   if (sdkLoadPPM4<unsigned char>(texFilename.c_str(), &_textureMapData, &_texWidth, &_texHeight) != true)
   {
      printf("Failed to load PPM image file: %s\n", texFilename.c_str());
      exit(EXIT_FAILURE);
   }
   if (sdkLoadPGM<unsigned char>(heightFilename.c_str(), &_heightMapData, &_mapWidth, &_mapHeight) != true)
   {
      printf("Failed to load PGM image file: %s\n", heightFilename.c_str());
      exit(EXIT_FAILURE);
   }
   // TODO: Generate texture map and height map for the Earth
   glGenTextures(1, &_gTextureHeightMap);
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, _gTextureHeightMap);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, _mapWidth, _mapHeight, 0, GL_LUMINANCE,GL_UNSIGNED_BYTE, _heightMapData);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   
   printf("HeightMap Info: Height = %d, Width = %d, Channels = %d\n", _mapHeight, _mapWidth, 1);
	
   glGenTextures(1, &_gTextureColorMap);		
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, _gTextureColorMap);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texWidth, _texHeight, 0, GL_RGBA,GL_UNSIGNED_BYTE, _textureMapData);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
   printf("ColorMap Info: Height = %d, Width = %d, Channels = %d\n", _texHeight, _texWidth, 4);

   vector<string> faces
   {
      "C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap_east.ppm",
      "C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap_west.ppm",
      "C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap_top.ppm",
      "C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap_bottom.ppm",
      "C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap_north.ppm",
      "C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap_south.ppm"
   };

   // Generate CubeMap
   glGenTextures(1, &_gTextureCubeMap);
   glActiveTexture(GL_TEXTURE2);

   glBindTexture(GL_TEXTURE_CUBE_MAP, _gTextureCubeMap);
   unsigned int width, height;
   for( unsigned int i=0; i < faces.size(); i++ )
   {
      if (sdkLoadPPM4<unsigned char>(faces[i].c_str(), &_cubeMapData[i], &width, &height) != true)
      {
         printf("Failed to load PGM image file: %s\n", faces[i].c_str());
         exit(EXIT_FAILURE);
      }

      glTexImage2D(
	  GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	  0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _cubeMapData[i]);
   }
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

//*************************************************
// void initShaders()
//*************************************************
void DisplayBuffer_Earth3D::initShaders()
{
   // Creation and initialization of CubeMap's shaders
   _gProgramCubeMap = glCreateProgram();
	
   glAttachShader(_gProgramCubeMap, createVS("C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap.vert"));
   glAttachShader(_gProgramCubeMap, createFS("C:\\Users\\67180\\source\\agast\\locuss_main\\data\\cubemap.frag"));
    
   glLinkProgram(_gProgramCubeMap);
    
   glUniform1i(glGetUniformLocation(_gProgramCubeMap, "cubeMap")   , 2);
	
   // TODO: Create and initialize new program(s) to use new shader(s)
   _gProgramEarth = glCreateProgram();
   glAttachShader(_gProgramEarth, createVS("C:\\Users\\67180\\source\\agast\\locuss_main\\data\\Earth.vert"));
   glAttachShader(_gProgramEarth, createFS("C:\\Users\\67180\\source\\agast\\locuss_main\\data\\Earth.frag"));
   glLinkProgram(_gProgramEarth);
}

//*************************************************
// void initCubeMap()
//*************************************************
void DisplayBuffer_Earth3D::initCubeMap(GLuint *verticesVBO, GLuint *indicesVBO) 
{
   float vertices[] = {
      1,  1,  1,				
     -1,  1,  1,
     -1, -1,  1,
      1, -1,  1,
      1, -1, -1,
     -1, -1, -1,
     -1,  1, -1,
      1,  1, -1
   };

   GLuint indices[] = {
      0, 1, 2, 3,                 // Front face of the cube
      7, 4, 5, 6,                 // Back face of the cube
      6, 5, 2, 1,                 // Left face of the cube
      7, 0, 3, 4,                 // Right face of the cube
      7, 6, 1, 0,                 // Top face of the cube
      3, 2, 5, 4,                 // Bottom face of the cube
   };
	
   glGenBuffers(1, verticesVBO);
   glGenBuffers(1, indicesVBO);
	
   glBindBuffer(GL_ARRAY_BUFFER, *verticesVBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW );
   glBindBuffer(GL_ARRAY_BUFFER, 0);
	 
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indicesVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//*************************************************
// void initCubeMap()
//*************************************************
void DisplayBuffer_Earth3D::initEarthMap(GLuint *verticesVBO, GLuint *indicesVBO, GLuint* texCoordVBO,GLuint* normalVBO, float radius, unsigned int lon, unsigned int lat) 
{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<unsigned int> indices;
    float const dB = M_PI/(float)(lat);
    float const dA = 2*M_PI/(float)(lon);
    vertices.resize(lat * lon * 3);
    normals.resize(lat*lon * 3);
    texcoords.resize(lat * lon *2);
    std::vector<GLfloat>::iterator v = vertices.begin();
    std::vector<GLfloat>::iterator n = normals.begin();
    std::vector<GLfloat>::iterator t = texcoords.end();
    double X, Y, Z;
    float x, y;

    FILE *fp;

    if( !(fp = fopen("coords.csv", "w")) )
    {
       cout << RED << "Cannot create coords.txt file." << WHT << endl << flush;
       return;
    }

    fprintf(fp, "Lat (deg), Lon (deg), x, y, ECEF_X, ECEF_Y, ECEF_Z,\n");

    for(int j = 0; j <lat; j++) {
        for(int i = 0; i<lon; i++) {
	    float const a = (i - (float)lon/2)*dA;
	    float const b = -(j - (float)lat/2)*dB;

            GeodeticToECEF((float)b, (float)a, 0.0f, &X, &Y, &Z);

	    *v++ = (float)X;
	    *v++ = (float)Y;
	    *v++ = (float)Z;

            y = j / (float)(lat - 1);
	    x = i / (float)(lon - 1);

	    *--t = y;
   	    *--t = x;

	    *n++ = (float)X;
	    *n++ = (float)Y;
	    *n++ = (float)Z;

	    fprintf(fp, "%.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n", b, a, x, y, X, Y, Z);
				
	 }
    }

    fclose(fp);
	
    indices.resize((lat-1) * (lon-1) * 6 +6*(lat-1) );
    std::vector<unsigned int>::iterator ind = indices.begin();
    for(int i = 0; i < lat-1; i++){
	for(int j = 0; j< lon-1; j++) {
		  
	  *ind++ = i*lon + j;
	  *ind++ = (i+1)*lon + j;
  	  *ind++ = i*lon + j+1;
	  *ind++ = (i+1)*lon + j;
	  *ind++ = (i+1)*lon + j+1;
	  *ind++ = i*lon + j+1;
       }
   }	
   
   for(int i=0;i<lat-1;i++){
      *ind++ = i*lon + lon-1;
      *ind++ = (i+1)*lon + lon-1;
      *ind++ = i*lon ;
      *ind++ = (i+1)*lon + lon-1;
      *ind++ = (i+1)*lon ;
      *ind++ = i*lon ;
   }
  
   
	
    glGenBuffers(1, verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, *verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, 3*lat*lon*sizeof(GLfloat) , &vertices[0], GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
    glGenBuffers(1, indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ((lat-1) * (lon-1) * 6+6*(lat-1))*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, *texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, 2*lat*lon*sizeof(GLfloat) , &texcoords[0], GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	
    glGenBuffers(1, normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, *normalVBO);
    glBufferData(GL_ARRAY_BUFFER, 3*lat*lon*sizeof(GLfloat) , &normals[0], GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//*************************************************
// void createVS()
//*************************************************
GLuint DisplayBuffer_Earth3D::createVS(const string& filename)
{
   string shaderSource;

   if (!readDataFromFile(filename, shaderSource)){
      cout << "Cannot find file name: " + filename << endl;
      exit(-1);
   }

   GLint length = shaderSource.length();
   const GLchar* shader = (const GLchar*) shaderSource.c_str();

   GLuint vs = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vs, 1, &shader, &length);
   glCompileShader(vs);

   char output[1024] = {0};
   glGetShaderInfoLog(vs, 1024, &length, output);
   printf("VS compile log: %s\n", output);

   return vs;
}

//*************************************************
// void createFS()
//*************************************************
GLuint DisplayBuffer_Earth3D::createFS(const string& filename) 
{
   string shaderSource;

   if (!readDataFromFile(filename, shaderSource)) {
      cout << "Cannot find file name: " + filename << endl;
      exit(-1);
   }

   GLint length = shaderSource.length();
   const GLchar* shader = (const GLchar*) shaderSource.c_str();

   GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fs, 1, &shader, &length);
   glCompileShader(fs);

   char output[1024] = {0};
   glGetShaderInfoLog(fs, 1024, &length, output);
   printf("FS compile log: %s\n", output);

   return fs;
}

//*************************************************
// void UpdateGPSPos_ECEF()
//*************************************************
void DisplayBuffer_Earth3D::UpdateGPSPos_ECEF(vector<float> pos)
{
    float d = dist(pos);
#ifdef _WIN32
	DWORD res = WaitForSingleObject(_lock, 0);
	if (res == WAIT_OBJECT_0)
	{
		(*_gCamDir)[0] = pos[0] / d;
		(*_gCamDir)[1] = pos[1] / d;
		(*_gCamDir)[2] = pos[2] / d;

		ReleaseMutex(_lock);
	}
#else
    if( !pthread_mutex_trylock(&_lock) )
    { 
       (*_gCamDir)[0] = pos[0] / d;
       (*_gCamDir)[1] = pos[1] / d;
       (*_gCamDir)[2] = pos[2] / d;

       pthread_mutex_unlock(&_lock);
    }
#endif
}
  
//*************************************************
// void UpdateGPSPos_LLA()
//*************************************************
void DisplayBuffer_Earth3D::UpdateGPSPos_LLA(vector<float> pos)
{
   vector<float> ecef(3, 0);

   GeodeticToECEF(pos, 6378137.0f, &ecef);

   //float d = dist(ecef);
#ifdef _WIN32
   DWORD res = WaitForSingleObject(_lock, 0);
   if (res == WAIT_OBJECT_0)
   {
	   (*_gCamDir)[0] = ecef[0];
	   (*_gCamDir)[1] = ecef[1];
	   (*_gCamDir)[2] = ecef[2];

	   ReleaseMutex(_lock);
   }
#else
   if( !pthread_mutex_trylock(&_lock) )
   {
      (*_gCamDir)[0] = ecef[0];
      (*_gCamDir)[1] = ecef[1];
      (*_gCamDir)[2] = ecef[2];

      pthread_mutex_unlock(&_lock);
   }
#endif
}

//*************************************************
// void UpdateGPSPos_LLA()
//*************************************************
#ifndef _WIN32
void DisplayBuffer_Earth3D::UpdateGPSINS(GPSINS_MSG &msg)
{
   vector<double> lla(3, 0);
   vector<double> ecef(3, 0);

   lla[0] = DEG2RAD(msg.lla_[0]);
   lla[1] = DEG2RAD(msg.lla_[1]);
   //lla[2] = msg.lla_[2];

#ifdef _DEBUG
   cout << GRN << "Convert LLA lat = " << lla[0] << " lon = " << lla[1] << " alt = " << lla[2] << " to ECEF from GPS/INS msg...\n" << WHT << flush;
#endif
   GeodeticToECEF(lla, 6378137.0, &ecef);

#ifdef _DEBUG
   cout << GRN "Current coord is (" << ecef[0] << "," << ecef[1] << "," << ecef[2] << ")...\n" << WHT << flush;
#endif

#ifdef _DEBUG
   cout << GRN << "Calculate vector Eucidean distance" << WHT << endl << flush;
#endif
   //double d = dist(ecef);

#ifdef _DEBUG
   cout << GRN << "Attempting to update camera pos based on ECEF coord...." << WHT << endl << flush;
#endif
   if( !pthread_mutex_trylock(&_lock) )
   {
      (*_gCamDir)[0] = (float)ecef[0];
      (*_gCamDir)[1] = (float)ecef[1];
      (*_gCamDir)[2] = (float)ecef[2];

      pthread_mutex_unlock(&_lock);
   }
}
#endif
