#ifndef CS240_PAGES_H
#define CS240_PAGES_H

#include "Page.h"

//!  PageNode implements a binary search tree node
class PageNode {
	friend class Pages; //!< Pages can access private members of PageNode
	friend class TreeIterator;

public:

	//!  Constructor
	PageNode(Page* const p) :
		page(p), left(NULL), right(NULL) {
	}

	~PageNode(){
		delete page;
	}
	//!  Read-only public methods for use by clients of the BST class
	Page* GetPage() const {
		return page;
	}

	PageNode * GetLeft() const {
		return left;
	}

	PageNode * GetRight() const {
		return right;
	}

private:
	Page* page; //!< Page stored in the node
	PageNode * left; //!< pointer to the node's left child
	PageNode * right; //!< pointer to the node's right child
};

class StackNode{
	friend class TreeStack;
	StackNode(PageNode* n, StackNode* s) : v(n), next(s){}
	PageNode* v;
	StackNode* next;
};

class TreeStack{
	friend class TreeIterator;
public:
	TreeStack (){
		count = 0;
		top = NULL;
	}
	~TreeStack(){
		Empty();
	}
	int Size(){
		return count;
	}
	bool IsEmpty(){
		return count == 0;
	}
	bool Push(PageNode* n){
		StackNode* temp = new StackNode(n, top);
		top = temp;
		count++;
		return true;
	}
	PageNode* Pop(){
		if (top == NULL){
			return NULL;
		}
		PageNode* temp = top->v;
		StackNode* garbage = top;
		top = top->next;
		delete garbage;
		count--;
		return temp;
	}
	bool Empty(){
		while (top != NULL){
			StackNode* garbage = top;
			top = top->next;
			delete garbage;
		}
		count = 0;
		return true;
	}
private:
	StackNode* top;
	int count;
	
	
};

//! A BST of all the Pages parsed

class Pages{
public:
	Pages();
	Pages(Pages& other);
	~Pages();
	//! @return The pointer containing the sought-after page or NULL if not found.
	bool Find(const std::string & url) const;
	//! Everyone outta the pool!
	void Clear();
	PageNode * GetRoot() const;

	//!  @return true if the BST is empty, or false if the BST is not empty
	bool IsEmpty() const;

	//!  @return the number of Pages in the BST
	int GetSize() const;
	
	

	//!  Inserts page page into the BST
	//!
	//!  @param page The new page being inserted
	//!
	//!  @return a pointer to the newly inserted node, or NULL if page was already
	//!          in the tree (i.e., NULL is used to indicate a duplicate insertion)
	PageNode * Insert(Page* const p);
	
	static bool Test(std::ostream & os);
	
	class TreeIterator{
	public:
		TreeIterator(PageNode* root){
			//stack.Push(root);
			StackEmUp(root);
		}
		void StackEmUp(PageNode* n){
			if (n->right != NULL){
				StackEmUp(n->right);
			}
			stack.Push(n);
			if (n->left != NULL){
				StackEmUp(n->left);
			}
		}
		bool HasNext(){
			return !stack.IsEmpty();
		}
		Page* Next(){
			PageNode* n = stack.Pop();
			/*if (n->right != NULL){
				stack.Push(n->right);
			}
			if (n->left != NULL){
				stack.Push(n->left);
			}*/
			return n->GetPage();
		}
		
	private:
		TreeStack stack;
	};
	TreeIterator* Iterator();

private:
	PageNode * Insert(Page* const p, PageNode* n);
	bool Find(const std::string & url, PageNode* n) const;
	void Free(PageNode* n);
	PageNode* root;
	PageNode* toAdd;
	int size;
};


#endif
