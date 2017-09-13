#include "common.h"

void print_usage(int argc, char **argv)
{
  printf("Usage: %s [OPTIONS]... output_filename.<ohctfm|ohctfu>\n", argv[0]);
  printf("  --help  (This help message)\n");
  printf("\n");
}

int main(int argc, char **argv) 
{
  char *description;
  unsigned int status;
  ExceptionType severity;

  FILE *header_file = NULL;
  FILE *output_file = NULL;

  char *input_filename = NULL;
  char *input_extension = NULL;

  unsigned char image_path[2048] = "";
  unsigned char header_path[2048] = "";
  unsigned char output_path[2048] = "";

  unsigned int image_size = 0;
  unsigned int image_width = 0;
  unsigned int image_height = 0;

  unsigned char image_header[20] = "";
  unsigned char image_data[1200 * 1200] = ""; /* Allocate enough space on stack */

  const char *option_string = "h?";

  int option_index = 0, option_value = 0;

  struct option option_list[] = {
    {"help",    no_argument,    NULL, 'h'},
    {NULL,      0,      NULL, 0}
  };

  printf("SpudFromHeaderPNG (Build "__TIME__" "__DATE__")\n");
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

  /* Set image data (hardcoded for now) */
  input_extension = strrchr(input_filename, '.');
  if(input_extension) {
    if(strncmp(input_extension, ".ohctfu", 7) == 0) {
      image_width = 54; image_height = 60; image_size = image_width * image_height;
    } else if(strncmp(input_extension, ".ohctfm", 7) == 0) {
      image_width = 1080; image_height = 1200; image_size = image_width * image_height;
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

  /* Read input image file */
  snprintf(image_path, sizeof(image_path), "%s.png", input_filename);
  status = MagickReadImage(input_image, image_path);
  if(status == MagickFalse)
    MagickThrowException(input_image);

  /* Export input image to image data */
  status = MagickExportImagePixels(input_image, 0, 0, image_width, image_height, "I", CharPixel, image_data);
  if(status == MagickFalse)
    MagickThrowException(input_image);

  /* Read header file */
  snprintf(header_path, sizeof(header_path), "%s.header", input_filename);
  header_file = fopen(header_path, "rb");
  fread(image_header, 1, 20, header_file);
  fclose(header_file);

  /* Save ohctfm/ohctfu file */
  snprintf(output_path, sizeof(output_path), "%s.new", input_filename);
  output_file = fopen(output_path, "wb");
  fwrite(image_header, 1, 20, output_file);
  fwrite(image_data, 1, image_size, output_file);
  fclose(output_file);
  printf("file written to: %s\n", output_path);

  /* Destroy images */
  DestroyMagickWand(input_image);

  /* Deinitialize magick */
  MagickWandTerminus();

  return(EXIT_SUCCESS);
}
