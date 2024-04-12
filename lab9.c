#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10 

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType* next; 
};

// HashType
struct HashType
{
    struct RecordType* head; 
};

// Compute the hash function
int hash(int x)
{
    return x % HASH_SIZE; 
}


int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL; 
        }

        fclose(inFile);
    }

    return dataSz;
}

// Inserts a record into the hash table
void insertRecord(struct HashType hashTable[], int key, struct RecordType* record)
{
    int index = hash(key);
    
    struct RecordType* newNode = (struct RecordType*)malloc(sizeof(struct RecordType));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        exit(-1);
    }
    // Copy data to the new node
    newNode->id = record->id;
    newNode->name = record->name;
    newNode->order = record->order;
    newNode->next = NULL;

    if (hashTable[index].head == NULL)
    {
        hashTable[index].head = newNode;
    }
    else 
    {
        struct RecordType* temp = hashTable[index].head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}


void displayRecordsInHash(struct HashType hashTable[], int hashSz)
{
    int i;
    printf("\nRecords in the hash structure:\n");
    for (i = 0; i < hashSz; ++i)
    {
        struct RecordType* temp = hashTable[i].head;
        printf("Index %d -> ", i);
        while (temp != NULL)
        {
            printf("(%d, %c, %d) -> ", temp->id, temp->name, temp->order);
            temp = temp->next;
        }
        printf("NULL\n");
    }
    printf("\n");
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;


    struct HashType hashTable[HASH_SIZE];
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        hashTable[i].head = NULL;
    }

    recordSz = parseData("input_lab_9.txt", &pRecords);

    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, pRecords[i].id, &pRecords[i]);
    }

    displayRecordsInHash(hashTable, HASH_SIZE);

    for (int i = 0; i < recordSz; ++i)
    {
        struct RecordType* temp = pRecords[i].next;
        while (temp != NULL)
        {
            struct RecordType* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
    free(pRecords);

    return 0;
}
