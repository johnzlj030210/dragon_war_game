#pragma once

// for security, T can only be integer, pointer, etc, but not be struct or class
template <class T> class CGQueue
{
	typedef struct _list
	{
		void *_pre;
		void *_post;
		T	 node;
	}LIST;
public:
	CGQueue(void);
	virtual ~CGQueue(void);
	bool push(T t);
	bool push(T t, short &index);
	bool pop(T &t);
	bool head(T &t);
	bool next(T &t);
	bool neighbor(T t, T &tout);
	bool erase(T t);
	bool fetch(T &t, short index);
	void entry(void);
	void leave(void);
	void entry2(void);
	void leave2(void);
	bool insert_after(T t, T t1);
	bool insert_before(T t, T t1);
	bool insert_current(T t);
	bool from(T t);
	bool prior(T &t);
	void clear(void);
private:
	LIST *root;
	LIST *curr, *last, *temp1, *temp2;
};

template <class T> CGQueue<T>::CGQueue(void)
: curr(NULL)
, last(NULL)
, temp1(NULL)
, temp2(NULL)
{
	root = new LIST;
	memset(root, 0, sizeof(LIST));
}

template <class T> CGQueue<T>::~CGQueue(void)
{
	LIST *q = root;
	while(q!= NULL)
	{
		LIST *q1 = q;
		q = (LIST *)q1->_post;
		delete q1;
	}
}

template <class T> void CGQueue<T>::clear(void)
{
	LIST *q = (LIST *)root->_post;
	while(q!= NULL)
	{
		LIST *q1 = q;
		q = (LIST *)q1->_post;
		delete q1;
	}
	memset(root, 0, sizeof(LIST));
}

template <class T> bool CGQueue<T>::push(T t)
{
	LIST *q1 = root;
	LIST *q2;

	while(q1!=NULL)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
	}
	q1 = new LIST;
	q1->_post = NULL;
	q1->node = t;

	q1->_pre = q2;
	q2->_post = q1;

	return true;
}

template <class T> bool CGQueue<T>::push(T t, short &index)
{
	LIST *q1 = root;
	LIST *q2;

	index = 0;

	while(q1!=NULL)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
		index ++;
	}
	q1 = new LIST;
	q1->_post = NULL;
	q1->node = t;

	q1->_pre = q2;
	q2->_post = q1;

	return true;
}

template <class T> bool CGQueue<T>::pop(T &t)
{
	LIST *q1 = (LIST *)root->_post;
	LIST *q2 = (LIST *)q1->_post;

	root->_post = q2;
	q2->_pre = root;

	t = q1->node;
	delete q1;

	return true;
}

template <class T> bool CGQueue<T>::head(T &t)
{
	curr = (LIST *)root->_post;
	last = (LIST *)root;
	if(curr)
		t = curr->node;
	else
		return false;
	return true;
}

template <class T> bool CGQueue<T>::next(T &t)
{
	if(curr==NULL)
		return false;
	last = curr;
	curr = (LIST *)curr->_post;
	if(curr)
		t = curr->node;
	else
		return false;
	return true;
}

//get the next to the t. If end reached, go to head
template <class T> bool CGQueue<T>::neighbor(T t, T &tout)
{
	bool retValue = true;
	LIST *q1 = root;
	LIST *q2;

	while(q1!=NULL)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
		if(q2->node == t)
		{
			if(q1){
				tout = q1->node;
			}else{
				q2 = (LIST *)root->_post;
				if(q2)
					tout = q2->node;
				else
					retValue = false;
			}
		}
	}
	return retValue;
}

template <class T> 	void CGQueue<T>::entry(void)
{
	flag = false;
}

template <class T> 	void CGQueue<T>::leave(void)
{
	flag = true;
}

template <class T> 	void CGQueue<T>::entry2(void)
{
	temp1 = curr;
	temp2 = last;
}

template <class T> 	void CGQueue<T>::leave2(void)
{
	curr = temp1;
	last = temp2;
}

template <class T> 	bool CGQueue<T>::erase(T t)
{
	LIST *q1 = root;
	LIST *q2;

	if(curr!= NULL && t == curr->node)
	{
		q2 = curr;
		curr = (LIST *)(q2->_post);
		((LIST *)(q2->_pre))->_post = curr;
		if(curr)
			curr->_pre = q2->_pre;
		delete q2;
		return true;
	}

	while(q1!=NULL)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
		if(q2->node == t)
		{
			((LIST *)(q2->_pre))->_post = q1;
			if(q1)
				q1->_pre = q2->_pre;
			if(q2==last)
				last = (LIST*)q2->_pre;
			delete q2;
			return true;
		}
	}
	return false;
}
template <class T> 	bool CGQueue<T>::fetch(T &t, short index)
{
	LIST *q1 = root;
	LIST *q2;

	int i = 0;
	while(q1!=NULL)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
		i ++;
		if(i == index){
			if(q1){
				t = q1->node;
				return true;
			}
		}
	}
	return false;
}

template <class T> bool CGQueue<T>::insert_after(T t, T t1)
{
	LIST *q1 = root;
	LIST *q2;

	while(q1!=NULL)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
		if(q2->node == t)
		{
			LIST *q3 = new LIST;
			q3->_pre = (void*)q2;
			q3->_post = (void*)q1;
			q3->node = t1;
			q2->_post = (void*)q3;
			if(q1){
				q1->_pre = (void*)q3;
			}
			return true;
		}
	}
	return false;
}
template <class T> bool CGQueue<T>::insert_before(T t, T t1)
{
	LIST *q1 = root;
	LIST *q2;

	while(1)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
		if(q1 == NULL)
			break;
		if(q1->node == t)
		{
			LIST *q3 = new LIST;
			q3->_pre = (void*)q2;
			q3->_post = (void*)q1;
			q3->node = t1;
			q2->_post = (void*)q3;
			q1->_pre = (void*)q3;
			return true;
		}
	}
	return false;
}

//insert before current
template <class T> bool CGQueue<T>::insert_current(T t)
{
	LIST *q1, *q2;

	q2 = new LIST;
	q2->node = t;
	if(curr!= NULL)
	{
		q1 = (LIST *)(curr->_pre);
		q2->_pre = (void*)q1;
		q2->_post = (void*)curr;
		q1->_post = (void*)q2;
		curr->_pre = (void*)q2;
	}else{
		q2->_pre = (void*)last;
		last->_post = (void*)q2;
		q2->_post = (void*)curr;
	}
	curr = q2;
	return true;
}

//from-next, from-prior
template <class T> bool CGQueue<T>::from(T t)
{
	LIST *q1 = root;
	LIST *q2;

	while(q1!=NULL)
	{
		q2 = q1;
		q1 = (LIST *)q2->_post;
		if(q2->node == t)
		{
			curr = (LIST*)q2;
			last = (LIST*)q2->_pre;
			return true;
		}
	}
	return false;
}

//from-next, from-prior
template <class T> bool CGQueue<T>::prior(T &t)
{
	if(curr==root || last==root)
		return false;
	curr = last;
	last = (LIST *)last->_pre;
	t = curr->node;
	return true;
}