#include <stdio.h> // use for c code
#include <stdlib.h> // use for malloc
#include <string.h> // use for string

#define MAX_ID 20
#define MAX_NAME 100
#define MAX_AUTHOR 30
#define MAX_TOPIC 100

// --- STRUCT DEFINITIONS ---

typedef struct {
    char id[MAX_ID];         // Book ID
    char name[MAX_NAME];     // Book Title
    char author[MAX_AUTHOR]; // Author
    int year;                // Year Published
    int quantity;            // Quantity
} Book;

typedef struct NodeBook {
    Book book;
    struct NodeBook* next;
} NodeBook;

typedef struct NodeTopic {
    char nameTopic[MAX_TOPIC];    // Topic name (e.g., "Science")
    NodeBook* listBook;           // Linked list of books in this topic
    struct NodeTopic* next;       // Next topic
} NodeTopic;

// --- FUNCTION DECLARATIONS ---

NodeTopic* addTopic(NodeTopic** head, const char* topicName);
void addBookToTopic(NodeTopic* head, const char* topicName, Book book);
void printAll(NodeTopic* head);

// --- FUNCTION IMPLEMENTATIONS ---

NodeTopic* addTopic(NodeTopic** head, const char* topicName) {
    // Check if topic already exists
    NodeTopic* temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->nameTopic, topicName) == 0) {
            return temp; // Return existing topic
        }
        temp = temp->next;
    }

    // Create new topic
    NodeTopic* newTopic = (NodeTopic*)malloc(sizeof(NodeTopic));
    strcpy(newTopic->nameTopic, topicName);
    newTopic->listBook = NULL;
    newTopic->next = *head;
    *head = newTopic;
    return newTopic;
}

void addBookToTopic(NodeTopic* head, const char* topicName, Book book) {
    NodeTopic* topic = head;
    while (topic != NULL) {
        if (strcmp(topic->nameTopic, topicName) == 0) {
            NodeBook* newBook = (NodeBook*)malloc(sizeof(NodeBook));
            newBook->book = book;
            newBook->next = topic->listBook;
            topic->listBook = newBook;
            return;
        }
        topic = topic->next;
    }

    printf("Topic '%s' not found. Cannot add book.\n", topicName);
}

void printAll(NodeTopic* head) {
    NodeTopic* topic = head;
    while (topic != NULL) {
        printf("=== Topic: %s ===\n", topic->nameTopic);
        NodeBook* bookNode = topic->listBook;
        while (bookNode != NULL) {
            Book b = bookNode->book;
            printf("  ID: %s | Title: %s | Author: %s | Year: %d | Quantity: %d\n",
                   b.id, b.name, b.author, b.year, b.quantity);
            bookNode = bookNode->next;
        }
        topic = topic->next;
        printf("\n");
    }
}

// --- MAIN FUNCTION ---

int main() {
    NodeTopic* topicList = NULL;

    // Add topics
    addTopic(&topicList, "Science");
    addTopic(&topicList, "Literature");

    // Create books
    Book book1 = {"B001", "Physics 101", "Einstein", 1950, 5};
    Book book2 = {"B002", "The Origin of Species", "Darwin", 1859, 3};
    Book book3 = {"B003", "Hamlet", "Shakespeare", 1603, 10};

    // Add books to topics
    addBookToTopic(topicList, "Science", book1);
    addBookToTopic(topicList, "Science", book2);
    addBookToTopic(topicList, "Literature", book3);

    // Print all
    printAll(topicList);

    return 0;
}
