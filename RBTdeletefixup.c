static void __rb_erase_color(RBT * x, RBT * P, RBroot * root)
{
	RBT * Sib;

	while ((!x || x->color == 'B') && x != root->root_ptr)//operate til doubleblack node is Black, and not root
	{
		if (P->left == x) //doubleblack is left child of its parent (parent->left exist); RIGHT SIBLING
		{
			Sib = P->right; //sibling is named as other in this code
			if (Sib->color == 'R') //Case 1 (Red sibling)
			{
				Sib->color = 'B';
				P->color = 'R';
				__rotate_left(P, root);
				Sib = P->right; //What the...
			}
			if ((!Sib->left || Sib->left->color == 'B') && (!Sib->right || Sib->right->color == 'B')) //Case 2-A
			{
				Sib->color = 'R';
				x = P;
				P = x->parent;
			}
			else //Case 2-B and Case 2-C
			{
				if (!Sib->right || Sib->right->color == 'B') // Case 2-B right_nephew is NULL or Black node with value
				{
					RBT * l_nephew;
					if ((l_nephew = Sib->left)) //Sib->left is not NIL node
						l_nephew->color = 'B';
					Sib->color = 'R';
					__rotate_right(Sib, root);
					Sib = P->right; //What the...
				}
				Sib->color = P->color; //Case 2-C
				P->color = 'B';
				if (Sib->right) //why..?
					Sib->right->color = 'B';
				__rotate_left(P, root);
				x = root->root_ptr;
				break;
			}
		}
		else //mirror case(double black is right child); LEFT SIBLING
		{
			Sib = P->left;
			if (Sib->color == 'R')
			{
				Sib->color = 'B';
				P->color = 'R';
				__rotate_right(P, root);
				Sib = P->left;
			}
			if ((!Sib->left || Sib->left->color == 'B') && (!Sib->right || Sib->right->color == 'B'))
			{
				Sib->color = 'R';
				x = P;
				P = x->parent;
			}
			else
			{
				if (!Sib->left || Sib->left->color == 'B')
				{
					RBT * r_nephew; //원래 register RBT *r_nephew 였음..
					if ((r_nephew = Sib->right))
						r_nephew->color = 'B';
					Sib->color = 'R';
					__rotate_left(Sib, root);
					Sib = P->left;
				}
				Sib->color = P->color;
				P->color = 'B';
				if (Sib->left)
					Sib->left->color = 'B';
				__rotate_right(P, root);
				x = root->root_ptr;
				break;
			}
		}
	}
	if (x)
		x->color = 'B';
}
void rb_erase(RBroot *root, int value)
{
	printf("deleting: %d\n", value);
	RBT *child, *P, *x;
	x = search(root->root_ptr, value);
	char color;

	if (!x->left)
		child = x->right;
	else if (!x->right)
		child = x->left;
	else
	{
		RBT * old = x, * left;

		x = x->right;
		while ((left = x->left))
			x = left;
		child = x->right;
		P = x->parent;
		color = x->color;

		if (child)
			child->parent = P;
		if (P)
		{
			if (P->left == x)
				P->left = child;
			else
				P->right = child;
		}
		else
			root->root_ptr = child;

		if (x->parent == old)
			P = x;
		x->parent = old->parent;
		x->color = old->color;
		x->right = old->rb_right;
		x->left = old->rb_left;

		if (old->parent)
		{
			if (old->parent->left == old)
				old->parent->left = x;
			else
				old->parent->right = x;
		} else
			root->root_ptr = x;

		old->left->parent = x;
		if (old->right)
			old->right->parent = x;
		goto color;
	}

	P = x->parent;
	color = x->color;

	if (child)
		child->parent = parent;
	if (P)
	{
		if (P->left == x)
			P->left = child;
		else
			P->right = child;
	}
	else
		root->root_ptr = child;

 color://?
	if (color == 'B')
		__rb_erase_color(child, P, root);
}

