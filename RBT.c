/*
  Red Black Trees
  (C) 2019 SangJin LEE <isangjin77@gmail.com>

  Implementation of Red Black tree to understand, How RB properties make sure of O(log n) time complexity

  so humble code,,
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
//Red Black Tree

typedef struct _RBTree{
	int value;
	char color;
	struct _RBTree *left, *right, *parent;
}RBT;

typedef struct _RBroot{
	struct _RBTree *root_ptr;
}RBroot;

void SwapColor(RBT *x){
	if(x->color == 'R')
		x->color = 'B';
	else
		x->color = 'R';
}

/*
 * move upper codes to RBT.h
 */

void __rotate_left(RBT *node, RBroot *root)
{
	RBT *r_child = node->right;
	
	if((node->right = r_child->left))	
		r_child->left->parent = node;
	
	r_child->left = node;

	if((r_child->parent = node->parent))
	{
		if(node == node->parent->left)
			node->parent->left = r_child;
		else
			node->parent->right = r_child;
	
	}else
		root->root_ptr = r_child;
	node->parent = r_child;
}

void __rotate_right(RBT *node, RBroot *root)
{
	RBT *l_child = node->left;
	
	if((node->left = l_child->right))	
		l_child->right->parent = node;
	
	l_child->right = node;

	if((l_child->parent = node->parent))
	{
		if(node == node->parent->right)
			node->parent->right = l_child;
		else
			node->parent->left = l_child;
	
	}else
		root->root_ptr = l_child;
	node->parent = l_child;
}



void LL_restruct(RBT *x, RBroot *root)
{
	RBT *GP, *P, *U, *temp;
	GP = x->parent->parent;
	P = x->parent;
	U = GP->right;
	__rotate_right(GP, root);
	
	SwapColor(GP);
	SwapColor(P);
}

void RR_restruct(RBT *x, RBroot *root)
{
	RBT *GP, *P, *U, *temp;
	GP = x->parent->parent;
	P = x->parent;
	U = GP->left;	
/*
	if(GP->parent)
	{	
		printf("HEY GP->parent exist~!\n");
		if(GP == GP->parent->left)
		{	
			printf("if GP->parent exist!! // GP is lchild\n");
			__rotate_left(GP, root);
		}
		else if(GP == GP->parent->right)
		{	
			printf("if GP->parent exist!! // GP is lchild\n");
			__rotate_left(GP, root);
		}
	}
	else //(!(GP->parent))  //GP was root node!!
	{
*///		printf("GP->parent does not exist...ToT\n");
		__rotate_left(GP, root);
//	}
	SwapColor(GP);
	SwapColor(P);
}


void LR_restruct(RBT *x, RBroot *root)
{
	RBT *GP, *P, *U, *temp;
	GP = x->parent->parent;
	P = x->parent;
	U = GP->right;
	__rotate_left(P, root);
	
	//apply LL case
	LL_restruct(P, root);

}


void RL_restruct(RBT *x, RBroot *root)
{
	RBT *GP, *P, *U, *temp;
	GP = x->parent->parent;
	P = x->parent;
	U = GP->right;

	//right rotate(small)
	__rotate_right(P, root);
	RR_restruct(P, root);

}

void restruct(RBT *x, RBroot *root)
{
		if(x->parent == x->parent->parent->left)
		{
			if(x == x->parent->left){		//LL case	right rotate(axis?), swap color
				printf("LL case\n");
				LL_restruct(x, root);
			}else if(x == x->parent->right){	//LR case	left rotate, apply LLcase
				printf("LR case\n");
				LR_restruct(x, root);
			}
		}
		else if(x->parent == x->parent->parent->right)
		{
			if(x == x->parent->right)		//RRcase	left rotate, swap color
			{	printf("RR case\n");
				RR_restruct(x, root);
			}else if(x == x->parent->left){		//RL case	right rotate, apply RRcase
				printf("RL case\n");
				RL_restruct(x, root);
			}
		}
}

RBT *recolor(RBT *x, RBroot *root)
{
	RBT *GP, *P, *U;
	P = x->parent;
	GP = P->parent;
	while(x != root->root_ptr && P->color == 'R' && GP != NULL)
	{
		GP = P->parent;
		printf("start while\n");
		if(P == GP->left){
			printf("******GP->left == P case\n");
			U = GP->right;
		}else{
			printf("******GP->right == P case\n");
			U = GP->left;
		}

		if(U->color == 'R')
		{	
			printf("******simple recolor...can be propagated\n");
			SwapColor(GP);
			SwapColor(P);
			SwapColor(U);
			if(GP == root->root_ptr)
				GP->color = 'B';
		}else{
			printf("propagate...restruct\n");
			restruct(x, root);
		}
		printf("asd\n");
		x = GP;
		P = x->parent;

		printf("end of while\n");
	}

		printf("current line : %d\n current func : %s\n",__LINE__, __func__);
	if(root->root_ptr->color == 'R')
		SwapColor(root->root_ptr);
	return root->root_ptr;
}


