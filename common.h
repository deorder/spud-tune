#pragma once

#ifndef _XCOMMON_H_
#define _XCOMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

/*************** INCLUDES ***************/

#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>
#include <dirent.h>
#include <signal.h>
#include <string.h>
#include <getopt.h>
#include <locale.h>
#include <sys/types.h>

#include <MagickWand/MagickWand.h>

#if defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || \
    defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
# define NOMINMAX
# define WIN32_LEAN_AND_MEAN
# define _CRT_SECURE_NO_WARNINGS
# include <windows.h>
#endif

/*************** DEFINITIONS ***************/

#define MagickThrowException(wand) { \
  description = MagickGetException(wand, &severity); \
  (void)fprintf(stderr,"%s %s %lu %s\n", GetMagickModule(), description); \
  free(description); \
  exit(EXIT_FAILURE); \
}

#ifdef __cplusplus
}
#endif

#endif // _XCOMMON_H_
