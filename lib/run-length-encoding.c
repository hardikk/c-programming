#include "run-length-encoding.h"
#define BUFFER_SIZE BUFSIZ
#define MAX_RUN UCHAR_MAX
static void mem_error(int line, const char *func)
{
  fprintf(stderr, "Memory Allocation Failure: %s:%d in function %s()\n", __FILE__, line, func);
}
static int resize_buffer(unsigned char **buffer, size_t *n)
{
  *n *= 2;
  unsigned char *temp = realloc(*buffer, (*n * sizeof(buffer[0])));
  if (!temp) {
    mem_error(__LINE__, __func__);
    return -1;
  }
  *buffer = temp;
  return 0;
}
void encode_rle(FILE *in, FILE *out)
{
  /* fread proved unreliable with int elements */
  unsigned char inbuffer[BUFFER_SIZE] = {0};
  /* encoded output can be up to 2x the input */
  size_t outbuffer_size = BUFFER_SIZE * 2;

  unsigned char *outbuffer = malloc(sizeof(unsigned char) * outbuffer_size);
  if (!outbuffer) {
    mem_error(__LINE__, __func__);
    return;
  }

  size_t i, n_read = 0, n_written = 0;
  unsigned char previous, occurrences;
  unsigned int n_encoded;

  for (;;) {
    if ((n_read = fread(inbuffer, sizeof(inbuffer[0]), BUFFER_SIZE, in)) < BUFFER_SIZE) {
      if (ferror(in)) {
        print_error(errno, __FILE__, "fread(in)");
        free(outbuffer);
        return;
      }
      if (feof(in) && n_read == 0)
        break;
    }

    previous = inbuffer[0];
    for (n_encoded = 0, i = 0; i < n_read; i++) {
      if (previous == inbuffer[i] && occurrences < MAX_RUN) {
        /*
         * keep counting as long as the byte value doesn't change
         * and we have room in the 'occurrences' variable
         */
        occurrences++;
      } else {
        if (n_encoded + 2 >= outbuffer_size) {
          if (resize_buffer(&outbuffer, &outbuffer_size) != 0) {
            free(outbuffer);
            return;
          }
        }
        outbuffer[n_encoded++] = occurrences;
        outbuffer[n_encoded++] = previous;
        occurrences = 1;
      }
      previous = inbuffer[i];
    }

    /* store data on the last run, which the loop can't handle */
    if (i) {
      outbuffer[n_encoded++] = occurrences;
      outbuffer[n_encoded++] = previous;
    }

    n_written = fwrite(outbuffer, sizeof(outbuffer[0]), n_encoded, out);
    if (n_written < n_encoded || ferror(out)) {
      print_error(errno, __FILE__, "fwrite(out)");
      free(outbuffer);
      return;
    }
  }
  free(outbuffer);
}
void decode_rle(FILE *in, FILE *out)
{
  puts("decoding");
}
#define EXT ".rle"
#define EXT_LEN strlen(EXT)

bool is_rle_file(char *filename)
{
  if (strcmp((filename + strlen(filename)) - EXT_LEN, EXT) == 0)
    return true;
  return false;
}
char *build_output_file_name(char *input)
{
  char *output = malloc(sizeof(char) * (strlen(input) + EXT_LEN + 1));
  if (!output)
    memory_error(__FILE__, __LINE__, __func__);

  if (is_rle_file(input))
    strncpy(output, input, strlen(input) - EXT_LEN);
  else {
    strcpy(output, input);
    strncat(output, EXT, EXT_LEN + 1);
  }
  return output;
}
