#ifndef __DISPLAY_BUFFER_EARTH3D_H__
#define __DISPLAY_BUFFER_EARTH3D_H__

#include <vector>
#include <DisplayBuffer.h>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#else
#include <GPSINS.h>
#endif

using namespace std;

class DisplayBuffer_Earth3D : public DisplayBuffer
{
public:
   DisplayBuffer_Earth3D(int width, int height, string title);
   ~DisplayBuffer_Earth3D(void);

   void UpdateGPSPos_ECEF(vector<float> pos);
   void UpdateGPSPos_LLA(vector<float> pos);
#ifndef _WIN32
   void UpdateGPSINS(GPSINS_MSG &msg);
#endif

   // Event Handlers
   virtual void MouseWheel(int button, int dir, int x, int y) { }
   virtual void Display(void);
   virtual void Reshape(int x, int y);
   virtual void Keyboard(unsigned char key, int /*x*/, int /*y*/);
   virtual void Motion(int x, int y);
   virtual void Mouse(int button, int state, int x, int y);
   virtual void Cleanup(void);

protected:
   virtual void computeFPS(void);

private:
#ifdef _WIN32
   HANDLE _lock;
#else
   pthread_mutex_t _lock;
#endif

   struct cudaGraphicsResource *_cuda_pbo_resource; // handles OpenGL-CUDA exchange

   GLuint _gProgramCubeMap;			    // program handler of the cube map with two attached shaders: CubeMap.vert, CubeMap.frag
   GLuint _gTextureCubeMap;			    // texture handler for the cube map. 
   GLuint _gVerticesVBO, _gIndicesVBO;		    // Vertex Buffer Objects for the cubemap's indices and vertices.

   GLfloat _gDispLevel;				    // displacement level for the height map. Can be passed to the shader source
   GLuint _gProgramEarth;
   GLuint _gTextureHeightMap;
   GLuint _gTextureColorMap;
   GLuint _gEarthVerticesVBO, _gEarthIndicesVBO, _gEarthTexCoordsVBO, _normalVBO;
   float _radius;
   int   _num_lon;
   int   _num_lat;

   // Camera's properties
   float _gCamAngle;
   vector<float> *_gCamDir;
   float _gCamDist;
   vector<float> *_curCamDir;
   bool _camDirValid;

   // Earth's properties
   float _gEarthAngle;		    // rotation angle of the Earth around its own axis
   float _gEarthAngleSun;	    // rotation angle of the Earth around Sun

   vector<float> *_gEarthPos;

   int _ox, _oy;
   int _offsetx, _offsety;
   int _buttonState;

   void Init(void); 
   void initTextures(void);
   void initShaders(void);
   void initCubeMap(GLuint *verticesVBO, GLuint *indicesVBO);
   void initEarthMap(GLuint *verticesVBO, GLuint *indicesVBO, GLuint* texCoordVBO,GLuint* normalVBO, float radius, unsigned int lon, unsigned int lat);
   GLuint createVS(const string& filename);
   GLuint createFS(const string& filename);

   unsigned int _texWidth, _texHeight;
   unsigned int _mapWidth, _mapHeight;
   unsigned char *_textureMapData;
   unsigned char *_heightMapData;
   unsigned char *_cubeMapData[6];
};

#endif // __DISPLAY_BUFFER_EARTH3D_H__
