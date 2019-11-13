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
	__rotate_left(GP, root);
	
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
	
	LL_restruct(P, root);
}


void RL_restruct(RBT *x, RBroot *root)
{
	RBT *GP, *P, *U, *temp;
	GP = x->parent->parent;
	P = x->parent;
	U = GP->right;
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
		x = GP;
		P = x->parent;

		printf("end of while\n");
	}

	if(root->root_ptr->color == 'R')
		SwapColor(root->root_ptr);
	return root->root_ptr;
}


//fixup tree when each time of insertion occurs for each inserted node
//double r을 root부터 내려가며 항상 조사해줄 필요가 없을듯... 매 insert마다 InsertFixup로 고쳐줄거고 recoloring은 자체적으로 propagate시킬거니까!
RBT* InsertFixup(RBT *x, RBroot *root)
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

void __rb_erase_color(RBT *x, RBT *P, RBroot *root)
{
	RBT *Sib;

	while((!x || x->color == 'B') && x != root->root_ptr)
	{
		if(P->left == x) //doubleblack is left child of its parent: RIGHT SIBLING
		{
			Sib = P->right;
			if(Sib->color == 'R') //Case 1 (Red sibling)
			{
				printf("current line : %d\n current func : %s\n",__LINE__, __func__);
				Sib->color = 'B';
				P->color = 'R';
				__rotate_left(P, root);
				Sib = P->right;
			}
	
			//Case2 start
			if((!Sib->left || Sib->left->color == 'B') && (!Sib->right || Sib->right->color == 'B')) //Case 2-A
			{
				Sib->color = 'R';
				
				x = P;
				P = x->parent;
			}
			else //Case 2-B and Case 2-C
			{
				if(!Sib->right || Sib->right->color == 'B') // Case 2-B right_nephew is NULL or Black node with value
				{
					RBT * l_nephew;
					if((l_nephew = Sib->left))
					{
						l_nephew->color = 'B';
					}
					Sib->color = 'R';
					__rotate_right(Sib, root);
					Sib = P->right;
				}
				Sib->color = P->color; //Case 2-C
				P->color = 'B';
				if(Sib->right) //check if r_nep is NULL, if not change r_nep color to Black
				{
					Sib->right->color = 'B';
				}
				__rotate_left(P, root);
				x = root->root_ptr;
				break;
			}
		}
		else //mirror case(double black is right child); LEFT SIBLING
		{
			Sib = P->left;
			if(Sib->color == 'R')
			{
				Sib->color = 'B';
				P->color = 'R';
				__rotate_right(P, root);
				Sib = P->left;
			}
			if((!Sib->left || Sib->left->color == 'B') && (!Sib->right || Sib->right->color == 'B'))
			{
				Sib->color = 'R';
				x = P;
				P = x->parent;
			}
			else
			{
				if(!Sib->left || Sib->left->color == 'B')
				{
					RBT * r_nephew;
					if((r_nephew = Sib->right))
					{
						r_nephew->color = 'B';
					}
					Sib->color = 'R';
					__rotate_left(Sib, root);
					Sib = P->left;
				}
				Sib->color = P->color;
				P->color = 'B';
				if(Sib->left)
				{
					Sib->left->color = 'B';
				}
				__rotate_right(P, root);
				x = root->root_ptr;
				break;
			}
		}
	}
	if(x)
	{
		x->color = 'B';
	}
}

void rb_erase(RBroot *root, int value) //simple BST delete
{
	printf("deleting: %d\n", value);
	RBT *child, *P, *x, *temp;
	temp = x = search(root->root_ptr, value);
	char color;
	
	//x is node to delete
	if(!x->left) //deal with both NIL children case && only left child case
	{
		child = x->right;
	}
	else if(!x->right) //deal with only right child case
	{
		child = x->left;
	}
	else //deleting node with 2 children
	{
		RBT *old = x, *left; //old is node to delete

		x = x->right; //finding successor(left-most node of right subtree)
		while((left = x->left))
		{
			x = left;
		}
		child = x->right;
		P = x->parent;
		color = x->color;

		if(child)
		{
			child->parent = P;
		}
		if(P)
		{
			if(P->left == x)
			{
				P->left = child;
			}
			else
			{
				P->right = child;
			}
		}
		else
			root->root_ptr = child;

		if(x->parent == old)
		{
			P = x;
		}
		//replace node old with successor node x
		x->parent = old->parent; 
		x->color = old->color; 
		x->right = old->right;
		x->left = old->left;

		if(old->parent)
		{
			if(old->parent->left == old)
			{		
				old->parent->left = x;
			}
			else
			{
				old->parent->right = x;
			}
		}else{
			root->root_ptr = x;
		}

		old->left->parent = x;
		if(old->right)
		{
			old->right->parent = x;
		}
		goto color;
	}
		
	P = x->parent;
	color = x->color;

	if(child)
	{
		child->parent = P;
	}
	if(P)
	{
		if (P->left == x)
		{
			P->left = child;
		}else{
			P->right = child;
		}
	}
	else
		root->root_ptr = child;

 color:
	if (color == 'B')
	{
		__rb_erase_color(child, P, root);
	}
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
		rb_erase(root,17);
		insert_check(root);
		rb_erase(root,15);
		insert_check(root);
		rb_erase(root,13);
		insert_check(root);
		printf("\n");
	return 0;
}
