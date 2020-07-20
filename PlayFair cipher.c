#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 80

//this fuction will generate the matrix with key in the table
void matrix(char key[], int key_length, char key_table[5][5])
{
	int i, j, k, * mat;

	mat = (int*)calloc(26, sizeof(int));
	for (i = 0; i < key_length; i++) {
		if (key[i] != 'j')
			mat[key[i] - 97] = 2;
	}

	mat['j' - 97] = 1;

	i = 0;
	j = 0;

	for (k = 0; k < key_length; k++) {
		if (mat[key[k] - 97] == 2) {
			mat[key[k] - 97] -= 1;
			key_table[i][j] = key[k];
			j++;
			if (j == 5) {
				i++;
				j = 0;
			}
		}
	}

	for (k = 0; k < 26; k++) {
		if (mat[k] == 0) {
			key_table[i][j] = (char)(k + 97);
			j++;
			if (j == 5) {
				i++;
				j = 0;
			}
		}
	}
}

//find function gives the row and column number of the two alphabets in the array
void find(char key_table[5][5], char a, char b, int arr[])
{
	int i, j;

	if (a == 'j') a = 'i';
	else if (b == 'j') 	b = 'i';

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			if (key_table[i][j] == a) {
				arr[0] = i;
				arr[1] = j;
			}
			if (key_table[i][j] == b) {
				arr[2] = i;
				arr[3] = j;
			}
		}
	}
}
//this function adds z in the end in case of odd number of characters in the plain string
int prepare(char str[], int ptrs)
{
	if (ptrs % 2 != 0) {
		str[ptrs++] = 'z';
		str[ptrs] = '\0';
	}
	return ptrs;
}

//this function converts upper case letters to lower case letters
void lowercase(char plain[], int plain_text_length)
{
	int i;
	for (i = 0; i < plain_text_length; i++) {
		if (plain[i] > 64 && plain[i] < 91) //ASCII of "A" is 65 and of "a" is 97
			plain[i] += 32;
	}
}

//this function deletes the spaces in the given string
int delete_spaces(char* plain, int plain_text_length)
{
	int i, count = 0;
	for (i = 0; i < plain_text_length; i++)
		if (plain[i] != ' ')
			plain[count++] = plain[i];
	plain[count] = '\0';
	return count;
}

//this is the encrypting function with its encryption procedure
void encrypt(char str[], char key_table[5][5], int plain_text_length)
{
	int i, a[4], p, q;

	for (i = 0; i < plain_text_length; i += 2) {

		find(key_table, str[i], str[i + 1], a);

		if ((a[0] == a[2]) && (a[1] == a[3])) {
			str[i] = key_table[a[0]][a[1]];
			str[i + 1] = key_table[a[0]][a[1]];
		}
		else if (a[0] == a[2]) {
			p = (a[1] + 1) % 5;
			q = (a[3] + 1) % 5;
			str[i] = key_table[a[0]][p];
			str[i + 1] = key_table[a[0]][q];
		}
		else if (a[1] == a[3]) {
			p = (a[0] + 1) % 5;
			q = (a[2] + 1) % 5;
			str[i] = key_table[p][a[1]];
			str[i + 1] = key_table[q][a[1]];
		}
		else {
			str[i] = key_table[a[0]][a[3]];
			str[i + 1] = key_table[a[2]][a[1]];
		}
	}
	i++;
	str[i] = '\0';
}

//this function calls the other functions for encryption
void playfair_encryption(char str[], char key[], char key_table[5][5])
{
	int plain_text_length, key_length;
	key_length = strlen(key);
	key_length = delete_spaces(key, key_length);
	lowercase(key, key_length);
	plain_text_length = strlen(str);
	lowercase(str, plain_text_length);
	plain_text_length = delete_spaces(str, plain_text_length);
	plain_text_length = prepare(str, plain_text_length);
	matrix(key, key_length, key_table);
	encrypt(str, key_table, plain_text_length);
}

//this is the decryption function with all its procedures
void decrypt(char str[], char key_table[5][5], int plain_text_length)
{
	int i, a[4], p, q;
	for (i = 0; i < plain_text_length; i += 2)
	{
		find(key_table, str[i], str[i + 1], a);

		if ((a[0] == a[2]) && (a[1] == a[3])) {
			str[i] = key_table[a[0]][a[1]];
			str[i + 1] = key_table[a[0]][a[1]];
		}
		else if (a[0] == a[2]) {
			if (a[1] == 0) p = 4;
			else p = (a[1] - 1) % 5;
			if (a[3] == 0) q = 4;
			else q = (a[3] - 1) % 5;
			str[i] = key_table[a[0]][p];
			str[i + 1] = key_table[a[0]][q];
		}
		else if (a[1] == a[3]) {
			if (a[0] == 0) p = 4;
			else p = (a[0] - 1) % 5;
			if (a[2] == 0) q = 4;
			else q = (a[2] - 1) % 5;
			str[i] = key_table[p][a[1]];
			str[i + 1] = key_table[q][a[1]];
		}
		else {
			str[i] = key_table[a[0]][a[3]];
			str[i + 1] = key_table[a[2]][a[1]];
		}
	}
	i++;
	str[i] = '\0';
}

//this function calls the other functions for decryption
void playfair_decryption(char str[], char key_table[5][5])
{
	int plain_text_length, key_length;
	plain_text_length = strlen(str);
	decrypt(str, key_table, plain_text_length);
}

void main()
{
	char str[SIZE], key[SIZE], ch;
	char key_table[5][5];
	int i = 0;
	strcpy(key, "encrypt"); // key is monarchy
	printf("Enter plain text to be encrypted\n");
	while ((ch = getchar()) != '\n')
	{
		str[i] = ch;
		i++;
	}
	printf("==========================\n");
	playfair_encryption(str, key, key_table);
	printf("Cipher text: %s\n", str);
	playfair_decryption(str, key_table);
	printf("...............\n\n");
	printf("Manual verification\n\n");
	printf("Key text: %s\n\n", key);
	printf("Deciphered text: %s\n", str);
	printf("===================\n");
	printf("The cipher board.....\n");
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
			printf("%c  ", key_table[i][j]);
		printf("\n");
	}
}


