#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
    struct node* prev;
    struct node* left; // will be used in tree
    int freq;
    char c;
    struct node* right; // will be used in tree
    struct node* next;
};

// priority queue on the basis of highest frequency
struct node* head = NULL;
struct node* tail = NULL;
struct node* ultra_root = NULL; 
struct node* createnode(int freq, char ch)
{
    struct node* newnode = malloc(sizeof(struct node));
    newnode->prev = NULL;
    newnode->next = NULL;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->freq = freq;
    newnode->c = ch;
    return newnode;
}

void insert(struct node* newnode)
{
    if(head == NULL){
        head = newnode;
        tail = newnode;
    }

    else
    {
        struct node* temp = head;
        struct node* current = head;
        while(temp!=NULL)
        {
            if(temp->freq < newnode->freq)
            {
                current = temp;
            }
            temp = temp->next;
        }
        // now current points to the position next to which nenwode should be added
        if(current == tail && current == head) // only one element
        {
            if(current->freq < newnode->freq)
            {
                newnode->prev = current;
                current->next = newnode;
                tail = newnode;
            }
            else{
                newnode->next = head;
                head->prev = newnode;
                head = newnode;
            }
            
        }
        else if(current == head) // insertion at head
        {
            newnode->next = head;
            head->prev = newnode;
            head = newnode;
        }
        else if(current == tail) // insertion at tail
        {
            newnode->prev = current;
            current->next = newnode;
            tail = newnode;
        }
        else  // elsewhere
        {
            newnode->next = current->next;
            newnode->prev = current;
            current->next->prev = newnode;
            current->next = newnode;
        }
        
    }
}

struct node** delete() // return a pointer to the pointers of first to elements of priority queue
{
    struct node** first2 = malloc(2*sizeof(struct node*));
    struct node* temp1 = NULL;
    struct node* temp2 = NULL;
    if(head->next == NULL)
    {
        temp1 = head;
        head = NULL;
        tail = NULL; 
    }
    else
    {
        temp1 = head;
        head = head->next;
        temp2 = head;
        head = head->next;
    }
    first2[1] = temp1; 
    first2[0] = temp2;
    return first2;
}

struct node* hftreemaker(struct node** nodearr)  
{
    // char* arr = malloc(2* sizeof(char));
    // for (int i = 0; i < 2; i++)
    // {
    //     arr[i] = nodearr[i]->c;
    // }
    struct node* root = createnode(nodearr[0]->freq+nodearr[1]->freq,'\0');
    root->right = nodearr[1];  // bigger freq is right
    root->left = nodearr[0];    // lower freq is left
    return root;
}
char** huffmann_code;

void hftree_trav(struct node* temp, int p, char* arr) // left is 0 & right is 1
{
    //struct node* temp = ultra_root;
    if(temp->left == NULL && temp->right == NULL)
    {
        arr[p] = '\0';
        huffmann_code[temp->c] = strdup(arr);
        printf("%s\n",arr);
        return;
    }
    else
    {
        arr[p] = '0';
        hftree_trav(temp->left,p+1,arr);
        arr[p] = '1';
        hftree_trav(temp->right,p+1,arr);
    }
}

void write_bit(FILE* fp, unsigned char bit)
{
    static unsigned char buffer = 0;
    static int buffer_index = 0;

    // Set the bit in the buffer
    if (bit)
    {
        buffer |= (1 << (8 - 1 - buffer_index));
    }

    buffer_index++;

    // If the buffer is full, write it to the file
    if (buffer_index == 8)
    {
        fwrite(&buffer, sizeof(unsigned char), 1, fp);
        buffer = 0;
        buffer_index = 0;
    }
}

int main(int argc, char* argv[])
{
    FILE* codex = fopen("codex.txt","r");
    FILE* fp = fopen(argv[1],"r");
    char ch;
    char ch_arr[128] = {0};
    int freq_arr[128] = {0};
    while(fscanf(fp,"%c",&ch) != EOF)
    {
        freq_arr[ch]+=1;
        ch_arr[ch] = ch;
    }
    for (int i = 0; i < 128; i++)
    {
        if(freq_arr[i] != 0)
        {
            printf("%d %c\n",freq_arr[i],ch_arr[i]);
        }
    }
    
    for (int i = 0; i < 128; i++)
    {
        if(freq_arr[i] != 0)
        {
            insert(createnode(freq_arr[i],ch_arr[i]));
        }
    }
    // the initial priority queue is ready
    struct node** first2;
    while(head != tail)
    {
        first2 = delete();
        for (int i = 0; i < 2; i++)
        {
            printf("%d\n",first2[i]->freq);
        }
        struct node* newnode = hftreemaker(first2);
        ultra_root = newnode;
        insert(newnode);
    }
    //printf("%d",ultra_root->left->freq);
    huffmann_code =(char**)calloc(128,sizeof(char*));
    char arr[128] = {-1};
    int index = 0;
    hftree_trav(ultra_root,index,arr);
    for (int i = 0; i < 128; i++)
    {
        if(huffmann_code[i] != NULL ) printf("%s\n",huffmann_code[i]);
    }
    FILE* op = fopen("output.dat","wb+");
    rewind(fp);
    for (int i = 0; i < 128; i++)
    {
        if(huffmann_code[i] != NULL)
        {
            fprintf(op,"%c %s ",ch_arr[i],huffmann_code[i]);
        
        }
    }
    fprintf(op,"\n");
    // fwrite(ultra_root, sizeof(*ultra_root), 1, op);
    // printf("%p",ultra_root);
    //fprintf(op,"\n");
    while(fscanf(fp,"%c",&ch)!=EOF)
    {
        for (int i = 0; i < strlen(huffmann_code[ch]); i++)
        {
            write_bit(op, huffmann_code[ch][i]);
        }
    }
    fclose(fp);
    fclose(op);
}
