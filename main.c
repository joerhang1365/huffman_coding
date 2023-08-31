#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wchar.h>

typedef unsigned char byte;
typedef uint8_t       ui8;
typedef int16_t       i16;

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

struct node
{
  char data;
  ui8 frequency;
  struct node *left;
  struct node *right;
};

struct node *node_create(char data, ui8 frequency)
{
  struct node *new_node = malloc(sizeof(struct node));
  if(new_node == NULL)
  {
    printf("error allocating memory to new node\n");
  }
  new_node->data = data;
  new_node->frequency = frequency;
  new_node->left = NULL;
  new_node->right = NULL;

  return new_node;
}

void node_insert_left(struct node *root, struct node *left)
{
  root->left = left;
}

void node_insert_right(struct node *root, struct node *right)
{
  root->right = right;
}

void node_swap(struct node **a, struct node **b)
{
  struct node *temp = *a;
  *a = *b;
  *b = temp;
}

void node_sort_least_greatest(struct node **nodes, ui8 length)
{
  // bubble sort algorithm brudda
  bool swapped = false;
  for(ui8 i = 0; i < length - 1; i++)
  {
    swapped = false;
    for(ui8 j = 0; j < length - i - 1; j++)
    {
      if(nodes[j]->frequency > nodes[j + 1]->frequency)
      {
        node_swap(&nodes[j], &nodes[j + 1]);
        swapped = true;
      }
    }
    if(swapped == false)
    {
      break;
    }
  }
}

#define MIN_HEAP_SIZE 26

void min_heap_shift_left(struct node **min_heap, ui8 *heap_size)
{
  for(ui8 i = 0; i < *heap_size - 2; i++)
  {
    // shift everyting to left
    min_heap[i] = min_heap[i + 1];
  }
  *heap_size -= 1;
}

void huffman_code_print(struct node *root, ui8 binary_code, ui8 bit_index)
{
  if(root == NULL)
  {
    return;
  }

  if(root->data != '#')
  {
    printf("char: %c  binary: " BYTE_TO_BINARY_PATTERN " number of bits: %d\n", root->data, BYTE_TO_BINARY(binary_code), bit_index);
  }

  // recurse through left and right nodes
  // set the bit at the index to 0 for left branches
  huffman_code_print(root->left, binary_code |= 0 << bit_index, bit_index + 1);
  // set the bit at the index to 1 for right branches
  huffman_code_print(root->right, binary_code |= 1 << bit_index, bit_index + 1);
}

int main(int argc, char **argv)
{
  struct node **min_heap = malloc(MIN_HEAP_SIZE * sizeof(struct node *));
  if(min_heap == NULL)
  {
    printf("error mallocating memory to min heap\n");
  }

  char characters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
  int frequencys[]   = { 43, 11, 23, 17, 57, 9, 13, 15, 38, 1, 6, 28, 15, 34, 36, 16, 1, 39, 29, 35, 19, 5, 7, 1, 9, 1 };
  int length = sizeof(characters) / sizeof(characters[0]);

  // create a node for every character and frequency
  for(ui8 i = 0; i < length; i++)
  {
    min_heap[i] = node_create(characters[i], frequencys[i]);
  }

  // generate the huffman tree
  ui8 heap_size = length;
  while(heap_size > 1)
  {
    node_sort_least_greatest(min_heap, heap_size);

    struct node *left = min_heap[0];
    struct node *right = min_heap[1];

    struct node *temp = node_create('#', left->frequency + right->frequency);
    node_insert_left(temp, left);
    node_insert_right(temp, right);

    min_heap[0] = temp;
    min_heap_shift_left(min_heap + 1, &heap_size);
  }

  huffman_code_print(min_heap[0], 1, 0);

  free(min_heap);

  return 0;
}