//fixup tree when each time of insertion occurs for each inserted node
//double r을 root부터 내려가며 항상 조사해줄 필요가 없을듯... 매 insert마다 InsertFixup로 고쳐줄거고 recoloring은 자체적으로 propagate시킬거니까!
RBT* InsertFixup(RBT *x, RBroot *root) //여기에 RBT **root 써놨었는데 왜 그랬을까?
{
	if(x->parent != NULL && x->parent->parent != NULL)
	{
		printf("FIXING.....\n");
		RBT *GP, *P, *U;
		GP = x->parent->parent;
		P = x->parent;
		if(GP->left == P)
			U = GP->right;
		else if(GP->right == P)
			U = GP->left;
		//나중에 U가 NULL인 경우도 따져줘야곘네
			
		if(x->color == 'R' && P->color == 'R')
		{
			if(U != NULL)
			{
				if(U->color == 'R'){ 				//recolor
					printf("recoloring case\n");
					root->root_ptr = recolor(x, root);
					printf("recoloring end...\n");
				}
				else if(U->color == 'B' || U == NULL){		//restruct
					printf("restructing start....\n");
					restruct(x, root);
				}
			}else if(U == NULL){
				restruct(x, root);
			}

		if(!root)
			return NULL;
		}
	}	
	
}

RBT* search(RBT *node_ptr, int value)
{
	if(node_ptr == NULL){
		printf("No such value in this tree\n");
		return NULL;
	}

	if(node_ptr->value < value)
		return search(node_ptr->right,value);
	else if(node_ptr->value > value)
		return search(node_ptr->left,value);
	else if(node_ptr->value == value)
		return node_ptr;
}

RBT* insert(RBroot *root, int value)
{
	printf("*****insert start***** %d\n",value);
	RBT *new_node;
        RBT *node_ptr = root->root_ptr;
	RBT *U;

	//make root node
	if(!(root->root_ptr))
	{
		new_node = (RBT *)malloc(sizeof(RBT));
		new_node->value = value;
		printf("%d\n", new_node->value);
		new_node->color = 'B';
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->parent = NULL;
		
		return new_node;
	}
	if(search(root->root_ptr,value))
	{
		printf("this value: %d, already exist\n",value);
		return node_ptr;
	}

	//make new node and insert it on proper location
	new_node = (RBT *)malloc(sizeof(RBT));
	new_node->value = value;
	new_node->color = 'R';
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;

	//search func[recursive ver]써서 위치 찾아줄 수도 있음
	while(TRUE){
		if(node_ptr->value < value){
			if(node_ptr->right == NULL){
	  			node_ptr->right = new_node;
	  			new_node->parent = node_ptr;
				break;
  			}else{
				node_ptr = node_ptr->right;
			}
		}else if(node_ptr->value > value){
  			if(node_ptr->left== NULL){
				node_ptr->left = new_node;
				new_node->parent = node_ptr;
				break;
			}else{
				node_ptr = node_ptr->left;
			}
		}
	}
	printf("Insertion END.....\n");
	InsertFixup(new_node, root);
	printf("InsertFixupEND.....\n");
	return root->root_ptr;
}
//inorder successor in BST (how about predecessor? is it optional?)
RBT* successor(RBT *node_ptr)
{
	RBT *r_child = node_ptr->right;
	if(r_child  == NULL)
		return NULL;
	while(TRUE){
		r_child = r_child->left;
		if(r_child->left == NULL)
			break;
	}
	return r_child;
}

RBT* predecessor(RBT *node_ptr)
{
	RBT *l_child = node_ptr->left;
	if(l_child == NULL)
		return NULL;
	while(TRUE){
		l_child = l_child->right;
		if(l_child->right == NULL)
			break;
	}
	return l_child;
}

