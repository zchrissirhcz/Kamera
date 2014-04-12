#pragma once

#define SIZE_X					46
#define SIZE_Y					56

#define N_POINTS				15

// math defines
#define M_PI				3.14159265
#define M_PI_FLOAT			3.14159265f
#define M_PI2				1.57079632
#define M_PI2_FLOAT			1.57079632f
#define M_PI32_FLOAT		4.71238897f
#define M_2PI_FLOAT			6.2831853f

//-------
#define NO					0
#define YES					1
#define CONTINUE			2
//-------

#define NO_FILE_EXCEPTION	200
#define CREATE_THREAD_EXCEPTION	100

#define ERROR_EXCEPTION 300


// debug console function
#ifndef NDEBUG
  #define DPRINT(text)  OutputDebugStringA(text "\n")
#else // #ifdef NDEBUG
  #define DPRINT(text)  ((void)0)
#endif

