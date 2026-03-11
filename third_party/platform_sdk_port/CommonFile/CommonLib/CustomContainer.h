
#ifndef _CUSTOM_COMTAINER_H_
#define _CUSTOM_COMTAINER_H_

template <class _ChildItemType>
class CChildItemContainer
{
private:
	struct _tag_node			//双向链表节点结构体
	{
		_tag_node *_Next;
		_tag_node *_Prev;
		_ChildItemType _MyVal;
	};

public:	
	class CMyIterator
	{
	public:
		CMyIterator(void) : m_Nodeptr(0) {}
		CMyIterator(_tag_node *Nodeptr) : m_Nodeptr(Nodeptr) {}
		CMyIterator(const CMyIterator &Iter)
		{
			m_Nodeptr = Iter.m_Nodeptr;
		}///拷贝构造函数

		~CMyIterator(void) {}
		const CMyIterator &operator=(const CMyIterator &Iter)
		{
			if (this != &Iter)
			{
				m_Nodeptr = Iter->m_Nodeptr;
			}

			return *this;
		}

		CMyIterator& operator ++()////前置++	（推荐接口）
		{
			m_Nodeptr = m_Nodeptr->_Next;
			return (*this);
		}

		CMyIterator operator ++(int)////后置++	
		{
			CMyIterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_ChildItemType* operator ->(void)
		{
			return &(m_Nodeptr->_MyVal);
		}

		_ChildItemType& operator *(void)
		{
			return m_Nodeptr->_MyVal;
		}

		bool operator==(const CMyIterator& _Right) const
		{
			return m_Nodeptr == _Right.m_Nodeptr;
		}

		bool operator!=(const CMyIterator& _Right) const
		{
			return (!(*this == _Right));
		}

	private:
		_tag_node *m_Nodeptr;
	}; 	

public:
	CChildItemContainer(void)
	{
		_Myhead = new _tag_node;
		_Myhead->_Next = _Myhead;
		_Myhead->_Prev = _Myhead;

		m_nItemCount = 0;
	}

	CChildItemContainer(const CChildItemContainer &);	///拷贝构造函数
	
	~CChildItemContainer(void)
	{
		_tag_node *pCurNode = _Myhead->_Next;
		_tag_node *pTmpNode = NULL;
		while (pCurNode != _Myhead)
		{
			pTmpNode = pCurNode;
			pCurNode = pCurNode->_Next;

			delete pTmpNode;
		}

		delete _Myhead;
	}

	const CChildItemContainer &operator=(const CChildItemContainer &);	//这个没有必要使用,不实现

	CMyIterator begin(void)
	{
		return CMyIterator(_Myhead->_Next);
	}

	CMyIterator end(void)
	{
		return CMyIterator(_Myhead);
	}

	int size(void) {return m_nItemCount;}

	void push_back(const _ChildItemType &ChildVal)			//外面不应使用此函数
	{
		_tag_node *pValNode = new _tag_node;
		pValNode->_MyVal = ChildVal;

		//新节点插入双向链表尾部
		pValNode->_Next = _Myhead;
		pValNode->_Prev = _Myhead->_Prev;
		_Myhead->_Prev->_Next = pValNode;
		_Myhead->_Prev = pValNode;

		++m_nItemCount;
	}

private:
	int m_nItemCount;
	_tag_node *_Myhead;
};

template <class _ChildItemFirstType, class _ChildItemSecondType>
class CChildPairContainer
{
public:
	struct MY_PAIR 
	{
		_ChildItemFirstType  first;
		_ChildItemSecondType second;
	};

private:
	struct _tag_node			//双向链表节点结构体
	{
		_tag_node *_Next;
		_tag_node *_Prev;
		MY_PAIR _MyVal;
	};

public:
	class CMyIterator
	{
	public:
		CMyIterator(void) : m_Nodeptr(0) {}
		CMyIterator(_tag_node *Nodeptr) : m_Nodeptr(Nodeptr) {}
		CMyIterator(const CMyIterator &Iter)
		{
			m_Nodeptr = Iter.m_Nodeptr;
		}///拷贝构造函数

		~CMyIterator(void) {}
		const CMyIterator &operator=(const CMyIterator &Iter)
		{
			if (this != &Iter)
			{
				m_Nodeptr = Iter->m_Nodeptr;
			}

			return *this;
		}

		CMyIterator& operator ++()////前置++	（推荐接口）
		{
			m_Nodeptr = m_Nodeptr->_Next;
			return (*this);
		}

		CMyIterator operator ++(int)////后置++	
		{
			CMyIterator _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		MY_PAIR *operator ->(void)
		{
			return &(m_Nodeptr->_MyVal);
		}

		MY_PAIR &operator *(void)
		{
			return m_Nodeptr->_MyVal;
		}

		bool operator==(const CMyIterator& _Right) const
		{
			return m_Nodeptr == _Right.m_Nodeptr;
		}

		bool operator!=(const CMyIterator& _Right) const
		{
			return (!(*this == _Right));
		}

	private:
		_tag_node *m_Nodeptr;
	}; 

public:
	CChildPairContainer(void)
	{
		_Myhead = new _tag_node;
		_Myhead->_Next = _Myhead;
		_Myhead->_Prev = _Myhead;

		m_nItemCount = 0;
	}

	CChildPairContainer(const CChildPairContainer &);///拷贝构造函数
	~CChildPairContainer(void)
	{
		_tag_node *pCurNode = _Myhead->_Next;
		_tag_node *pTmpNode = NULL;
		while (pCurNode != _Myhead)
		{
			pTmpNode = pCurNode;
			pCurNode = pCurNode->_Next;

			delete pTmpNode;
		}

		delete _Myhead;
	}

	CChildPairContainer &operator=(const CChildPairContainer &);

	CMyIterator begin(void)
	{
		return CMyIterator(_Myhead->_Next);
	}

	CMyIterator end(void)
	{
		return CMyIterator(_Myhead);
	}

	int size(void) {return m_nItemCount;}

	void push_back(const _ChildItemFirstType &first, const _ChildItemSecondType &second)			//外面不应使用此函数
	{
		_tag_node *pValNode = new _tag_node;
		pValNode->_MyVal.first = first;
		pValNode->_MyVal.second = second;

		//新节点插入双向链表尾部
		pValNode->_Next = _Myhead;
		pValNode->_Prev = _Myhead->_Prev;
		_Myhead->_Prev->_Next = pValNode;
		_Myhead->_Prev = pValNode;

		++m_nItemCount;
	}

protected: 
	int m_nItemCount;
	_tag_node *_Myhead;
};	

#endif //_CUSTOM_COMTAINER_H_