//Simple BST delete(is it Inorder BST delete?)
//implement with while and compare with this code
//does double pointer necessary?
RBT* BSTdelete(RBroot *root, int value)
{
	RBT *node_ptr = root->root_ptr;
	RBT *node_alter;		//alternate node
	printf("BST delete start\n");

	node_ptr = search(node_ptr, value);

	if(node_ptr == NULL)
		return NULL;

	if(node_ptr->right == NULL && node_ptr->left == NULL){
		node_alter = node_ptr->parent;
		free(node_ptr);
		node_alter->left = NULL;
		node_ptr = node_alter;
	}else if(node_ptr->right != NULL && node_ptr->left == NULL){
		node_alter = successor(node_ptr);
		node_ptr->value = node_alter->value;
		free(node_alter);
	}else if(node_ptr->right == NULL && node_ptr->left != NULL){
		node_alter = predecessor(node_ptr);
		node_ptr->value = node_alter->value;
		free(node_alter);
	}else{
		node_alter = successor(node_ptr);
		node_ptr->value = node_alter->value;
		free(node_alter);
	}
	return node_ptr;
}

//sibling of double black is Red
void Redsib(RBroot *root, RBT *x)
{
	RBT *P, *sib;
	P = x->parent;
	//right sibling
	if(P->left == x && P->right != NULL)
	{
		sib = P->right;
		SwapColor(sib);
		SwapColor(P);
		__rotate_left(P, root);
	}
	//left sibling
	else if(P->left != NULL && P->right == x)
	{	
		sib = P->left;
		SwapColor(sib);
		SwapColor(P);
		__rotate_right(P, root);
	}
}

//sibling of doubleblack is Black (three cases occur)
/*
 * sibling of doubleblack is Black
 * both children of sibling are Black
 */
void Blacksib_caseA(RBroot *root, RBT *x)
{
	RBT *P, *sib;
	P = x->parent;
	//right sibling
	if(P->left == x && P->right != NULL)
	{
		sib = P->right;
		SwapColor(sib);
	}
	//left sibling
	else if(P->left != NULL && P->right == x)
	{	
		sib = P->left;
		SwapColor(sib);
	}
}

/*
 * sibling of doubleblack is Black
 * r_child of sibling is Black
 * l_child of sibling is Red
 */
void Blacksib_caseB(RBroot *root, RBT *x)
{
	RBT *P, *sib, *sib_l_child;
	P = x->parent;
	//right sibling
	if(P->left == x && P->right != NULL)
	{
		sib = P->right;
		sib_l_child = sib->left;
		SwapColor(sib);
		SwapColor(sib_l_child);
		__rotate_right(sib, root);
	}
	//left sibling
	else if(P->left != NULL && P->right == x)
	{	
		sib = P->left;
		sib_l_child = sib->left;
		SwapColor(sib);
		SwapColor(sib_l_child);
		__rotate_right(sib, root);
	}
}

/*
 * sibling of doubleblack is Black
 * r_child of sibling is RED
 */
void Blacksib_caseC(RBroot *root, RBT *x)
{
	RBT *P, *sib, *sib_r_child;
	P = x->parent;
	//right sibling
	if(P->left == x && P->right != NULL)
	{	
		sib = P->right;
		sib_r_child = P->right;
		sib->color = P->color;
		if(P->color = 'R')
			SwapColor(P);
		SwapColor(sib_r_child);
		__rotate_left(P, root);
	}
	//left sibling
	else if(P->left != NULL && P->right == x)
	{	
		sib = P->left;
		sib_r_child = P->right;
		sib->color = P->color;
		if(P->color = 'R')
			SwapColor(P);
		SwapColor(sib_r_child);
		__rotate_left(P, root);
	}
}

/*
 * fixup tree after BST delete to follow RB properties
 * used when double_black occurs (;when BST delete get rid of Black ode)
 * variable RBT *x represents for double_black node
 */
