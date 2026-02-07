#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 256
#define MAX_PATH_LEN 512

typedef struct tnode
{
char *word;
int count;
struct tnode *lchild;
struct tnode *rchild;
} TNODE;
TNODE *tree = NULL;

TNODE *tree_create_node(const char *word);
void string_to_lower(char *str);
int is_alpha_char(int c);
int is_alnum_char(int c);
int get_word_from_string(char *str, int *pos, char *word);
TNODE *tree_insert(TNODE *p, const char *word);
void tree_write_to_file(TNODE *p, FILE *out);
void tree_free(TNODE *p);


TNODE *tree_create_node(const char *word)
{
	TNODE *p = (TNODE *)malloc(sizeof(TNODE));
if (p == NULL)
{
	printf("Error: failed to allocate memory for node\n");
	exit(0);
}
p->word = (char *)malloc(strlen(word) + 1);
if (p->word == NULL)
{
	printf("Error: failed to allocate memory for word\n");
	free(p);
	exit(0);
}
strcpy(p->word, word);
p->count = 1;
p->lchild = NULL;
p->rchild = NULL;

return p;
}
void string_to_lower(char *str)
{
	int i;
	for (i = 0; str[i] != '\0'; i++)
	{
	str[i] = tolower((unsigned char)str[i]);
}
}
int is_alpha_char(int c)
{
	return isalpha((unsigned char)c);
}
int is_alnum_char(int c)
{
	return isalnum((unsigned char)c);
}
int get_word_from_string(char *str, int *pos, char *word)
{
	int i = *pos;
	int j = 0;
	while (str[i] != '\0' && !is_alpha_char(str[i]))
	{
		i++;
	}
	if (str[i] == '\0')
	{
		*pos = i;
		return  0;
	}
	while (str[i] != '\0' && is_alnum_char(str[i]))
	{
	word[j] = str[i];
	j++;
	i++;
}


	word[j] = '\0';
    *pos = i;
    return 1;
}

TNODE *tree_insert(TNODE *p, const char *word)
{
	if (p == NULL)
	{
		return tree_create_node(word);
	}
    int cmp = strncmp(word, p->word, 5);
	if(cmp < 0)
{
	p->lchild = tree_insert(p->lchild, word);
	return p;
}
    if (cmp > 0)
{
	p->rchild = tree_insert(p->rchild, word);
	return p;
}
cmp = strcmp(word, p->word);
if (cmp < 0)
{
	p->lchild = tree_insert(p->lchild, word);
	return p;
}
if (cmp > 0)
{
	p->rchild = tree_insert(p->rchild, word);
	return p;
}
p->count++;
return p;
}

void tree_write_to_file(TNODE *p, FILE *out)
{
	if (p != NULL)
	{
		tree_write_to_file(p->lchild, out);
		fprintf(out, "%s\t%d\n", p->word, p->count);
		tree_write_to_file(p->rchild, out);
	}
}
void tree_free(TNODE *p)
{
	if (p != NULL)
	{
		tree_free(p->lchild);
		tree_free(p->rchild);
		free(p->word);
		free(p);
	}
}
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <file1> [file2 ...]\n", argv[0]);
		printf("Example: %s files/text1.txt files/text2.txt\n", argv[0]);
		return 1;
	}
	for (int i = 1; i < argc; i++)
	{
		char filepath[MAX_PATH_LEN];
		snprintf(filepath, sizeof(filepath), "%s", argv[i]);
		
		FILE *file = fopen(filepath, "r");
		if (file == NULL)
		{
			printf("Cannot open file: %s\n", filepath);
			continue;
		}
		printf("Processing file: %s\n", filepath);
		char line[MAX_WORD_LEN];
		while (fgets(line, sizeof(line), file) != NULL)
		{
			int pos = 0;
			char word[MAX_WORD_LEN];
			while (get_word_from_string(line, &pos, word))
			{
				string_to_lower(word);
				tree = tree_insert(tree, word);
			}
		}
		fclose(file);
	}
	FILE *output = fopen("dictionary.txt", "w");
	if (output == NULL)
	{
		printf("Cannot create output file dictionary.txt\n");
		tree_free(tree);
		return 1;
	}
	
	tree_write_to_file(tree, output);
	fclose(output);
	printf("\nDictionary written to: dictionary.txt\n");
    tree_free(tree);
    tree = NULL;
    getchar();
    return 0;
}



