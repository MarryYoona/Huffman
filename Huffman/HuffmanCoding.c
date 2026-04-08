#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_WORDS 100  
#define MAX_WORD_LENGTH 50  

struct Word {
    char text[MAX_WORD_LENGTH];
    int count;
};

bool isWord(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int read_word(char* word, int max_word_length, FILE* file) {
    int c, len = 0;

    while ((c = getc(file)) != EOF && !isWord(c))
        ;
    if (c == EOF) {
        return EOF;
    }

    word[len++] = (char)c;
    while ((c = getc(file)) != EOF && isWord(c) && len < max_word_length - 1) {
        word[len++] = (char)c;
    }
    word[len] = '\0';

    return len;
}

void wordCount(const char* filename) {
    char word[MAX_WORD_LENGTH];
    struct Word words[MAX_WORDS];
    int num_words = 0;
    int i, k;
    int word_length;
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("Failed to open %s\n", filename);
        return;
    }

    while ((word_length = read_word(word, MAX_WORD_LENGTH, file)) != EOF) {
        for (k = 0; k < word_length; k++) {
            word[k] = tolower(word[k]);
        }

        for (i = 0; i < num_words; i++) {
            if (strcmp(word, words[i].text) == 0) {
                break;
            }
        }
        if (i < num_words) {
            words[i].count++;
        }
        else {
            strcpy(words[num_words].text, word);
            words[num_words].count = 1;
            num_words++;
        }
    }

    fclose(file);

    FILE* output_file = fopen("D:\\count.txt", "w");
    if (output_file == NULL) {
        printf("Failed to create count.txt\n");
        return;
    }

    for (i = 0; i < num_words; i++) {
        fprintf(output_file, "%s %d\n", words[i].text, words[i].count);
    }

    fclose(output_file);
}

typedef struct HTNode {
    char data[50];
    int pd;
    double weight;
    int parent;
    int lchild;
    int rchild;
} HTNode;

#define N 100
#define M 2*N-1

void CreateHT(HTNode ht[], int n) {
    int i, k, lnode, rnode;
    double min1, min2;

    for (i = 0; i < 2 * n - 1; i++) {
        ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
    }

    for (i = n; i < 2 * n - 1; i++) {
        min1 = min2 = INT_MAX;
        lnode = rnode = -1;

        for (k = 0; k <= i - 1; k++) {
            if (ht[k].parent == -1) {
                if (ht[k].weight < min1) {
                    min2 = min1;
                    rnode = lnode;
                    min1 = ht[k].weight;
                    lnode = k;
                }
                else if (ht[k].weight < min2) {
                    min2 = ht[k].weight;
                    rnode = k;
                }
            }
        }

        ht[i].weight = ht[lnode].weight + ht[rnode].weight;
        ht[i].lchild = lnode;
        ht[i].rchild = rnode;
        ht[lnode].parent = i;
        ht[rnode].parent = i;
    }
}

#define MAX_ENTRIES 100
typedef struct {
    char word[MAX_WORD_LENGTH];
    int frequency;
} WordInfo;

int compare(const void* a, const void* b) {
    WordInfo* wordA = (WordInfo*)a;
    WordInfo* wordB = (WordInfo*)b;
    return wordB->frequency - wordA->frequency;
}

