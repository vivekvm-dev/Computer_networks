#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char sentence[1024];

    // 1. Create UDP socket (SOCK_DGRAM is used for UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Setup server address & port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // 3. Bind socket to port
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("UDP Server waiting for sentence...\n");

    // 4. Receive sentence from client
    int n = recvfrom(sockfd, sentence, sizeof(sentence) - 1, 0, 
                     (struct sockaddr *)&client_addr, &addr_len);
    sentence[n] = '\0'; // Null-terminate the received string

    // 5. Analyze the sentence
    // counts[0]=words, [1]=vowels, [2]=consonants, [3]=digits, [4]=special chars
    int counts[5] = {0, 0, 0, 0, 0}; 
    int in_word = 0;

    for (int i = 0; sentence[i] != '\0'; i++) {
        char ch = sentence[i];

        // Word counting logic (tracks transitions from space to non-space)
        if (isspace(ch)) {
            in_word = 0;
        } else if (in_word == 0) {
            in_word = 1;
            counts[0]++; // Found a new word
        }

        // Character type counting logic
        if (isalpha(ch)) {
            char lower = tolower(ch);
            if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u') {
                counts[1]++; // Vowel
            } else {
                counts[2]++; // Consonant
            }
        } else if (isdigit(ch)) {
            counts[3]++; // Digit
        } else if (!isspace(ch)) {
            counts[4]++; // Special character (excludes spaces/newlines)
        }
    }

    // 6. Send the 5 counts back to client
    sendto(sockfd, counts, sizeof(counts), 0, 
           (struct sockaddr *)&client_addr, addr_len);

    // 7. Close socket
    close(sockfd);
    return 0;
}