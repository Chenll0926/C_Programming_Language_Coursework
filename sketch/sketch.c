// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int *binary(byte c){
  int i = 8;
  int result = c; //Convert from hexadecimal to decimal
  int array[10] = {0};
  while(i > 0){
    array[i] = result % 2;
    result = result / 2;
    i--;
  }
  int *p = array;
  return p; //Using pointer to deliver the binary result
}

void commandDY(display *d, state *s, int operand){
  s->ty += operand;
  if(s->tool == NONE){
    s->x = s->tx;
    s->y = s->ty;
  }else if(s->tool == LINE){
    line(d, s->x, s->y, s->tx, s->ty);
    s->x = s->tx;
    s->y = s->ty;
  }else if(s->tool == BLOCK){
    block(d, s->x, s->y, s->tx - s->x, s->ty - s->y);//block(d, x, y, width, hight)
    s->x = s->tx;
    s->y = s->ty;
  }
}

int current = 0; //The current position of the command in the file
void commandTOOL(display *d, state *s, int operand){
  if(operand == COLOUR){
    colour(d, s->data);
  }else if(operand == TARGETX){
    s->tx = s->data;
  }else if(operand == TARGETY){
    s->ty = s->data;
  }else if(operand == SHOW){
    show(d);
  }else if(operand == PAUSE){
    pause(d, s->data);
  }else if(operand == NEXTFRAME){
    s->start = current;
    s->end = true;
  }else{
    s->tool = operand;
  }
}

// Allocate memory for a drawing state and initialise it
state *newState() {
  state *new = malloc(sizeof(state));
  new->x = 0;
  new->y = 0;
  new->tx = 0;
  new->ty = 0;
  new->tool = LINE;
  new->start = 0;
  new->data = 0;
  new->end = false;
  return new;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
  free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
  int *p = binary(b);
  int result = 0;
  if(*(p + 1) == 1 && *(p + 2) == 0) result = TOOL;
  else if(*(p + 1) == 0 && *(p + 2) == 1) result = DY;
  else if(*(p + 1) == 0 && *(p + 2) == 0) result = DX;
  else if(*(p + 1) == 1 && *(p + 2) == 1) result = DATA;
  return result;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
  int result = b & 0x3f; //0x3f = 0011 1111, ignore the opcode and retain the remaining 6 bits
  if(result > 31){
    result -= 64; //Get the negative demical value of 6 bits binary
  }
  return result;
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
  int opcode = getOpcode(op);
  int operand = getOperand(op);
  byte opdata = op & 0x3f; //Get the remaining 6 bits except the 2 bits opcode;
  if(opcode == DX){
    s->tx += operand;
  }else if(opcode == DY){
    commandDY(d, s, operand);
  }else if(opcode == TOOL){
    commandTOOL(d, s, operand);
    s->data = 0; //After executing any TOOL command the 'data' field of the drawing state must be set to 0
  }else if(opcode == DATA){
    s->data = ((s->data) << 6) + opdata;
  }
}

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey) {
  if(data == NULL){
    return (pressedKey == 27);
  }
  char *filename = getName(d);
  state *s = (state*) data;
  FILE *fp = fopen(filename, "rb");
  byte op = fgetc(fp);
  int times = 0; //The times read a byte in a file
  while(times != current){ //If start with the middle command in the file,
    times++;               //read the comman byte by byte and correspomds to the start command
    op = fgetc(fp);
  }
  while(!feof(fp)){
    if(s->end == false){
      obey(d, s, op);
      op = fgetc(fp);
      current++;
    }else if(s->end == true){
      break;
    }
  }
  if(s->end == false){ //When finish to read all bytes in a file,
    current = 0;       //reset the current position to make sure that
  }                    //it would not make a mistake when the obey function is called again
  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
  s->tool = LINE;
  s->data = 0;
  s->end = false;
  show(d);
  fclose(fp);
  return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeState(s);
  freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./sketch file\n");
    exit(1);
  } else view(args[1]); // otherwise view sketch file in argument
  return 0;
}
#endif
