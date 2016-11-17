#ifndef	AVL_TREE
#define AVL_TREE
#include <iostream>
#include <iomanip>
using namespace std;

template <class T,class Compare>
class AVL_Tree{
protected :
	/*
	*	�����
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
	*	header->parentָ����ڵ㣬
	*	header->left ָ����������ߣ�
	*	header_>right ָ���������ұ�
	*/
	Link header; 
	long node_count;

private:
	/*
	*	����AVL��
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
	*	������Ҷ��
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
	*	ͨ����ת����ƽ�����ӻָ�AVL����
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
		}//���1���������ȵ�ƽ�����Ӷ��ǡ�=��
		else if((ancestor->balanceFactor =='L'&& !compare(item,ancestor->item)) || 
			(ancestor->balanceFactor =='R' && compare(item,ancestor->item)))
		{
			ancestor->balanceFactor = '=';
			adjustPath(root,inserted);
		}//���2���ں�ancestor��ƽ�������෴�������н��в���
		else if(ancestor->balanceFactor == 'R' && !compare(item,ancestor->right->item))
		{
			ancestor->balanceFactor = '=';
			rotateLeft(ancestor);
			adjustPath(ancestor->parent,inserted);
		}//���3����ancestor�����������������ֲ���
		else if(ancestor->balanceFactor == 'L'&& compare(item,ancestor->left->item))
		{
			ancestor->balanceFactor = '=';
			rotateRight(ancestor);
			adjustPath(ancestor->parent,inserted);
		}//���4����ancestor�����������������ֲ���
		else if(ancestor->balanceFactor == 'L' && !compare(item,ancestor->left->item))
		{
			rotateLeft(ancestor->left);
			rotateRight(ancestor);
			adjustLeftRight(ancestor,inserted);
		}//���5����ancestor�������������������в���
		else
		{
			rotateRight(ancestor->right);
			rotateLeft(ancestor);
			adjustRightLeft(ancestor,inserted);
		}//���6����ancestor�������������������в���
	}//	fixAfterlnsertion
	/*
	*	����ת
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
	*	����ת
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
	*	����inserted���������� �� to���������� ��ƽ������
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
	*	����������ת֮�󣬵���inserted���������� �� ancestor���������� ��ƽ������
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
		} // item ">" ancestor�ĸ��׵���
		else
		{
			ancestor->balanceFactor = '=';
			ancestor->parent->right->balanceFactor = 'R';
			adjustPath(ancestor,inserted);
		} //item "<" ancestor �ĸ��׵���
	}// adjustRightLeft

	/*
	*	����������ת֮�󣬵���inserted���������� �� ancestor���������� ��ƽ������
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
		} // item "<" ancestor�ĸ��׵���
		else
		{
			ancestor->balanceFactor = '=';
			ancestor->parent->left->balanceFactor = 'L';
			adjustPath(ancestor,inserted);
		} //item ">" ancestor �ĸ��׵���
	}// adjustLeftRight
	/*
	*	ɾ��link���ڽ��
	*
	*/
	void deleteLink(Link& link)
	{
		if(link->left == NULL || link->right== NULL)	//link��ָ�������������һ����Ů
			prune(link);
		else
		{
			deleteSuccessor(link);
		}//	link����������
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
		}//link ����Ϊ��
		else
		{
			Link temp = link;
			while(temp->left != NULL)
				temp = temp->left;
			successor = temp->item;
			link1->item = successor;
			prune(temp->parent->left);
		} //link �������գ���������link->right������ˣ�����ֵ����successor,Ȼ��ɾ����
	}//deleteSuccessor 

	/*
	*	linkָ�������������һ����Ů
	*	ɾ��link��ָ�����
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
				// ���¼�������ߵ���
		}//	link ���,�Ҳ���
		else
		{
			link =link->left;
			link->parent = linktoDel->parent;
			if(linktoDel == header->right)	
				header->right = link;
			// ���¼������ұߵ���
		}//	link �ҿգ��󲻿�
		//�������ֵ�����ʹlinkΪҶ��
		fixAfterErase(x,x->parent);
		delete linktoDel;
	}//	prune
	/**
	*	��Ҷ��link��ʼ�����ƣ�ֱ������header ���� ����ancestorƽ��('=')
	*	ͨ����ת����ƽ�����ӣ��ָ�AVL����
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
			}//link Ϊ��ڵ㣬���丸�ڵ���ǡ��������Ҷ
			else if(!compare(link->item,ancestor->item) && ancestor->balanceFactor == '=')
			{
				cout<<""<<endl;
				ancestor->balanceFactor = 'L';	
				break;
			}//link Ϊ�ҽڵ㣬���丸�ڵ���ǡ��������Ҷ
			else if(compare(link->item,ancestor->item) && ancestor->balanceFactor == 'R')
			{
				if(ancestor->right->balanceFactor == 'L')
				{
					rotateRight(ancestor->right);
					rotateLeft(ancestor);
					ancestor->balanceFactor = '=';
					ancestor->parent->right->balanceFactor = '=';
					//ancestor->parentΪroot�����߶ȼ�С1
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->right ����ҵ�
				else if(ancestor->right->balanceFactor == 'R')
				{
					rotateLeft(ancestor);
					ancestor->balanceFactor = '=';
					ancestor->parent->balanceFactor = '=';
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->right ����Ҹ�
				else if(ancestor->right->balanceFactor  == '=')
				{
					rotateLeft(ancestor);
					ancestor->balanceFactor = 'R';
					ancestor->parent->balanceFactor = 'L';
					link = NULL;
					break;
				}//ancestor->right ƽ��
			}//link Ϊ��ڵ㣬���丸�ڵ�����Ҹ�
			else if(!compare(link->item,ancestor->item) && ancestor->balanceFactor == 'L')
			{
				if(ancestor->left->balanceFactor == 'R')
				{
					rotateLeft(ancestor->left);
					rotateRight(ancestor);	
					ancestor->balanceFactor = '=';
					ancestor->parent->left->balanceFactor = '=';
					//��ancestor->parentΪroot�����߶ȼ�С1
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->left ����Ҹ�
				else if(ancestor->left->balanceFactor == 'L')
				{
					rotateRight(ancestor);
					ancestor->balanceFactor = '=';
					ancestor->parent->balanceFactor = '=';
					//��ancestor->parentΪroot�����߶ȼ�С1
					link = ancestor->parent;
					ancestor = link->parent;
				}//ancestor->left ����ҵ�
				else if(ancestor->left->balanceFactor  == '=')
				{
					rotateRight(ancestor);
					ancestor->balanceFactor = 'L';
					ancestor->parent->balanceFactor = 'R';
					//link = NULL;
					break;
				}//ancestor->left ƽ��
			}//link Ϊ�ҽڵ㣬���丸�ڵ�����ҵ�
			else if((!compare(link->item,ancestor->item) && ancestor->balanceFactor == 'R') ||
				(compare(link->item,ancestor->item) && ancestor->balanceFactor == 'L'))
			{
				ancestor->balanceFactor = '=';
				link = ancestor;
				ancestor = ancestor->parent;
			}	//link Ϊ�ҽڵ㣬���丸�ڵ�����Ҹ� �� link Ϊ��ڵ㣬���丸�ڵ�����ҵ�
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
	*	��ӡAVL���ݹ�ʵ��
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
	*	 AVL�������Ŀ
	*
	*/
	int size() const
	{
		return node_count;
	}//size


	/**
	*	����ͼ�鵥����Ϣ����,�����ҵ����������ĵ�һ��������������end()
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
			}//��child->item���ڵ���itemʱִ��
			else
				child=child->right;
		}//while
		if(parent == header || compare(item ,parent->item))
			return end();
		else
			return parent;
	}//find

	/**
	*	����һ��ͼ����Ϣ
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
		}//��������
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
			}//��parent�������в���
			else
			{
				insertLeaf(item,parent,parent->right);
				fixAfterlnsertion(ancestor,parent->right);
				if(header->right == parent)
					header->right = parent->right;
				return parent->right;
			}//��parent���Ҳ���в���
		}//���ǿ�
	}//insert



	/*
	*	ɾ��itr���ڽ��
	*
	*/
	void erase(Iterator itr)
	{
		if(itr.nodePtr->parent->parent == itr.nodePtr)	//itrλ�ڸ����
			deleteLink(itr.nodePtr->parent->parent);
		else if (itr.nodePtr->parent->left == itr.nodePtr)	//itrλ��һ������Ů��
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
	*	��ӡAVL��
	*
	*/
	void graph(ostream& out)const
	{
		graphAux(out,0,header->parent);
	}
	/*
	*	AVL���߶�
	*
	*/
	int height()
	{
		return Cur_height(header->parent);
	}
	/*
	*	AVL������
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
		*	����������� operator*
		*/
		T& operator*()const{
			return nodePtr->item;
		}//operator*
		/*
		*	ǰ++ ��linkΪ�����ұ�ʱ������header����end()
		*/
		Iterator&  operator ++()
		{
			Link tempLink;
			if((nodePtr->right)!= NULL)
			{	
				nodePtr = nodePtr->right;
				while((nodePtr->left)!=NULL)
					nodePtr = nodePtr->left;
			}// �ڵ�������Ů
			else
			{
				tempLink = nodePtr->parent;
				while(nodePtr == tempLink->right)
				{
					nodePtr = tempLink;
					tempLink = tempLink->parent;
				}//�����ƶ�������������,nodePtr�ĺ�����nodePtr������ߵ����ȵĸ���
				// ��linkΪ�����ұ�ʱ������header����end()
			if((nodePtr->right)!=tempLink)
				nodePtr = tempLink;
			}//
			return *this;
		}//prefix++

		/*
		*	��++
		*/
		Iterator&  operator ++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		} // postfix ++

		/*
		*	ǰ--����linkΪ�������ʱ������header����end()
		*/
		Iterator&  operator--()
		{
			if (nodePtr->isHeader)
				nodePtr = nodePtr->right; //�������ұ�
			else if (nodePtr->left != NULL)
			{
				Link y = nodePtr->left;
				while (y->right != NULL)
					y = y->right;
				nodePtr = y;
			} // ��������
			else
			{
				Link y = nodePtr->parent;
				while (nodePtr == y->left)
				{
					nodePtr = y;
					y = y->parent;
				} //�����ƶ����������ң�nodePtr��ǰ����nodePtr�����ұߵ�����
				//	��linkΪ�������ʱ������header
				nodePtr = y;
			} //  ����Ϊ��
			return *this;
		} // pre-decrement operator

		/*
		*	��--
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
	*	���ǿգ�����������С��ĵ����������򷵻غ�end()����ֵ��ͬ�ĵ�����
	*
	*/
	Iterator beign(){
		return header->left;
	}
	/*
	*	���ǿգ�����ǰһλ���Ͼ���������ĵ�����
	*
	**/
	Iterator end(){
		return header;
	}
	/*
	*	����С�����˳�����,����<<
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