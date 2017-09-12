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

#if defined(_WIN32) || defined(__WIN32) || defined(__WIN32__) || \
    defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
#	define NOMINMAX
#	define WIN32_LEAN_AND_MEAN
#	define _CRT_SECURE_NO_WARNINGS
#	include <windows.h>
#endif

#include <wand/MagickWand.h>
#define MagickThrowException(wand) { \
	description = MagickGetException(wand, &severity); \
	(void)fprintf(stderr,"%s %s %lu %s\n", GetMagickModule(), description); \
	free(description); \
	exit(EXIT_FAILURE); \
}

void print_usage(int argc, char **argv)
{
	printf("Usage: %s [OPTIONS]... input_filename.<ohctfm|ohctfu>\n", argv[0]);
	printf("  --help	(This help message)\n");
	printf("\n");
}

int main(int argc, char **argv) 
{

  char *description;
	unsigned int status;
	ExceptionType severity;

  FILE *input_file = NULL;
  FILE *header_file = NULL;

	char *input_filename = NULL;
	char *input_extension = NULL;

  unsigned char image_path[2048] = "";
  unsigned char header_path[2048] = "";

  unsigned int image_size = 0;
  unsigned int image_width = 0;
  unsigned int image_height = 0;

  unsigned char image_header[20] = "";
  unsigned char image_data[1200 * 1200] = ""; /* Allocate enough space on stack */

	const char *option_string = "h?";

	int option_index = 0, option_value = 0;

	struct option option_list[] = {
		{"help",		no_argument,		NULL,	'h'},
		{NULL,			0,			NULL,	0}
	};

	printf("SpudToHeaderPNG (Build "__TIME__" "__DATE__")\n");
	printf("Copyright (c) 2017, Deorder. All rights reserved.\n\n");

	/* Handle arguments */
	while(option_value != -1)
	{
		option_value = getopt_long(argc, argv, option_string, option_list, &option_index);

		switch(option_value)
		{
			case 'h':
			case '?':
      print_usage(argc, argv); 
      exit(EXIT_SUCCESS);
			break;

			case 0:
			case -1:
			/* Do nothing */
			break;

			default:
			abort();
			break;
		}
	}

  if(optind < argc && argc - optind == 1) {
    input_filename = argv[optind];
  } else {
    print_usage(argc, argv);
    exit(EXIT_FAILURE);
  }

  /* Load image data (hardcoded for now) */
  input_extension = strrchr(input_filename, '.');
  if(input_extension) {
    if(strncmp(input_extension, ".ohctfu", 7) == 0) {
      image_width = 54; image_height = 60; image_size = image_width * image_height;
      input_file = fopen(input_filename, "rb");
      fread(image_header, 1, 20, input_file); fread(image_data, 1, image_size, input_file);
      fclose(input_file);
    } else if(strncmp(input_extension, ".ohctfm", 7) == 0) {
      image_width = 1080; image_height = 1200; image_size = image_width * image_height;
      input_file = fopen(input_filename, "rb");
      fread(image_header, 1, 20, input_file); fread(image_data, 1, image_size, input_file);
      fclose(input_file);
    } else {
      print_usage(argc, argv);
      exit(EXIT_FAILURE);
    }
  } else {
    print_usage(argc, argv);
    exit(EXIT_FAILURE);
  }

	/* Initialize magick */
	MagickWandGenesis();

  /* Create images */
  MagickWand *input_image = NewMagickWand();

  /* Read input image blob */
  status = MagickConstituteImage(input_image, image_width, image_height, "I", CharPixel, image_data);
  if(status == MagickFalse)
    MagickThrowException(input_image);

  /* Set image type */
  status = MagickSetImageType(input_image, GrayscaleType);
  if(status == MagickFalse)
    MagickThrowException(input_image);

	/* Save output header file */
  snprintf(header_path, sizeof(header_path), "%s.header", input_filename);
  header_file = fopen(header_path, "wb");
  fwrite(image_header, 1, 20, header_file);
  fclose(header_file);
  printf("header written to: %s\n", header_path);

  /* Save output image file */
  snprintf(image_path, sizeof(image_path), "%s.png", input_filename);
  status = MagickWriteImage(input_image, image_path);
  if(status == MagickFalse)
    MagickThrowException(input_image);
  printf("image written to: %s\n", image_path);

  /* Destroy images */
  DestroyMagickWand(input_image);

	/* Deinitialize magick */
	MagickWandTerminus();

	return(EXIT_SUCCESS);
}