void readDataFromFile(WordInfo* wordArray, int* arraySize) {
    FILE* file = fopen("D:\\count.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    char word[MAX_WORD_LENGTH];
    int frequency;
    int i = 0;
    while (fscanf(file, "%s %d", word, &frequency) != EOF) {
        strcpy(wordArray[i].word, word);
        wordArray[i].frequency = frequency;
        i++;
    }

    *arraySize = i;
    fclose(file);
}

void writeHuffmanTreeToFile(HTNode ht[], int n) {
    FILE* file = fopen("D:\\hfmtree.txt", "w");
    if (file == NULL) {
        printf("Failed to create hfmtree.txt\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%s %d %lf %d %d %d\n", ht[i].data, ht[i].pd, ht[i].weight,
            ht[i].parent, ht[i].lchild, ht[i].rchild);
    }

    fclose(file);
}

typedef struct {
    char cd[N];
    int start;
} HCode;

void CreateHCode(HTNode ht[], HCode hcd[], int n) {
    int i, f, c;
    HCode hc;

    for (i = 0; i < n; i++) {
        hc.start = n;
        c = i;
        f = ht[i].parent;

        while (f != -1) {
            if (ht[f].lchild == c) {
                hc.cd[hc.start--] = '0';
            }
            else {
                hc.cd[hc.start--] = '1';
            }
            c = f;
            f = ht[f].parent;
        }
        hc.start++;
        hcd[i] = hc;
    }
}

struct HuffmanNode {
    char word[MAX_WORD_LENGTH];
    char huffmanCode[N];
};

void writeHuffmanNode(struct HuffmanNode hm[], HTNode ht[], HCode hcd[], int n) {
    int i, k, t, j;
    int sum = 0, m = 0;

    for (i = 0; i < n; i++) {
        j = 0;
        strcpy(hm[i].word, ht[i].data);
        t = 0;
        for (k = hcd[i].start; k <= n; k++) {
            hm[i].huffmanCode[t] = hcd[i].cd[k];
            t++;
            j++;
        }
        m += ht[i].weight;
        sum += ht[i].weight * j;
    }
}

int isWordSeparator(char c) {
    return isspace(c) || ispunct(c);
}

char word[1000];

void convertToLowerCase(char* word) {
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        if (isalpha(word[i])) {
            word[i] = tolower(word[i]);
        }
    }
}

void replaceWordWithHuffman(struct HuffmanNode words[], int numWords, const char* inputText, const char* outputText) {
    FILE* inputFile = fopen(inputText, "r");
    FILE* outputFile = fopen(outputText, "w");

    while (fscanf(inputFile, "%s", word) != EOF) {
        int len = strlen(word);
        for (int i = 0; i < len; i++) {
            if (isWordSeparator(word[i])) {
                word[i] = ' ';
            }
        }
        convertToLowerCase(word);

        int replaced = 0;
        char* token = strtok(word, " ");
        while (token != NULL) {
            replaced = 0;
            for (int i = 0; i < numWords; i++) {
                if (strcmp(token, words[i].word) == 0) {
                    fprintf(outputFile, "%s ", words[i].huffmanCode);
                    replaced = 1;
                    break;
                }
            }
            if (!replaced) {
                fprintf(outputFile, "%s ", token);
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

char huffmanCode[1000];

void replaceHuffmanWithWord(struct HuffmanNode words[], int numWords, const char* inputText, const char* outputText) {
    FILE* inputFile = fopen(inputText, "r");
    FILE* outputFile = fopen(outputText, "w");

    while (fscanf(inputFile, "%s", huffmanCode) != EOF) {
        int replaced = 0;
        for (int i = 0; i < numWords; i++) {
            if (strcmp(huffmanCode, words[i].huffmanCode) == 0) {
                fprintf(outputFile, "%s ", words[i].word);
                replaced = 1;
                break;
            }
        }
        if (!replaced) {
            fprintf(outputFile, "%s ", huffmanCode);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

int main(void) {
    wordCount("D:\\huffman_input.txt");

    WordInfo wordArray[MAX_ENTRIES];
    int arraySize = 0;
    readDataFromFile(wordArray, &arraySize);
    qsort(wordArray, arraySize, sizeof(WordInfo), compare);

    HTNode ht[M] = { {{'\0'},0,0.0,0,0,0} };
    for (int i = 0; i < arraySize; i++) {
        strcpy(ht[i].data, wordArray[i].word);
        ht[i].pd = wordArray[i].frequency;
        ht[i].weight = wordArray[i].frequency;
    }
    CreateHT(ht, arraySize);
    writeHuffmanTreeToFile(ht, arraySize);

    HCode hcd[N] = { {{'\0'},0} };
    CreateHCode(ht, hcd, arraySize);
    struct HuffmanNode hm[M] = { {'\0'},{'\0'} };
    writeHuffmanNode(hm, ht, hcd, arraySize);

    replaceWordWithHuffman(hm, arraySize, "D:\\huffman_input.txt", "D:\\codefile.txt");

    replaceHuffmanWithWord(hm, arraySize, "D:\\codefile.txt", "D:\\textfile.txt");

    return 0;
}
