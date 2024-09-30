#ifndef DAZUM_H
# define DAZUM_H

# define CAMERA_ZOOM_DELTA 0.1f
# define FLASHLIGHT_ZOOM 100.0f
# define FLASHLIGHT_ZOOM_DELTA 10.0f

# define STD_IMAGE_WRITE_IMPLEMENTATION
# include "stb_image_write.h"
# ifdef __linux__
#  include "X11/Xlib.h"
# endif
# include "raylib.h"
# include <stdio.h>

void		convert_to_rgba(unsigned char *data, const int width, const int height, int stride);
int			take_screenshot(const char *filename, const int width, const int height);
int			get_screen_width(void);
int			get_screen_height(void);
void		get_mouse_position(int *x, int *y);

Camera2D	init_camera(void);
Vector2		custom_get_mouse_position(void);
void		reset_camera(Camera2D *camera);
void		drag_position(Camera2D *camera);
void		handle_zoom(Camera2D *camera, float *flashlightRadius, bool isFlashlightMode);

# ifdef _WIN32
// Copied from https://github.com/raysan5/raylib/issues/1217#issuecomment-618428626
// To avoid conflicting windows.h symbols with raylib, some flags are defined
// WARNING: Those flags avoid inclusion of some Win32 headers that could be required
// by user at some point and won't be included...
//-------------------------------------------------------------------------------------

// If defined, the following flags inhibit definition of the indicated items.
#  define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#  define NOVIRTUALKEYCODES // VK_*
#  define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#  define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#  define NOSYSMETRICS      // SM_*
#  define NOMENUS           // MF_*
#  define NOICONS           // IDI_*
#  define NOKEYSTATES       // MK_*
#  define NOSYSCOMMANDS     // SC_*
#  define NORASTEROPS       // Binary and Tertiary raster ops
#  define NOSHOWWINDOW      // SW_*
#  define OEMRESOURCE       // OEM Resource values
#  define NOATOM            // Atom Manager routines
#  define NOCLIPBOARD       // Clipboard routines
#  define NOCOLOR           // Screen colors
#  define NOCTLMGR          // Control and Dialog routines
#  define NODRAWTEXT        // DrawText() and DT_*
#  define NOGDI             // All GDI defines and routines
#  define NOKERNEL          // All KERNEL defines and routines
#  define NOUSER            // All USER defines and routines
// #define NONLS             // All NLS defines and routines
#  define NOMB             // MB_* and MessageBox()
#  define NOMEMMGR         // GMEM_*, LMEM_*, GHND, LHND, associated routines
#  define NOMETAFILE       // typedef METAFILEPICT
#  define NOMINMAX         // Macros min(a,b) and max(a,b)
#  define NOMSG            // typedef MSG and associated routines
#  define NOOPENFILE       // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#  define NOSCROLL         // SB_* and scrolling routines
#  define NOSERVICE        // All Service Controller routines, SERVICE_ equates, etc.
#  define NOSOUND          // Sound driver routines
#  define NOTEXTMETRIC     // typedef TEXTMETRIC and associated routines
#  define NOWH             // SetWindowsHook and WH_*
#  define NOWINOFFSETS     // GWL_*, GCL_*, associated routines
#  define NOCOMM           // COMM driver routines
#  define NOKANJI          // Kanji support stuff.
#  define NOHELP           // Help engine interface.
#  define NOPROFILER       // Profiler interface.
#  define NODEFERWINDOWPOS // DeferWindowPos routines
#  define NOMCX            // Modem Configuration Extensions

// Type required before windows.h inclusion
typedef struct tagMSG *LPMSG;

#  include <windows.h>
#  undef PlaySound

// Type required by some unused function...
typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#  include <objbase.h>
#  include <mmreg.h>
#  include <mmsystem.h>

// Some required types defined for MSVC/TinyC compiler
#  if defined(_MSC_VER) || defined(__TINYC__)
#    include "propidl.h"
#  endif
# endif

#endif // DAZUM_H
