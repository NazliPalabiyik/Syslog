#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Bağlı listedeki her düğümün yapısını tanımlayan Node yapısı
typedef struct Node {
    char data[MAX_LINE_LENGTH];
    struct Node* next;
} Node;

// Yeni bir düğüm oluşturma fonksiyonu
Node* create_node(char* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    strcpy(new_node->data, data);
    new_node->next = NULL;
    return new_node;
}

// Bağlı listeye düğüm ekleme fonksiyonu
void append(Node** head, char* data) {
    Node* new_node = create_node(data);
    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* last_node = *head;
        while (last_node->next) {
            last_node = last_node->next;
        }
        last_node->next = new_node;
    }
}

// Bağlı listenin içeriğini ekrana yazdıran fonksiyon
void display(Node* head) {
    Node* current_node = head;
    while (current_node) {
        printf("%s", current_node->data);
        current_node = current_node->next;
    }
}

// Syslog dosyasını okuyan ve verileri bağlı listeye ekleyen fonksiyon
Node* read_syslog(char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    
    Node* head = NULL;
    char line[MAX_LINE_LENGTH];
    
    // Dosyadaki her satırı okuyup bağlı listeye ekleme
    while (fgets(line, sizeof(line), file)) {
        append(&head, line);
    }
    
    fclose(file);
    return head;
}

int main() {
    char* file_path = "/var/log/syslog"; // Syslog dosyasının yolu
    Node* syslog_linked_list = read_syslog(file_path); // Syslog verilerini okuma ve bağlı listeye ekleme
    if (syslog_linked_list != NULL) {
        display(syslog_linked_list); // Bağlı listenin içeriğini yazdırma
    }
    return 0;
}

