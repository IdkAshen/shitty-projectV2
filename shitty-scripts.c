#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a classic node structure
typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Function to add a node to the end of the list
void append(Node **head_ref, int new_data) {
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return;

    new_node->data = new_data;
    new_node->next = NULL;

    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    Node *last = *head_ref;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new_node;
}

// Function to print the list and free memory safely
void printList(Node *node) {
    while (node != NULL) {
        printf("%d -> ", node->data);
        Node *temp = node;
        node = node->next;
        free(temp); // Freeing memory node by node
    }
    printf("NULL\n");
}

int main() {
    Node *head = NULL;

    append(&head, 10);
    append(&head, 20);
    append(&head, 30);

    printList(head); // Prints and clears the list

    return EXIT_SUCCESS;
}
