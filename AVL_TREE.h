#ifndef	AVL_TREE
#define AVL_TREE
#include <iostream>
#include <iomanip>
using namespace std;

template <class T,class Compare>
class AVL_Tree{
protected :
	/*
	*	树结点
	*
	*/
	class tree_node{
	public:
		T item;
		tree_node* parent,*left,*right;
		bool isHeader;
		char balanceFactor;
	};
	typedef tree_node* Link;
	Compare compare;
	/**
	*	header->parent指向根节点，
	*	header->left 指向树的最左边，
	*	header_>right 指向树的最右边
	*/
	Link header; 
	long node_count;

private:
	/*
	*	析构AVL树
	*
	*/
	void destroy(Link link)
	{
		if(link != NULL)
		{
			destroy(link->left);
			destroy(link->right);
			delete link;
		}
	}//destroy


	/**
	*	插入新叶子
	*
	**/
	void insertLeaf(const T& item,Link parent,Link& child)
	{
		child = new tree_node;
		child->balanceFactor = '=';
		child->isHeader = false;
		child->item = item;
		child->left = NULL;
		child->right = NULL;
		child->parent = parent;
		node_count++;
	}//insertLeaf

	/**
	*	通过旋转调整平衡因，子恢复AVL属性
	*
	**/
	void fixAfterlnsertion(Link ancestor,Link inserted)
	{
		Link root = header->parent;
		T item = inserted->item;
		if(ancestor == NULL)
		{
			if(compare(item ,root->item))
				root->balanceFactor = 'L';
			else
				root->balanceFactor = 'R';
			adjustPath(root,inserted);
		}//情况1：所有祖先的平衡因子都是‘=’
		else if((ancestor->balanceFactor =='L'&& !compare(item,ancestor->item)) || 
			(ancestor->balanceFactor =='R' && compare(item,ancestor->item)))
		{
			ancestor->balanceFactor = '=';
			adjustPath(root,inserted);
		}//情况2：在和ancestor的平衡因子相反的子树中进行插入
		else if(ancestor->balanceFactor == 'R' && !compare(item,ancestor->right->item))
		{
			ancestor->balanceFactor = '=';
			rotateLeft(ancestor);
			adjustPath(ancestor->parent,inserted);
		}//情况3：在ancestor的右子树的右子树种插入
		else if(ancestor->balanceFactor == 'L'&& compare(item,ancestor->left->item))
		{
			ancestor->balanceFactor = '=';
			rotateRight(ancestor);
			adjustPath(ancestor->parent,inserted);
		}//情况4：在ancestor的左子树的左子树种插入
		else if(ancestor->balanceFactor == 'L' && !compare(item,ancestor->left->item))
		{
			rotateLeft(ancestor->left);
			rotateRight(ancestor);
			adjustLeftRight(ancestor,inserted);
		}//情况5：在ancestor的左子树的右子树进行插入
		else
		{
			rotateRight(ancestor->right);
			rotateLeft(ancestor);
			adjustRightLeft(ancestor,inserted);
		}//情况6：在ancestor的右子树的左子树进行插入
	}//	fixAfterlnsertion
	/*
	*	左旋转
	*
	*/
	void rotateLeft(Link x)	
	{
		Link y = x->right;
		x->right = y->left;
		if(y->left != NULL)
			y->left->parent = x;
		y->parent = x->parent;
		if(x == header->parent)
			header->parent = y;
		else if(x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}//rotateLeft
	/*
	*	右旋转
	*
	*/
	void rotateRight(Link x)	
	{
		tree_node* y = x->left;
		x->left = y->right;
		if(y->right != NULL)
			y->right->parent = x;
		y->parent = x->parent;
		if(x == header->parent)
			header->parent = y;
		else if(x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}//rotateRight

	/*
	*	调节inserted（不包括） 到 to（不包括） 的平衡因子
	*
	*/
	void adjustPath(Link to, Link inserted)		
	{
		T item = inserted->item;
		Link temp = inserted->parent;
		while(temp != to)
		{
			if(compare(item,temp->item))
				temp->balanceFactor = 'L';
			else
				temp->balanceFactor ='R';
			temp = temp->parent;
		}//while
	}//adjustPath

	/*
	*	进行右左旋转之后，调节inserted（不包括） 到 ancestor（不包括） 的平衡因子
	*
	*/
	void adjustRightLeft(Link ancestor, Link inserted)	
	{
		T item =inserted->item;
		if(ancestor->parent == inserted)
			ancestor->balanceFactor = '=';
		else if(!compare(item,ancestor->parent->item))
		{
			ancestor->balanceFactor ='L';
			adjustPath(ancestor->parent->right,inserted);
		} // item ">" ancestor的父亲的项
		else
		{
			ancestor->balanceFactor = '=';
			ancestor->parent->right->balanceFactor = 'R';
			adjustPath(ancestor,inserted);
		} //item "<" ancestor 的父亲的项
	}// adjustRightLeft

	/*
	*	进行左右旋转之后，调节inserted（不包括） 到 ancestor（不包括） 的平衡因子
	*
	*/
	void adjustLeftRight(Link ancestor, Link inserted)
	{
			T item =inserted->item;
		if(ancestor->parent == inserted)
			ancestor->balanceFactor = '=';
		else if(compare(item,ancestor->parent->item))
		{
			ancestor->balanceFactor ='R';
			adjustPath(ancestor->parent->left,inserted);
		} // item "<" ancestor的父亲的项
		else
		{
			ancestor->balanceFactor = '=';
			ancestor->parent->left->balanceFactor = 'L';
			adjustPath(ancestor,inserted);
		} //item ">" ancestor 的父亲的项
	}// adjustLeftRight
	/*
	*	删除link所在结点
	*
	*/
	void deleteLink(Link& link)
	{
		if(link->left == NULL || link->right== NULL)	//link项指向的子树至多有一个子女
			prune(link);
		else
		{
			deleteSuccessor(link);
		}//	link项有两子树
	}//	deleteLink
	void deleteSuccessor(Link& link1)
	{
		T successor;
		Link link = link1->right;
		if(link->left == NULL)
		{
			successor = link->item;
			link1->item = successor;
			prune(link);
		}//link 左树为空
		else
		{
			Link temp = link;
			while(temp->left != NULL)
				temp = temp->left;
			successor = temp->item;
			link1->item = successor;
			prune(temp->parent->left);
		} //link 左树不空，往下移至link->right的最左端，将此值赋给successor,然后删除它
	}//deleteSuccessor 

	/*
	*	link指向的子树至多有一个子女
	*	删除link所指向的项
	*/
	void prune(Link& link)
	{
		Link linktoDel = link,x = link;
		node_count--;
		if((link->left == NULL) && (link->right == NULL))
		{
			if(link == header ->left)
				header->left = link->parent; // new leftmost
			if(link == header->right)
				header->right = link->parent; // new rightmost
			if(link->parent->left == link)
			{
				link->parent->left = NULL;
				//x = x->parent;
			//	fixAfterErase(x,x->parent);
				link = NULL;
			}
			else
			{
				link->parent->right = NULL;
			//	x = x->parent;
				//fixAfterErase(x,x->parent);
				link = NULL;
				
			}
			
		}//link is a leaf
		else if (link->left == NULL)
		{
			link = link->right;
			link->parent = linktoDel->parent;
			if(linktoDel == header->left)	
				header->left = link;
				// 重新计算最左边的项
		}//	link 左空,右不空
		else
		{
			link =link->left;
			link->parent = linktoDel->parent;
			if(linktoDel == header->right)	
				header->right = link;
			// 重新计算最右边的项
		}//	link 右空，左不空
		//经过各种调整，使link为叶子
		fixAfterErase(x,x->parent);
		delete linktoDel;
	}//	prune
	/**
	*	从叶子link开始向上移，直到到达header 或者 遇到ancestor平衡('=')
	*	通过旋转调整平衡因子，恢复AVL属性
	*
	**/
	void fixAfterErase(Link& link,Link& ancestor)
	{
		while(!ancestor->isHeader)
		{
			if(compare(link->item,ancestor->item) && ancestor->balanceFactor == '=')
			{	
				ancestor->balanceFactor = 'R';
				break;
			}//link 为左节点，且其父节点有恰有左右子叶
			else if(!compare(link->item,ancestor->item) && ancestor->balanceFactor == '=')
			{
				cout<<""<<endl;
				ancestor->balanceFactor = 'L';	
				break;
			}//link 为右节点，且其父节点有恰有左右子叶
			else if(compare(link->item,ancestor->item) && ancestor->balanceFactor == 'R')
			{
				if(ancestor->right->balanceFactor == 'L')
				{
					rotateRight(ancestor->right);
					rotateLeft(ancestor);
					ancestor->balanceFactor = '=';
					ancestor->parent->right->balanceFactor = '=';
					//ancestor->parent为root的树高度减小1
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->right 左高右低
				else if(ancestor->right->balanceFactor == 'R')
				{
					rotateLeft(ancestor);
					ancestor->balanceFactor = '=';
					ancestor->parent->balanceFactor = '=';
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->right 左低右高
				else if(ancestor->right->balanceFactor  == '=')
				{
					rotateLeft(ancestor);
					ancestor->balanceFactor = 'R';
					ancestor->parent->balanceFactor = 'L';
					link = NULL;
					break;
				}//ancestor->right 平衡
			}//link 为左节点，且其父节点左低右高
			else if(!compare(link->item,ancestor->item) && ancestor->balanceFactor == 'L')
			{
				if(ancestor->left->balanceFactor == 'R')
				{
					rotateLeft(ancestor->left);
					rotateRight(ancestor);	
					ancestor->balanceFactor = '=';
					ancestor->parent->left->balanceFactor = '=';
					//以ancestor->parent为root的树高度减小1
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->left 左低右高
				else if(ancestor->left->balanceFactor == 'L')
				{
					rotateRight(ancestor);
					ancestor->balanceFactor = '=';
					ancestor->parent->balanceFactor = '=';
					//以ancestor->parent为root的树高度减小1
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->left 左高右低
				else if(ancestor->left->balanceFactor  == '=')
				{
					rotateRight(ancestor);
					ancestor->balanceFactor = 'L';
					ancestor->parent->balanceFactor = 'R';
					//link = NULL;
					break;
				}//ancestor->left 平衡
			}//link 为右节点，且其父节点左高右低
			else if((!compare(link->item,ancestor->item) && ancestor->balanceFactor == 'R') ||
				(compare(link->item,ancestor->item) && ancestor->balanceFactor == 'L'))
			{
				ancestor->balanceFactor = '=';
				link = ancestor;
				ancestor = ancestor->parent;
			}	//link 为右节点，且其父节点左低右高 或 link 为左节点，且其父节点左高右低
		}//while
	}//fixAfterErase
	/* 
	*	Postcondition: The tree has been checked to make sure that the
    *                heights of the left and right subtrees (if they
    *                exist) are within 1 of each other, and that those
    *                subtrees are AVL trees.
	*/
	bool checkAVLTree (Link root)
	 {
		 if (root == NULL)
			 return true;
		else if ((abs (height (root -> left) - height (root -> right)) <= 1)
                 && (checkAVLTree (root -> left))
                 && (checkAVLTree (root -> right)))
				 return true;
		 return false;
	 } // checkAVLTree

	/*
	*	打印AVL树递归实现
	*
	*/
	void graphAux(ostream & out ,int indent, Link subtreeRoot) const
	{
		if(subtreeRoot != NULL)
		{
			graphAux(out,indent+8, subtreeRoot->right);
			out<<setw(indent)<<" "<<subtreeRoot->item<<subtreeRoot->balanceFactor<<endl;
			graphAux(out,indent+8, subtreeRoot->left);
		}
	}

	/*
	*	the height of items in the AVLtree
	*
	*/
	int Cur_height(const Link& root)
	{
		int leftHeight,rightHeight;
		if(root == NULL)
			return -1;
		else
		{
			leftHeight = Cur_height(root->left);
			rightHeight = Cur_height(root->right);
			if(leftHeight > rightHeight)
				return 1+leftHeight;
			return 1 + rightHeight;
		} // else
	}// height
public:
	class Iterator;
	friend class Iterator;
	AVL_Tree()
	{
		node_count = 0;
		header = new tree_node;
		header->parent = NULL;
		header->left = header;
		header->right = header;
		header->isHeader = true;
	} //defalut constructor
	~AVL_Tree(){
		destroy(header->parent);
	}


	/*
	*	 AVL树结点数目
	*
	*/
	int size() const
	{
		return node_count;
	}//size


	/**
	*	根据图书单项信息查找,返回找到符合条件的第一个迭代器，否则end()
	*
	**/
	Iterator find(const T& item)
	{
		Link parent = header; 
		Link child = header->parent;
		while(child!=NULL){
			if(!compare(child->item,item))
			{
				parent = child;
				if(!compare(item,child->item))
					break;
				child =child->left;
			}//当child->item大于等于item时执行
			else
				child=child->right;
		}//while
		if(parent == header || compare(item ,parent->item))
			return end();
		else
			return parent;
	}//find

	/**
	*	插入一条图书信息
	*
	**/
	Iterator insert(const T& item)
	{
		if(header->parent == NULL)
		{
			insertLeaf(item,header,header->parent);
			header->left = header->parent;
			header->right = header->parent;
			return header->parent;
		}//根部插入
		else
		{
			Link parent = header, child = header->parent,ancestor = NULL;
			while(child!=NULL)
			{
				parent = child;
				if(child->balanceFactor !='=')
					ancestor = child;
				if(compare(item,child->item))
					child = child->left;
				else
					child = child ->right;
			}
			if(compare(item,parent->item))
			{
				insertLeaf(item,parent,parent->left);
				fixAfterlnsertion(ancestor,parent->left);
				if(header->left == parent)
					header->left = parent->left;
				return parent->left;
			}//在parent的左侧进行插入
			else
			{
				insertLeaf(item,parent,parent->right);
				fixAfterlnsertion(ancestor,parent->right);
				if(header->right == parent)
					header->right = parent->right;
				return parent->right;
			}//在parent的右侧进行插入
		}//树非空
	}//insert



	/*
	*	删除itr所在结点
	*
	*/
	void erase(Iterator itr)
	{
		if(itr.nodePtr->parent->parent == itr.nodePtr)	//itr位于根结点
			deleteLink(itr.nodePtr->parent->parent);
		else if (itr.nodePtr->parent->left == itr.nodePtr)	//itr位于一个左子女处
			deleteLink(itr.nodePtr->parent->left);
		else
			deleteLink(itr.nodePtr->parent->right);
	}//	erase


	/*
	*	Postcondition: true has been returned if this tree
	*	really is an AVLTree.  Otherwise, false
	*	has been returned.
	*/
	bool isAVLTree() 
	{ 
		return checkAVLTree( header -> parent); 
	}

	/*
	*	打印AVL树
	*
	*/
	void graph(ostream& out)const
	{
		graphAux(out,0,header->parent);
	}
	/*
	*	AVL树高度
	*
	*/
	int height()
	{
		return Cur_height(header->parent);
	}
	/*
	*	AVL迭代器
	*
	*
	*/
	class Iterator{
		tree_node* nodePtr;
	public:
		friend class AVL_Tree<T,Compare>;
		Iterator (){}
		Iterator(Link newptr):nodePtr(newptr){}
		bool operator==(const Iterator& itr)const
		{
			return nodePtr == itr.nodePtr;
		}//operator==
		bool operator!=(const Iterator& itr)const
		{
			return nodePtr != itr.nodePtr;
		}// operator!=
		/*
		*	脱引用运算符 operator*
		*/
		T& operator*()const{
			return nodePtr->item;
		}//operator*
		/*
		*	前++ 当link为在最右边时，返回header，即end()
		*/
		Iterator&  operator ++()
		{
			Link tempLink;
			if((nodePtr->right)!= NULL)
			{	
				nodePtr = nodePtr->right;
				while((nodePtr->left)!=NULL)
					nodePtr = nodePtr->left;
			}// 节点有右子女
			else
			{
				tempLink = nodePtr->parent;
				while(nodePtr == tempLink->right)
				{
					nodePtr = tempLink;
					tempLink = tempLink->parent;
				}//向上移动并尽可能向左,nodePtr的后任是nodePtr的最左边的祖先的父亲
				// 当link为在最右边时，返回header，即end()
			if((nodePtr->right)!=tempLink)
				nodePtr = tempLink;
			}//
			return *this;
		}//prefix++

		/*
		*	后++
		*/
		Iterator&  operator ++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		} // postfix ++

		/*
		*	前--，当link为在最左边时，返回header，即end()
		*/
		Iterator&  operator--()
		{
			if (nodePtr->isHeader)
				nodePtr = nodePtr->right; //返回最右边
			else if (nodePtr->left != NULL)
			{
				Link y = nodePtr->left;
				while (y->right != NULL)
					y = y->right;
				nodePtr = y;
			} // 左树不空
			else
			{
				Link y = nodePtr->parent;
				while (nodePtr == y->left)
				{
					nodePtr = y;
					y = y->parent;
				} //向上移动尽可能向右，nodePtr的前继是nodePtr的最右边的祖先
				//	当link为在最左边时，返回header
				nodePtr = y;
			} //  左树为空
			return *this;
		} // pre-decrement operator

		/*
		*	后--
		*/
		Iterator&  operator--(int)
		{
			Iterator temp = *this;
			--(*this);
			return temp;
		} // postfix --
		~Iterator(){}
	};//Iterator


	/*
	*	若非空，返回树的最小项的迭代器，否则返回和end()返回值相同的迭代器
	*
	*/
	Iterator beign(){
		return header->left;
	}
	/*
	*	若非空，返回前一位置上就是最大的项的迭代器
	*
	**/
	Iterator end(){
		return header;
	}
	/*
	*	按从小到大的顺序输出,重载<<
	*
	*/
	friend ostream& operator<<(ostream& out,AVL_Tree<T,Compare>& tree)
	{
		for(Iterator itr = tree.begin();itr!=tree.end();itr++)
		{
		out<<*itr<<endl;
		}
		return out;
	}
};


#endif