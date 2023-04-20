#include <windows.h>
#include <gl/gl.h>
#include "GLEXT.h"
#pragma comment(lib,"opengl32.lib")

int GL_FRAMEBUFFER            = 0x8D40;
int GL_COLOR_ATTACHMENT0      = 0x8CE0;
int GL_RENDERBUFFER           = 0x8D41;
int GL_FRAMEBUFFER_COMPLETE   = 0x8CD5;
int GL_MULTISAMPLE            = 0x809D;

PROC glGenFramebuffers;
PROC glBindFramebuffer;
PROC glFramebufferRenderbuffer;
PROC glFramebufferTexture2D;
PROC glGenRenderbuffers;
PROC glBindRenderbuffer;
PROC glRenderbufferStorage;
PROC glRenderbufferStorageMultisample;
PROC glCheckFramebufferStatus;

//--------------------------------------------------------------------
// 获取OpenGL函数
//--------------------------------------------------------------------
void GL_FunctionGet()
{
    glGenFramebuffers = wglGetProcAddress("glGenFramebuffersEXT");
    glBindFramebuffer = wglGetProcAddress("glBindFramebufferEXT");
    glFramebufferRenderbuffer = wglGetProcAddress("glFramebufferRenderbufferEXT");
    glFramebufferTexture2D = wglGetProcAddress("glFramebufferTexture2DEXT");
    glGenRenderbuffers = wglGetProcAddress("glGenRenderbuffersEXT");
    glBindRenderbuffer = wglGetProcAddress("glBindRenderbufferEXT");
    glRenderbufferStorage = wglGetProcAddress("glRenderbufferStorageEXT");
    glRenderbufferStorageMultisample = wglGetProcAddress("glRenderbufferStorageMultisampleEXT");
    glCheckFramebufferStatus = wglGetProcAddress("glCheckFramebufferStatusEXT");
}
//--------------------------------------------------------------------