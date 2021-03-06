#ifndef CS240_WORD_INDEX_H
#define CS240_WORD_INDEX_H

#include "Occurrences.h"

//!  WIMapNode implements a tree map node for the WordIndex class
class WIMapNode {
	friend class WordIndex;
	friend class IndexIterator;

public:

	//!  Constructor
	WIMapNode(const std::string & v) : wordKey(v), left(NULL), right(NULL) {
		occMap = new Occurrences;
	}
	~WIMapNode(){
		delete occMap;
	}

	//!  Read-only public methods for use by clients of the WordIndex class
	const std::string & GetWordKey() const {
		return wordKey;
	}
	
	Occurrences* const GetOccurrences() const {
		return occMap;
	}
	
	WIMapNode * GetLeft() const {
		return left;
	}

	WIMapNode * GetRight() const {
		return right;
	}

private:
	std::string wordKey; //!< wordKey stored in the node
	Occurrences*  occMap;
	WIMapNode * left; //!< pointer to the node's left child
	WIMapNode * right; //!< pointer to the node's right child
	WIMapNode(const WIMapNode & other) :
		wordKey(other.wordKey), occMap(other.occMap), left(other.left), right(other.right) {
	}
	WIMapNode & operator=(const WIMapNode & other) {
		if (this != &other) {
			wordKey = other.wordKey;
			left = other.left;

			right = other.right;
		}

		return *this;

	}
};

class WIStackNode{
	friend class WIStack;
	WIStackNode(WIMapNode* n, WIStackNode* s) : v(n), next(s){}
	WIMapNode* v;
	WIStackNode* next;
};

class WIStack{
	friend class IndexIterator;
public:
	WIStack (){
		count = 0;
		top = NULL;
	}
	~WIStack(){
		Empty();
	}
	int Size(){
		return count;
	}
	bool IsEmpty(){
		return count == 0;
	}
	bool Push(WIMapNode* n){
		WIStackNode* temp = new WIStackNode(n, top);
		top = temp;
		count++;
		return true;
	}
	WIMapNode* Pop(){
		if (top == NULL){
			return NULL;
		}
		WIMapNode* temp = top->v;
		WIStackNode* garbage = top;
		top = top->next;
		delete garbage;
		count--;
		return temp;
	}
	bool Empty(){
		while (top != NULL){
			WIStackNode* garbage = top;
			top = top->next;
			delete garbage;
		}
		count = 0;
		return true;
	}
private:
	WIStackNode* top;
	int count;
	
	
};

//!  WordIndex implements a binary search tree
class WordIndex {

public:

	//!  No-arg constructor.  Initializes an empty WordIndex
	WordIndex();

	//!  Copy constructor.  Makes a complete copy of its argument
	//WordIndex(const WordIndex & other);

	//!  Destructor
	~WordIndex();

	//!  Assignment operator.  Makes a complete copy of its argument
	//!  @return Reference to oneself
	//WordIndex& operator =(const WordIndex & other);

	//!  @return a pointer to the root node of the tree, or NULL if the tree is empty.
	//!  @note This is useful for WordIndex clients that need to traverse the tree.)
	WIMapNode * GetRoot() const;

	//!  @return true if the WordIndex is empty, or false if the WordIndex is not empty
	bool IsEmpty() const;

	//!  Removes all wordKeys from the WordIndex
	void Clear();

	//!  @return the number of wordKeys in the WordIndex
	int GetSize() const;

	//!  Inserts wordKey v into the WordIndex
	//!
	//!  @param v The new wordKey being inserted
	//!
	//!  @return a pointer to the newly inserted node, or NULL if v was already
	//!          in the tree (i.e., NULL is used to indicate a duplicate insertion)
	WIMapNode * Insert(const std::string & v, URL* const url);

	//void AddIndex(const std::string & word, const std::string & url);
	//!  Searches the tree for wordKey v
	//!
	//!  @param v The new wordKey being searched for
	//!
	//!  @return a pointer to the node containing v, or NULL if v is not in the tree
	WIMapNode * Find(const std::string & v) const;

	//! @NOTE: YOU ARE NOT REQUIRED TO IMPLEMENT THE Remove METHOD BELOW
	//!        (BUT YOU CAN IF YOU WANT TO)
	//!
	//!  Removes wordKey v from the tree
	//!
	//!  @param v The wordKey being removed from the tree
	//!
	//!  @return true if v was removed from the tree, or false if v was not in the tree
	//bool Remove(const std::string & v);
	
	static bool Test(std::ostream & os);
	
	class IndexIterator{
	public:
		IndexIterator(WIMapNode* root){
			//stack.Push(root);
			StackEmUp(root);
		}
		void StackEmUp(WIMapNode* n){
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
		WIMapNode* Next(){
			WIMapNode* n = stack.Pop();
			/*if (n->right != NULL){
				stack.Push(n->right);
			}
			if (n->left != NULL){
				stack.Push(n->left);
			}*/
			return n;
		}
		
	private:
		WIStack stack;
	};
	IndexIterator* Iterator();

private:
	
	WIMapNode * Insert(const std::string & v, URL* const url, WIMapNode* n);
	WIMapNode* Find(const std::string & v, WIMapNode* n) const;
	WIMapNode* Copy (const WIMapNode * n);
	void Free(WIMapNode* n);
	WIMapNode* root;
	WIMapNode* toAdd;
	int size;
};

#endif
