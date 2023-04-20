extern int GL_FRAMEBUFFER;
extern int GL_COLOR_ATTACHMENT0;
extern int GL_RENDERBUFFER;
extern int GL_FRAMEBUFFER_COMPLETE;
extern int GL_MULTISAMPLE;

PROC glGenFramebuffers;
PROC glBindFramebuffer;
PROC glFramebufferRenderbuffer;
PROC glFramebufferTexture2D;
PROC glGenRenderbuffers;
PROC glBindRenderbuffer;
PROC glRenderbufferStorage;
PROC glRenderbufferStorageMultisample;
PROC glCheckFramebufferStatus;

void GL_FunctionGet();