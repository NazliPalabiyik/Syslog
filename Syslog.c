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
Node* create_node(const char* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->data, data, MAX_LINE_LENGTH - 1);
    new_node->data[MAX_LINE_LENGTH - 1] = '\0'; // Güvenlik için sonlandırıcı ekleniyor
    new_node->next = NULL;
    return new_node;
}

// Bağlı listeye düğüm ekleme fonksiyonu
void append(Node** head, const char* data) {
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
    if (head == NULL) {
        printf("Bagli liste bos.\n");
        return;
    }
    
    Node* current_node = head;
    while (current_node) {
        printf("%s", current_node->data);
        current_node = current_node->next;
    }
}

// Bağlı listeyi serbest bırakma fonksiyonu
void free_list(Node* head) {
    Node* current_node = head;
    Node* next_node;
    while (current_node) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
}

// Syslog dosyasını okuyan ve verileri bağlı listeye ekleyen fonksiyon
Node* read_syslog(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if (!file) {
        perror("Dosya acma hatasi");
        return NULL;
    }
    
    Node* head = NULL;
    char line[MAX_LINE_LENGTH];
    int is_empty = 1; // Dosyanın boş olup olmadığını kontrol etmek için

    // Dosyadaki her satırı okuyup bağlı listeye ekleme
    while (fgets(line, sizeof(line), file)) {
        append(&head, line);
        is_empty = 0;
    }

    fclose(file);
    
    if (is_empty) {
        printf("Dosya bos.\n");
        return NULL;
    }

    return head;
}

int main() {
    const char* file_path = "/var/log/syslog"; // Syslog dosyasının yolu
    Node* syslog_linked_list = read_syslog(file_path); // Syslog verilerini okuma ve bağlı listeye ekleme

    if (syslog_linked_list) {
        display(syslog_linked_list); // Bağlı listenin içeriğini yazdırma
        free_list(syslog_linked_list); // Belleği serbest bırakma
    }

    return 0;
}
