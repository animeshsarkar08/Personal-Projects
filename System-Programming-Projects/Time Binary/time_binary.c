#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct BTnode {
    int value;
    struct BTnode *left,*right;
}BTnode;

BTnode* createnode(int num);
bool insert(BTnode **root,int num);
void dealloc(BTnode **root);
void preordertree(BTnode *node);
void inordertree(BTnode *node);
void cal_time(BTnode **root);

int main(void){
	BTnode *root = NULL;
	/* insert(&root,67); */
	/* insert(&root,52); */
	/* insert(&root,13); */
	/* insert(&root,02); */
	/* preordertree(root); */
	/* printf("\n"); */
	/* inordertree(root); */
	/* printf("\n"); */
	/* dealloc(&root); */
	cal_time(&root);
	return 0;
}

void cal_time(BTnode **root){
	time_t begin;
	time(&begin);
	printf("%ld\n",begin);
	FILE *fp = fopen("/home/ubuntuhp/projects/C/testdata.txt","r+");
	if(!fp){
		perror("file not found\n");
		return;
	}
	int data;
	while(!feof(fp)){
		fscanf(fp,"%d",&data);
		insert(root,data);
	}
	fclose(fp);
	time_t end;
	time(&end);
	printf("%ld\n",end);
	printf("time diff is:%ld\n",end - begin);
	dealloc(root);
}
BTnode *createnode(int num){
    BTnode *new = malloc(sizeof(*new));
    new->value = num;
    new->left = new->right = NULL;
    return new;
}

void preordertree(BTnode *node){
    //recursive approach
    if(!node) return;
    printf("%d ",node->value);
    preordertree(node->left);
    preordertree(node->right);
}

void inordertree(BTnode *node){
	if(!node) return;
	inordertree(node->left);
	printf("%d ",node->value);
	inordertree(node->right);
}
bool insert(BTnode **root,int num){
	if(!(*root)){
		*root = createnode(num); 
		return true;
	}
	BTnode *cur= *root;
	while(cur->value != num){
		if(num < cur->value){
			if(!cur->left)
				cur->left = createnode(num);
			else
				cur = cur->left;
		}
		else{
			if(!cur->right)
				cur->right = createnode(num);
			else 
				cur = cur->right;
		}
	}
}
void dealloc(BTnode **root){
     if(!(*root)) return;
    dealloc(&((*root)->left));
    dealloc(&((*root)->right));

    if(!((*root)->left) && !((*root)->right)){
        free(*root);
        *root = NULL;
        return;
    } 
}
