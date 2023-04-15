#include <stdio.h>
#include <stdlib.h>

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
	struct RecordType** records;
	int size;
};

// Compute the hash function
int hash(int x, int hashSz)
{
	unsigned int compHash;
	compHash = (unsigned int) ((3*x%23) + (3*x%47)) % (hashSz);
	return (int) compHash;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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
			pRecord->next  = NULL;
		}
		fclose(inFile);
	}
	return dataSz;
}

// Initializes hash table
struct HashType* createHashTable(int size)
{
	// printf("HashType function opens\n");
	struct HashType* pHash = (struct HashType*) malloc(sizeof(struct HashType));
	pHash->size = size;
	pHash->records = (struct RecordType**) calloc(size, sizeof(struct RecordType*));
	if (pHash->records == NULL)
	{
		printf("Cannot allocate memory\n");
		exit(-1);
	}
	return pHash;
}

// Inserts a record into the hash table
void insertRecord(struct RecordType* pRecord, struct HashType* pHash)
{
	// printf("insertRecord function opens\n");
	int index;
	index = hash(pRecord->id, pHash->size);
	// printf("Index = %d ID %d Name %c ORder %d\n", index, pRecord->id, pRecord->name, pRecord->order);
	if (pHash->records[index] == NULL) {
		// printf("First insert at %d\n", index);
		pHash->records[index] = pRecord;
	} else {
		struct RecordType *theNext;
		//printf("Collision at %d Outer\n", index);
		theNext = pHash->records[index];
		while (theNext->next != NULL) {
		    // printf("Collision at %d Inner\n", index);
			theNext = theNext->next;
		}
		theNext->next = pRecord;
	}
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	// printf("printRecords function opens\n");
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
	// printf("printRecords function closes\n");
}

// Displays records in the hash table
void displayRecordsInHash(struct HashType* pHash, int hashSz)
{
	// printf("displayRecordsInHash function opens\n");
	int i, j;
	struct RecordType* pRecord;

	for (i = 0; i < hashSz; ++i)
	{
		printf("Index %d -> ", i);
		if (pHash->records[i] == NULL) // Check if the index is empty
		{
			printf("Empty\n");
			continue;
		} else {
			pRecord = pHash->records[i];
			printf("%d %c %d ", pRecord->id, pRecord->name, pRecord->order);
			while (pRecord->next != NULL) {
				pRecord = pRecord->next;
				printf("\t==>CHAIN==> %d %c %d ", pRecord->id, pRecord->name, pRecord->order);
			}
		}
		printf("\n");
	}
	// printf("displayRecordsInHash function closes\n");
}

int main(void)
{
	// printf("main function opens\n");
	struct RecordType *pRecords;
    int recordSz = 0;
    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    int hashSz = recordSz * 2;
    struct HashType* hashTable = createHashTable(hashSz);
	// Display records in the hash table debug
	// displayRecordsInHash(hashTable, hashSz);
	// printf("this printed\n");
	for (int i = 0; i < recordSz; ++i){
      insertRecord(&pRecords[i], hashTable);
	  //printf("this is the problem\n");
    }
	// printf("this also printed\n");

    displayRecordsInHash(hashTable, hashSz);
	
    // Free memory
    free(pRecords);
	free(hashTable->records);	
    free(hashTable);
    return 0;
  
}
