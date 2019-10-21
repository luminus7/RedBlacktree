
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
//Binary Search Tree


typedef struct _RBTree{
	int value;
	int color;
	struct _RBTree *left, *right;
}RBT;

typedef RBT * treepointer;

/*
 * search for node (same as find or retrieve)
 * if node exist, return this node pointer 
 */
RBT* search(RBT *node_ptr, int value)
{
	//if(!node_ptr){
	if(node_ptr == NULL){
		return NULL;
	}

	if(node_ptr->value < value){ 	//search right
		printf("go right\n");
		return search(node_ptr->right,value);
	}
	else if(node_ptr->value > value){ 	//search left
		printf("go left\n");
		return search(node_ptr->left,value);
	}
	else if(node_ptr->value == value){
	//else{
		return node_ptr;
	}
}

/*
 * insert node
 * return must be root node 
 */
RBT* insert(RBT *root, int value)
{
	treepointer node_ptr = root;
	treepointer new_node;

	if(!root){		//예외처리?
		new_node = (RBT *)malloc(sizeof(RBT));
		new_node->value = value;
		new_node->left = NULL;
		new_node->right = NULL; 

		return new_node;
	}

	if(search(root, value)){
		printf("this value : %d , already exist\n",value);
		return root;
	}

	new_node = (RBT *)malloc(sizeof(RBT));
	new_node->value = value;
	new_node->left = NULL;
	new_node->right = NULL; 
	
	// insert value
	while(TRUE){
		if(node_ptr->value < value){
			if(node_ptr->right == NULL){
				node_ptr->right = new_node;
				break;
			}else{
				node_ptr = node_ptr->right;
			}
		}else if(node_ptr->value > value){
			if(node_ptr->left== NULL){
				node_ptr->left = new_node;
				break;
			}else{
				node_ptr = node_ptr->left;
			}
		}
	}

	return root;

}

/*
 * delete node(same as remove)
 * input: treepointer *root, int value
 * return node_ptr
 */
RBT* successor(RBT *node_ptr)		//find minimum value of right-subtree (find successor)
{					//there is another way to find replacing node (find predecessor)
//	if(node_ptr == NULL)
//		return NULL;
//	while(TRUE){
//		node_ptr = node_ptr->left;
//		if(node_ptr->left == NULL){
//			break;
//		}
//	}
//	return node_ptr;
	if(node_ptr == NULL)
		return NULL;
	else if(node_ptr->left != NULL)
 		return successor(node_ptr->left);
 	return node_ptr;

}

RBT* delete(RBT *node_ptr, int value)
{	
	//search 함수를 사용해서 구현해보고 싶다.
	//searching for the item to be deleted
	printf("delete start\n");
	if(node_ptr == NULL){			//예외처리
		return NULL;
	}

	if (node_ptr->value < value){
			printf("go right\n");
			node_ptr->right = delete(node_ptr->right, value);
			return node_ptr;	//없어도 됨
	}
	else if(node_ptr->value > value){
			printf("go left\n");
			node_ptr->left = delete(node_ptr->left, value);
			return node_ptr; 	//업어도 됨
	}else{
		if(node_ptr->left == NULL && node_ptr->right == NULL)		//no child
		{								//delete childless node or delete successor
			printf("no child\n");
			free(node_ptr);
			return NULL;
		
		}else if(node_ptr->left == NULL || node_ptr->right == NULL){		//one child
			printf("one child\n");
			RBT *temp;
	    		if(node_ptr->left == NULL)
			{
				printf("right child\n");
				temp = node_ptr->right;
			}else{
				printf("left child\n");
				temp = node_ptr->left;
			}
			free(node_ptr);
    			return temp;
		}else{							//two children
			printf("two child\n");
			RBT *temp = successor(node_ptr->right);
			//successor로 값을 넘길 때 node_ptr만 넘겨도 되도록 해보고 싶다.
	   		node_ptr->value = temp->value;
	    		node_ptr->right = delete(node_ptr->right, temp->value);
		}
	}
	printf("-----return-----\n");
	return node_ptr;
}



/* 
 * traversal
 * return nothing
 */
void pre_order(treepointer ptr){ //전위 트리 순회
		if(ptr){
			printf("%d\n", ptr->value);  //D
			pre_order(ptr->left);  //L
			pre_order(ptr->right);  //R
		}
	}
void in_order(treepointer ptr){ //중위 트리 순회
		if(ptr){
			in_order(ptr->left);  //L
			printf("%d ", ptr->value);  //D
			in_order(ptr->right);  //R
		}
	}
void post_order(treepointer ptr){ //후위 트리 순회
		if(ptr){
			post_order(ptr->left);  //L
			post_order(ptr->right);  //R
			printf("%d\n", ptr->value);  //D
		}
	}
void level_order(){ //레벨 순서 순회
		//미구현
		printf("sorry still in progress");
}

void traversal(treepointer ptr){
	printf("Choose how to traverse along this tree\n [preorder: 1, inorder: 2, postorder: 3, levelorder: 4]");
	int a;
	scanf("%d", &a);

	if(a == 1){
		pre_order(ptr);
	}else if(a == 2){
		in_order(ptr);
	}else if(a == 3){
		post_order(ptr);
	}else if(a == 4){
		level_order();
	}else{
		printf("올바른 숫자를 입력해주세요\n");
	//	traversal(ptr);
	}
}

int main(){
	RBT *root = NULL;
	int new_value;
	int delete_value;
	int count = 0;

		printf("Insert value: ");
		while(scanf("%d", &new_value)){
				count ++;
				root = insert(root, new_value);
				printf("Insert value: ");
			if(count > 10) break;
		}
		in_order(root);
		printf("\nDelete value: ");
		scanf("%d", &delete_value);
		delete(root,delete_value);	
		in_order(root);
/*		
		if(a == y){
	       		printf("Delete value: ");
			while(scanf("%d", &delete_value)){
				delete(root, delete_value);
				in_order(root);
				printf("Delete value: ");
			}
		}	
*/	
	return 0;
}

