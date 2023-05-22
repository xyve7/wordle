#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    FILE* fp = fopen("words.txt", "rb");
    if(fp != NULL) {
        /* Get number of tries passed, if any. */
        long tries = 5;
        if(argc > 1) {
            tries = strtol(argv[1], NULL, 10);
        }
        /* Word to guess, null for now. */
        char word[6] = {0};

        /* Get the size of the file in bytes, divide it by 6 (5 letter word plus 1 newline). */
        fseek(fp, 0, SEEK_END);
        long word_count = ftell(fp) / 6;

        /* Seed the random value and get the word. */
        srand(time(NULL) + getpid());
        long word_i = rand() % word_count;

        /* Get the random word. */
        fseek(fp, word_i * 6, SEEK_SET);
        fread(word, 1, 5, fp);
        word[5] = '\0';

        printf("word: %s, tries: %ld\n", word, tries);

        /* Start the guessing. */
        char input_word[6] = {0};
        while(strcmp(word, input_word) != 0 && tries > 0) {
            /* Get until the end of the buffer. */
            char ch;
            size_t pop_word_i = 0;
            while((ch = getchar()) != '\n' && pop_word_i < 5) {
                input_word[pop_word_i] = ch;
                pop_word_i++;
            }
            input_word[5] = '\0';

            /* Check if the word is valid. */
            bool valid = false;
            fseek(fp, 0, SEEK_SET);
            for(long i = 0; i < word_count; i++) {
                char compare[6];
                fread(compare, 1, 6, fp);
                compare[5] = '\0';
                if(strcmp(input_word, compare) == 0) {
                    valid = true;
                    break;
                }
            }

            if(valid) {
                /* Print out the characters properly. */
                printf("\x1b[1F\x1b[2K\r");
                for(size_t i = 0; i < 5; i++) {
                    if(input_word[i] == word[i]) {
                        printf("\x1b[92m%c\x1b[39m", input_word[i]);
                    } else if(strchr(word, input_word[i]) != NULL) {
                        printf("\x1b[93m%c\x1b[39m", input_word[i]);
                    } else {
                        putchar(input_word[i]);
                    }
                }
                tries--;
                printf(" (%ld)\n", tries);
            } else {
                /* Make entire word red. */
                printf("\x1b[1F\x1b[2K\r\x1b[91m%s\x1b[39m (%ld)\n", input_word, tries);
            }
        }
    } else {
        puts("wordle: word file not found!");
    }
}