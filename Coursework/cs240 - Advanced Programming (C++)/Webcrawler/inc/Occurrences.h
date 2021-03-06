#ifndef CS240_OCCURRENCES_H
#define CS240_OCCURRENCES_H

#include <iostream>
#include "URL.h"

//!  OccMapNode implements a tree map node for the Occurrences class
class OccMapNode {
	friend class Occurrences; //!< Occurrences can access private members of OccMapNode
	friend class OccIterator;

public:

	//!  Constructor
	OccMapNode(URL* const v) :
		urlKey(v), countValue(1), left(NULL), right(NULL) {
	}

	//! Copy Constructor
	OccMapNode(const OccMapNode & other) :
		urlKey(other.urlKey), countValue(other.countValue), left(other.left), right(other.right) {
	}
	~OccMapNode(){
		//delete urlKey;
	}

	//!  Read-only public methods for use by clients of the Occurrences class
	URL* GetUrlKey() const {
		return urlKey;
	}
	
	const int GetCountValue() const {
		return countValue;
	}
	
	/*void Increment(){
		countValue++;
	}*/
	OccMapNode * GetLeft() const {
		return left;
	}

	OccMapNode * GetRight() const {
		return right;
	}

	//! Assignment operator
	OccMapNode & operator=(const OccMapNode & other) {
		if (this != &other) {
			urlKey = other.urlKey;
			left = other.left;
			right = other.right;
		}

		return *this;
	}

private:
	URL* urlKey; //!< urlKey stored in the node
	int countValue;
	OccMapNode * left; //!< pointer to the node's left child
	OccMapNode * right; //!< pointer to the node's right child
};

class OccStackNode{
	friend class OccStack;
	OccStackNode(OccMapNode* n, OccStackNode* s) : v(n), next(s){}
	OccMapNode* v;
	OccStackNode* next;
};

class OccStack{
	friend class OccIterator;
public:
	OccStack (){
		count = 0;
		top = NULL;
	}
	~OccStack(){
		Empty();
	}
	int Size(){
		return count;
	}
	bool IsEmpty(){
		return count == 0;
	}
	bool Push(OccMapNode* n){
		OccStackNode* temp = new OccStackNode(n, top);
		top = temp;
		count++;
		return true;
	}
	OccMapNode* Pop(){
		if (top == NULL){
			return NULL;
		}
		OccMapNode* temp = top->v;
		OccStackNode* garbage = top;
		top = top->next;
		delete garbage;
		count--;
		return temp;
	}
	bool Empty(){
		while (top != NULL){
			OccStackNode* garbage = top;
			top = top->next;
			delete garbage;
		}
		count = 0;
		return true;
	}
private:
	OccStackNode* top;
	int count;
	
	
};

//!  Occurrences implements a binary search tree
class Occurrences {

public:

	//!  No-arg constructor.  Initializes an empty Occurrences
	Occurrences();


	//!  Destructor
	~Occurrences();

	//!  @return a pointer to the root node of the tree, or NULL if the tree is empty.
	//!  @note This is useful for Occurrences clients that need to traverse the tree.)
	OccMapNode * GetRoot() const;

	//!  @return true if the Occurrences is empty, or false if the Occurrences is not empty
	bool IsEmpty() const;

	//!  Removes all urlKeys from the Occurrences
	void Clear();

	//!  @return the number of urlKeys in the Occurrences
	int GetSize() const;

	//!  Inserts urlKey v into the Occurrences
	//!
	//!  @param v The new urlKey being inserted
	//!
	//!  @return a pointer to the newly inserted node, or NULL if v was already
	//!          in the tree (i.e., NULL is used to indicate a duplicate insertion)
	OccMapNode * Insert(URL* const v);

	//!  Searches the tree for urlKey v
	//!
	//!  @param v The new urlKey being searched for
	//!
	//!  @return a pointer to the node containing v, or NULL if v is not in the tree
	OccMapNode * Find(const std::string & v) const;

	static bool Test(std::ostream & os);
	
	class OccIterator{
	public:
		OccIterator(OccMapNode* root){
			//stack.Push(root);
			StackEmUp(root);
		}
		void StackEmUp(OccMapNode* n){
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
		OccMapNode* Next(){
			OccMapNode* n = stack.Pop();
			/*if (n->right != NULL){
				stack.Push(n->right);
			}
			if (n->left != NULL){
				stack.Push(n->left);
			}*/
			return n;
		}
		
	private:
		OccStack stack;
	};
	OccIterator* Iterator();

private:
	OccMapNode * Insert(URL* const v, OccMapNode* n);
	//!  Copy constructor.  Makes a complete copy of its argument
	Occurrences(const Occurrences & other);
	//!  Assignment operator.  Makes a complete copy of its argument
	//!  @return Reference to oneself
	Occurrences& operator =(const Occurrences & other);
	OccMapNode* Find(const std::string & v, OccMapNode* n) const;
	OccMapNode* Copy (const OccMapNode * n);
	void Free(OccMapNode* n);
	OccMapNode* root;
	OccMapNode* toAdd;
	int size;
};

#endif
