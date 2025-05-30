#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 20
#define MAX_NAME 100
#define MAX_AUTHOR 30
#define MAX_TOPIC 100
#define TEXT_FILE "library.txt"
#define BIN_FILE "library.bin"

typedef struct {
    char id[MAX_ID];
    char name[MAX_NAME];
    char author[MAX_AUTHOR];
    int year;
    int quantity;
} Book;

typedef struct NodeBook {
    Book book;
    struct NodeBook* next;
} NodeBook;

typedef struct NodeTopic {
    char nameTopic[MAX_TOPIC];
    NodeBook* listBook;
    struct NodeTopic* next;
} NodeTopic;

// --- Function Prototypes ---
NodeTopic* addTopic(NodeTopic** head, const char* topicName);
void addBookToTopic(NodeTopic* head, const char* topicName, Book book);
void printAll(NodeTopic* head);
void writeText(NodeTopic* head);
void writeBin(NodeTopic* head);
void readBin(NodeTopic** head);

// --- Core Functions ---
NodeTopic* addTopic(NodeTopic** head, const char* topicName) {
    NodeTopic* temp = *head;
    while (temp) {
        if (strcmp(temp->nameTopic, topicName) == 0) return temp;
        temp = temp->next;
    }

    NodeTopic* newTopic = (NodeTopic*)malloc(sizeof(NodeTopic));
    strcpy(newTopic->nameTopic, topicName);
    newTopic->listBook = NULL;
    newTopic->next = *head;
    *head = newTopic;
    return newTopic;
}

void addBookToTopic(NodeTopic* head, const char* topicName, Book book) {
    NodeTopic* topic = head;
    while (topic) {
        if (strcmp(topic->nameTopic, topicName) == 0) {
            NodeBook* newBook = (NodeBook*)malloc(sizeof(NodeBook));
            newBook->book = book;
            newBook->next = topic->listBook;
            topic->listBook = newBook;
            return;
        }
        topic = topic->next;
    }
    printf("Topic '%s' not found.\n", topicName);
}

void printAll(NodeTopic* head) {
    NodeTopic* topic = head;
    while (topic) {
        printf("=== Topic: %s ===\n", topic->nameTopic);
        NodeBook* bookNode = topic->listBook;
        while (bookNode) {
            Book b = bookNode->book;
            printf("  ID: %s | Title: %s | Author: %s | Year: %d | Quantity: %d\n",
                   b.id, b.name, b.author, b.year, b.quantity);
            bookNode = bookNode->next;
        }
        topic = topic->next;
        printf("\n");
    }
}

// --- File I/O ---

void writeText(NodeTopic* head) {
    FILE* fp = fopen(TEXT_FILE, "w");
    if (!fp) {
        perror("Error opening text file");
        return;
    }

    NodeTopic* topic = head;
    while (topic) {
        fprintf(fp, "Topic: %s\n", topic->nameTopic);
        NodeBook* book = topic->listBook;
        while (book) {
            Book b = book->book;
            fprintf(fp, "%s;%s;%s;%d;%d\n", b.id, b.name, b.author, b.year, b.quantity);
            book = book->next;
        }
        fprintf(fp, "END_TOPIC\n");
        topic = topic->next;
    }

    fclose(fp);
    printf("Data saved to %s (text)\n", TEXT_FILE);
}

void writeBin(NodeTopic* head) {
    FILE* fp = fopen(BIN_FILE, "wb");
    if (!fp) {
        perror("Error opening binary file");
        return;
    }

    NodeTopic* topic = head;
    while (topic) {
        // Write topic name
        fwrite(topic->nameTopic, sizeof(char), MAX_TOPIC, fp);

        // Count books
        int bookCount = 0;
        NodeBook* tempBook = topic->listBook;
        while (tempBook) {
            bookCount++;
            tempBook = tempBook->next;
        }

        fwrite(&bookCount, sizeof(int), 1, fp);

        // Write books
        tempBook = topic->listBook;
        while (tempBook) {
            fwrite(&(tempBook->book), sizeof(Book), 1, fp);
            tempBook = tempBook->next;
        }

        topic = topic->next;
    }

    fclose(fp);
    printf("Data saved to %s (binary)\n", BIN_FILE);
}

void readBin(NodeTopic** head) {
    FILE* fp = fopen(BIN_FILE, "rb");
    if (!fp) {
        perror("Error reading binary file");
        return;
    }

    char topicName[MAX_TOPIC];
    int bookCount;
    Book book;

    while (fread(topicName, sizeof(char), MAX_TOPIC, fp) == MAX_TOPIC) {
        fread(&bookCount, sizeof(int), 1, fp);

        NodeTopic* topic = addTopic(head, topicName);

        for (int i = 0; i < bookCount; i++) {
            fread(&book, sizeof(Book), 1, fp);
            addBookToTopic(*head, topicName, book);
        }
    }

    fclose(fp);
    printf("Data loaded from %s (binary)\n", BIN_FILE);
}

// --- MAIN ---
int main() {
    NodeTopic* topicList = NULL;

    // Uncomment this block for demo data generation
    /*
    addTopic(&topicList, "Science");
    Book b1 = {"B001", "Physics", "Einstein", 1950, 5};
    Book b2 = {"B002", "Biology", "Darwin", 1859, 3};
    addBookToTopic(topicList, "Science", b1);
    addBookToTopic(topicList, "Science", b2);

    addTopic(&topicList, "Literature");
    Book b3 = {"B003", "Hamlet", "Shakespeare", 1603, 10};
    addBookToTopic(topicList, "Literature", b3);

    writeText(topicList);
    writeBin(topicList);
    */

    // Read from binary file
    readBin(&topicList);

    // Print structure
    printAll(topicList);

    return 0;
}
