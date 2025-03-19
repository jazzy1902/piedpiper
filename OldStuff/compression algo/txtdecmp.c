#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
struct node
{
    struct node* left; // will be used in tree
    char c;
    int isleaf;
    struct node* right; // will be used in tree
};
struct node* ultra_root = NULL;

struct node* createnode(char ch)
{
    struct node* newnode = malloc(sizeof(struct node));
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->c = ch;
    newnode->isleaf = 1;
    return newnode;
}

void hftree(char* code, struct node* lfnode)
{
    struct node* temp = ultra_root;
    int i=0;
    while(code[i] != '\0')
    {
        if(code[i] == '0' && temp->left == NULL)
        {
            struct node* newnode = lfnode;
            temp->left = newnode;
            temp->isleaf = 0;
            temp = temp->left;
        }
        else if(code[i] == '1' && temp->right == NULL)
        {
            struct node* newnode = lfnode;
            temp->right = newnode;
            temp->isleaf = 0;
            temp = temp->right;
        }
        else if(code[i] == '0')
        {
            temp = temp->left;
        }
        else if(code[i] == '1')
        {
            temp = temp->right;
        }
        i++;
    }
    temp = lfnode;
}
struct node* global_temp = NULL;
char hftree_trav(char bit)
{
    if(global_temp->isleaf == 1)
    {
        struct node* temp = NULL;
        temp = global_temp;
        global_temp = ultra_root;
        return temp->c;
    }
    else if(bit == '1')
    {
        global_temp = global_temp->right;
    }
    else if(bit == '0')
    {
        global_temp = global_temp->left;
    }
}
int main(int argc, char* argv[])
{
    FILE* fp = fopen("output.dat","r");
    char ch;
    char* code;
    int i;
    int newnline_counter = 0;
    ultra_root = createnode('a');
    printf("%p\n",ultra_root);
    while(fscanf(fp,"%c",&ch) != EOF)
    {
        printf("%c",ch);

        if(ch == '\n')
        {
            newnline_counter += 1;
        }
        if(newnline_counter == 2)
        {
            break;
        }
        fseek(fp,SEEK_CUR,1);
        fscanf(fp,"%s",code);
        hftree(code,createnode(ch));
        printf("%s",code);
        fseek(fp,SEEK_CUR,1);
    }
    rewind(fp);
    FILE* op = fopen("decom.txt","w");
    global_temp = ultra_root;
    // char bit;
    // bit = fgetc(fp);
    // //printf("%d",bit);
    // while(bit != EOF)
    // {
    //     printf("in here");
    //     printf("%d",bit);
    //     bit = fgetc(fp);
    // }
    // unsigned char byte;
    // fread(&byte, sizeof(unsigned char), 1, fp);
    // fprintf(op,"%c",hftree_trav(byte);
    // while(fread(&byte, sizeof(unsigned char), 1, fp) != 0)
    // {
    //     for(int i = 7; i <= 0; i++)
    //     {
    //         hftree_trav((byte >> i) & 1)
    //     }
    // }
}