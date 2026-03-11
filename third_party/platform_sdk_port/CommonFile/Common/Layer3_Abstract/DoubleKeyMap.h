#ifndef DOUBLE_KEY_MAP_HEAD
#define DOUBLE_KEY_MAP_HEAD

#include <map>

template<class KEY1, class KEY2, class VALUE>
class CDoubleKeyMap
{
public:
	CDoubleKeyMap(void) {}
	~CDoubleKeyMap(void) {}

	void insert(const KEY1 &Key1, const KEY2 &Key2, VALUE *const pValue)
	{
		m_Key1ValueMap.insert(std::pair<KEY1, VALUE *>(Key1, pValue));
		m_Key2ValueMap.insert(std::pair<KEY2, VALUE *>(Key2, pValue));
	}

 	bool empty(void)
 	{
 		return m_Key1ValueMap.empty();								//任何一个Map都一样
 	}

	void clear(void)
	{
		m_Key1ValueMap.clear();
		m_Key2ValueMap.clear();
	}

	typename std::map<KEY1, VALUE *>::iterator find(const KEY1 &Key)
 	{
 		return m_Key1ValueMap.find(Key);
 	}

	typename std::map<KEY2, VALUE *>::iterator find(const KEY2 &Key)
	{
		return m_Key2ValueMap.find(Key);
	}
 
 	void erase(typename std::map<KEY1, VALUE *>::iterator Iter, const KEY2 &Key)
 	{
 		m_Key1ValueMap.erase(Iter);
 		m_Key2ValueMap.erase(Key);
 	}

	void erase(typename std::map<KEY2, VALUE *>::iterator Iter, const KEY1 &Key)
	{
		m_Key1ValueMap.erase(Key);
		m_Key2ValueMap.erase(Iter);
	}

	typename std::map<KEY1, VALUE *>::iterator begin(void)
	{
		return m_Key1ValueMap.begin();
	}

	typename std::map<KEY2, VALUE *>::iterator begin(const KEY2 &)
	{
		return m_Key2ValueMap.begin();
	}

 	typename std::map<KEY1, VALUE *>::iterator end(const KEY1 &)
 	{
 		return m_Key1ValueMap.end();
 	}

 	typename std::map<KEY2, VALUE *>::iterator end(const KEY2 &)
  	{
  		return m_Key2ValueMap.end();
  	}

private:
	std::map<KEY1, VALUE *> m_Key1ValueMap;
	std::map<KEY2, VALUE *> m_Key2ValueMap;
};

#endif //DOUBLE_KEY_MAP_HEAD