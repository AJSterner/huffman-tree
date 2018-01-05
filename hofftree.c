#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "queue.h"

typedef struct Node
{
        unsigned long weight;
        struct Node *left;      
        struct Node *right;
        int symbol;
} *Node;

int weight_compare(const void *a, const void *b);
struct Node *sorted_nodes(FILE *f, unsigned bits);

int weight_compare(const void *a, const void *b)
{
        return ((Node)a)->weight > ((Node)b)->weight;
}

struct Node *sorted_nodes(FILE *f, unsigned bits){
        assert(bits <= 16);
        uint32_t len = (uint32_t)1 << bits;

        struct Node *nodes = calloc(len, sizeof(*nodes));
        int c;
        for (unsigned i = 0; i < len; i++) {
                assert(nodes[i].weight == 0);
                nodes[i].symbol = (char)i;
                nodes[i].left = NULL;
                nodes[i].right = NULL;
        }

        while((c = fgetc(f)) != EOF) {
                assert(c > 0);
                nodes[c].weight++;
        }
        qsort(nodes, len, sizeof(*nodes), weight_compare);
        return nodes;
}

Queue_T hoffqueue (struct Node *nodes, size_t len)
{
        assert(len > 0);
        size_t i = 0;

        while (nodes[i++].weight == 0) {
                if(i == len) return NULL;
        }

        Queue_T queue = Queue_new(len - i);

        for(; i < len; i++) {
                Queue_addhi(queue, (nodes + i));
        }

        return queue;
}

struct Node *next_node(Queue_T mainq, Queue_T auxq)
{
        if(Queue_size(mainq) == 0) {
                return Queue_poplo(auxq);
        } else if (Queue_size(auxq) == 0) {
                return Queue_poplo(mainq);
        }

        if (((Node)Queue_peaklo(auxq))->weight < ((Node)Queue_peaklo(mainq))->weight) {
                return Queue_poplo(auxq);
        }

        return Queue_poplo(mainq);
}

Node Hofftree_new(struct Node *nodes, size_t len)
{
        Queue_T mainq = hoffqueue(nodes, len);
        Queue_T auxq = Queue_new(Queue_size(mainq) / 2);

        while(Queue_size(mainq) + Queue_size(auxq) > 1) {
                struct Node *new_node = malloc(sizeof(*new_node));
                new_node->symbol = -1;
                new_node->left = next_node(mainq, auxq);
                new_node->right = next_node(mainq, auxq);
                new_node->weight = new_node->left->weight + new_node->right->weight;
                // printf("left: %c %16lu, right: %c %16lu\n", new_node->left->symbol, new_node->left->weight, new_node->right->symbol, new_node->right->weight);
                Queue_addhi(auxq, new_node);
        }

        Node tree = next_node(mainq, auxq);
        assert(Queue_size(mainq) + Queue_size(auxq) == 0);
        Queue_free(&mainq);
        Queue_free(&auxq);
        return tree;
}

void Hofftree_free(Node hoff)
{
        if (hoff == NULL || hoff->symbol >= 0) return;
        Hofftree_free(hoff->left);
        Hofftree_free(hoff->right);
        free(hoff);
}

void Hofftree_print(Node hoff, int lvl)
{
        if (hoff == NULL) return;

        if (hoff->symbol >= 0) {
                printf("%c, %4lu, %2d\n", hoff->symbol, hoff->weight, lvl);
                return;
        }
        Hofftree_print(hoff->left, lvl + 1);
        Hofftree_print(hoff->right, lvl + 1);
}

int main ()
{
        FILE *f = fopen("hofftree.c", "rb");
        unsigned bits = 8;
        unsigned len = (uint32_t) 1 << bits;
        struct Node *nodes = sorted_nodes(f, bits);
        Node hoff = Hofftree_new(nodes, len);
        
        Hofftree_print(hoff, 1);

        Hofftree_free(hoff);
        free(nodes);
        fclose(f);
}
