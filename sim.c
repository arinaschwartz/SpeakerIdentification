#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>

/* utility routine that checks the return value from
 * malloc and generates an error if malloc returns NULL 
 */
void *ckMalloc(int size) {
  void *rv = malloc(size);
  if (rv == NULL) {
    fprintf(stderr, "malloc failed\n");
    exit(2);
  }
  return rv;
}

void *ckRealloc(void *data, int size) {
  void *rv = realloc(data, size);
  if (rv == NULL) {
    fprintf(stderr, "realloc failed\n");
    exit(2);
  }
  return rv;
}

/* getText: return a string containing the contents of the 
 * specified file.   Fails and exits, if it runs out of space
 * to hold the file.
 */

char *getText(FILE *f) {
  int c;
  int max = 100;    // current number of bytes in s
  char *s = (char *)ckMalloc(max);
  int len = 0;     // current length of the string represented by s

  while ((c = getc(f)) != EOF) {
    // compress runs of white space into one space
    if (isspace(c)) {
      if ((len > 0) && (s[len-1] != ' ')) {
	s[len] =' ';
	len++;
      }
    } else {
      s[len] = c;
      len++;
    }

    if (len >= max) {
      // allocate more space if necessary.
      max = max * 1.5;
      char *tmp = ckRealloc(s, max);
      s = tmp;

    }
  }

  // get rid of trailing space at the end of the string.
  if (s[len-1] == ' ') {
    s[len-1] = '\0';
  } else 
    s[len] = '\0';

  return s;
}


/*
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: <filename>\n");
    exit(2);
  }

  FILE *f1 = fopen(argv[1], "r");
  if (f1 == NULL) {
    fprintf(stderr, "Failed to open: %s\n", argv[1]);
    exit(2);
  }
  char *model1Text = getText(f1);
  printf("%s: %d\n", argv[1], strlen(model1Text));

  return 0;
}
*/