RBT* DeleteFixup(RBroot *root, RBT *x, RBT *x_parent)
{
	printf("Fixing after BST delete...\n");
	RBT *P, *Sib, *r_child, *l_child;
	P = x_parent;
	while(TRUE)
	{	
	printf("deletion while start...");
		if(x == P->right){
			printf("current line : %d\n current func : %s\n",__LINE__, __func__);
			Sib = P->left;
		}
		else if(x == P->left)
		{
			printf("current line : %d\n current func : %s\n",__LINE__, __func__);
			Sib = P->right;
		}

		if(Sib->right != NULL && Sib->left != NULL)
		{
			printf("current line : %d\n current func : %s\n",__LINE__, __func__);
			r_child = Sib->right;
			l_child = Sib->left;
		}
		else if(Sib->right == NULL && Sib->left != NULL)
		{
			printf("current line : %d\n current func : %s\n",__LINE__, __func__);
			r_child = NULL;
			l_child = Sib->left;
		}
		else if(Sib->right != NULL && Sib->left == NULL)
		{
			printf("current line : %d\n current func : %s\n",__LINE__, __func__);
			r_child = Sib->right;
			l_child = NULL;
		}

				printf("current line : %d\n current func : %s\n",__LINE__, __func__);
		//red sibling
		if(Sib->color == 'R')
		{
				printf("current line : %d\n current func : %s\n",__LINE__, __func__);
			Redsib(root, x);
		}
		else//you can NEVER access to NULL pointer in these cases)
		{		
			//arghhhhhhhhhhhhhhhhhhhhhhhhhh!!!!!!!!!!!! damn
				printf("current line : %d\n current func : %s\n",__LINE__, __func__);
				if(r_child->color == 'B' && l_child->color == 'B')	//Black sibling, both Black children
				{
				printf("current line : %d\n current func : %s\n",__LINE__, __func__);
					Blacksib_caseA(root, x);
				}
				else if((r_child == NULL || r_child->color == 'B') && l_child->color =='R')	//Black sibling, Black r_child(l_nephew), Red l_child(l_nephew)
				{
				printf("current line : %d\n current func : %s\n",__LINE__, __func__);
					Blacksib_caseB(root, x);
				}
				else
				{		
				printf("current line : %d\n current func : %s\n",__LINE__, __func__);	//Black sibling, Red r_child(r_nephew)
					Blacksib_caseC(root, x);
				}
		}
				printf("current line : %d\n current func : %s\n",__LINE__, __func__);
	}
}

RBT* delete(RBroot *root, int value)
{
	printf("*****delete start***** %d\n",value);
	RBT *node_ptr, *node_alter, *node_ptr_parent;
	node_ptr = search(root->root_ptr, value);
	node_ptr_parent = node_ptr->parent;
	node_alter = successor(node_ptr);

	if(node_ptr == NULL)
		return NULL;

	if(node_ptr->right == NULL && node_ptr->left == NULL){
		//double black occurs
		BSTdelete(root, value);
		DeleteFixup(root, node_alter, node_ptr_parent);
	}else{
		if(node_alter->color == 'R'){
			BSTdelete(root, value);
		}else{
			node_alter = BSTdelete(root, value);
			DeleteFixup(root, node_alter, node_ptr_parent);
		}
	}
	return root->root_ptr;
}

/*
 * traversal
 * (only inorder)
 */
RBT* inorder(RBT *ptr)
{
	if(ptr){
		inorder(ptr->left);		//L
		printf("%d ", ptr->value);	//D
		inorder(ptr->right);		//R
	}
}

RBT* inorder_color(RBT *ptr)
{
	if(ptr){
		inorder_color(ptr->left);
		printf("%c ", ptr->color);
		inorder_color(ptr->right);
	}
}

void insert_check(RBroot *root)
{
	printf("Inorder Traversal is: ");
	inorder(root->root_ptr);
	printf("\nInorder Color is: ");
	inorder_color(root->root_ptr);
	printf("\n\n");
}


int main()
{
	int value;
	RBroot *root = (RBroot *)malloc(sizeof(RBroot)); //한번만 만들어 주면 되니까 그냥 main에서 만들어버리자.
	
		//scanf("Insert value: %d", &insert_value);
		printf("current line : %d\n current func : %s\n",__LINE__, __func__);
		root->root_ptr = insert(root,10);
		printf("root clr\n");
		insert_check(root);
		insert(root,15);
		printf("second clr\n");
		insert_check(root);
		insert(root,30);
		printf("third clr\n");
		insert_check(root);
		insert(root,35);
		printf("fourth clr\n");
		insert_check(root);
		insert(root,6);
		printf("fifth clr\n");
		insert_check(root);
		insert(root,75);
		printf("sixth clr\n");
		insert_check(root);
		insert(root,33);
		printf("seventh clr\n");
		insert_check(root);
		insert(root,28);
		printf("eighth clr\n");
		insert_check(root);
		insert(root,9);
		printf("ninth clr\n");
		insert_check(root);
		insert(root,17);
		printf("tenth clr!\n");
		insert_check(root);
		insert(root,13);
		printf("eleventh clr\n");
		insert_check(root);
		insert(root,4);
		printf("twelfth clr\n");
		insert_check(root);
		insert(root,25);
		printf("thirtheenth clr\n");
		insert_check(root);
		insert(root,27);
		printf("fourteenth clr\n");
		insert_check(root);
		printf("\n");

		//scanf("Delete value: %d",&delete_value);
		delete(root,17);
		insert_check(root);
		printf("\n");
	return 0;
}
