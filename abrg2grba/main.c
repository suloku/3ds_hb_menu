#include <stdio.h>
#include <stdlib.h>

void swap(char* buf){

    char tmpbuff[4];
    tmpbuff[0] = buf[1];
    tmpbuff[1] = buf[2];
    tmpbuff[2] = buf[3];
    tmpbuff[3] = buf[0];

    buf[0] = tmpbuff[0];
    buf[1] = tmpbuff[1];
    buf[2] = tmpbuff[2];
    buf[3] = tmpbuff[3];
}

int main (int argc, char **argv) {
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( argv[1] , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */

  // terminate
  fclose (pFile);

  int i;
  for (i=0; i< lSize; i=i+4){
      swap(buffer+i);
  }

  char tmp[2048];
  sprintf(tmp, "%s.grba", argv[1]);
  pFile = fopen (tmp, "wb");
  fwrite (buffer , sizeof(char), lSize, pFile);
  fclose (pFile);


  free (buffer);
  return 0;
}
