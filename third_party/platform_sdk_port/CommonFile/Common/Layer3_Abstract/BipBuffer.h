#ifndef __BIP_BUFFER__
#define __BIP_BUFFER__

/*
  Bip-Buffer使用起来有些类似循环缓冲区，但是在结构上略有不同。
  Bip-Buffer内部采用了两个循环存储区（而不是靠维持 头指针和尾
  指针）来实现数据的高速存取，而且可以让Bip-Buffer的使用者完
  全不必担心写入数据到达缓冲区末尾，导致重新从缓冲区的首部开
  始写入的 问题。Bip-Buffer维护的存储区是连续的，因此，Bip-Buffer
  可以通过API调用非常高效地使用存储区，在整个使用过程中可以最大限
  度避免使用诸如memcpy(),memmove()之类的内存拷贝操作（通常对于循
  环缓冲区来说，频繁地调用内存操作函数会成为效率瓶颈）。
  最后，Bip- Buffer两段式的内存分配系统允许用户申请一块较大的内存，
  而通过Commit操作来确认真正需要的内存大小，然后把没有用完的内存回收。

  因为成员函数比较简单，为了效率，全为inline函数
*/

#include "PFHelper.h"
#include "ShareSDK.h"

class CBipBuffer
{
public:
	CBipBuffer() : m_pBuffer(NULL), m_buflen(0),m_ixa(0), m_sza(0), m_ixb(0), m_szb(0),  m_ixResrv(0), m_szResrv(0)
	{
	}

	~CBipBuffer()
	{
		if (m_pBuffer != NULL)
		{
			delete []m_pBuffer;
		}
	}
	//申请BipBuffer空间
	inline bool AllocateBuffer(int buffersize = 4096);
	//释放BitBuffer空间
	inline void FreeBuffer();

	//申请委派存储空间
	inline BYTE* Reserve(int size, int& reserved);
	//委派存储空间
	inline void Commit(int size);
	//获得连续数据块
	inline const BYTE* GetContiguousBlock(int& size) const ;
	inline BYTE* GetContiguousBlock(int& size);
	//解除委派存储空间
	inline void DecommitBlock(int size);

	//获得委派空间大小
	inline int GetCommittedSize() const;
	//获得申请委派空间大小
	inline int GetReservationSize() const;
	//获得BipBuffer空间大小
	inline int GetBufferSize() const;
	//判断是否已经初始化、可用
	inline bool IsInitialized() const;

	//清理重置
	inline void Clear();

private:
	int GetSpaceAfterA() const
	{
		return m_buflen - m_ixa - m_sza;
	}

	int GetBFreeSpace() const
	{
		return m_ixa - m_ixb - m_szb;
	}

private:
    BYTE* m_pBuffer;//向系统申请的空间
	int m_buflen;//向系统申请空间的长度

	volatile int m_ixa;//A存储区的开始位置
    volatile int m_sza;//A存储区的大小
    volatile int m_ixb;//B存储区的开始位置
    volatile int m_szb;//B存储区的大小
   
    volatile int m_ixResrv;//存储区的开始位置
    volatile int m_szResrv;//存储区的大小

	//CLock m_dataLock;//数据访问锁
};


// Allocate Buffer
//
// Allocates a buffer in virtual memory, to the nearest page size (rounded up)
//
// Parameters:
//   int buffersize                size of buffer to allocate, in bytes (default: 4096)
//
// Returns:
//   bool                        true if successful, false if buffer cannot be allocated
bool CBipBuffer::AllocateBuffer(int buffersize/* = 4096*/)
{
	if (buffersize <= 0) return false;

	if (m_pBuffer != NULL) FreeBuffer();

	int dwPageSize=CPFHelper::GetPageSize();

	// Calculate nearest page size
	buffersize = ((buffersize + dwPageSize - 1) / dwPageSize) * dwPageSize;
	try
	{
		m_pBuffer = new BYTE[buffersize];
		memset(m_pBuffer, 0, buffersize);
	}
	catch(...)
	{
		m_pBuffer = NULL;
        NORMAL_OUTPUT("~~~~~ new err ~~~~~~~buffersize = %d ",buffersize);
		return false;
	}

	m_buflen = buffersize;
	return true;
}

///
/// \brief Clears the buffer of any allocations.
///
/// Clears the buffer of any allocations or reservations. Note; it
/// does not wipe the buffer memory; it merely resets all pointers,
/// returning the buffer to a completely empty state ready for new
/// allocations.
void CBipBuffer::Clear()
{
	m_ixa = m_sza = m_ixb = m_szb = m_ixResrv = m_szResrv = 0;
}

// Free Buffer
//
// Frees a previously allocated buffer, resetting all internal pointers to 0.
//
// Parameters:
//   none
//
// Returns:
//   void
void CBipBuffer::FreeBuffer()
{
	if (m_pBuffer == NULL) return;

	m_ixa = m_sza = m_ixb = m_szb = m_buflen = 0;

	delete []m_pBuffer;

	m_pBuffer = NULL;
}

// Reserve
//
// Reserves space in the buffer for a memory write operation
//
// Parameters:
//   int size                amount of space to reserve
//   OUT int& reserved        size of space actually reserved
//
// Returns:
//   BYTE*                    pointer to the reserved block
//
// Notes:
//   Will return NULL for the pointer if no space can be allocated.
//   Can return any value from 1 to size in reserved.
//   Will return NULL if a previous reservation has not been committed.
BYTE* CBipBuffer::Reserve(int size, int& reserved)
{
	//m_dataLock.Lock();
	// We always allocate on B if B exists; this means we have two blocks and our buffer is filling.
	if (m_szb)
	{
		int freespace = GetBFreeSpace();

		if (size < freespace) freespace = size;

		if (freespace == 0)
		{
		//	m_dataLock.Unlock();
			return NULL;
		}

		m_szResrv = freespace;
		reserved = freespace;
		m_ixResrv = m_ixb + m_szb;
		return m_pBuffer + m_ixResrv;
	}
	else
	{
		// Block b does not exist, so we can check if the space AFTER a is bigger than the space
		// before A, and allocate the bigger one.

		int freespace = GetSpaceAfterA();
		if (freespace >= m_ixa)
		{
			if (freespace == 0) 
			{
			//	m_dataLock.Unlock();
				return NULL;
			}

			if (size < freespace) freespace = size;

			m_szResrv = freespace;
			reserved = freespace;
			m_ixResrv = m_ixa + m_sza;
			return m_pBuffer + m_ixResrv;
		}
		else
		{
			if (m_ixa == 0)
			{
			//	m_dataLock.Unlock();
				return NULL;
			}

			if (m_ixa < size) size = m_ixa;
			m_szResrv = size;
			reserved = size;
			m_ixResrv = 0;
			return m_pBuffer;
		}
	}
}

// Commit
//
// Commits space that has been written to in the buffer
//
// Parameters:
//   int size                number of bytes to commit
//
// Notes:
//   Committing a size > than the reserved size will cause an assert in a debug build;
//   in a release build, the actual reserved size will be used.
//   Committing a size < than the reserved size will commit that amount of data, and release
//   the rest of the space.
//   Committing a size of 0 will release the reservation.
//
void CBipBuffer::Commit(int size)
{
	if (size == 0)
	{
		// decommit any reservation
		m_szResrv = m_ixResrv = 0;
		return;
	}

	// If we try to commit more space than we asked for, clip to the size we asked for.

	if (size > m_szResrv)
	{
		size = m_szResrv;
	}

	// If we have no blocks being used currently, we create one in A.

	if (m_sza == 0 && m_szb == 0)
	{
		m_ixa = m_ixResrv;
		m_sza = size;

		m_ixResrv = 0;
		m_szResrv = 0;

	//	m_dataLock.Unlock();
		return;
	}

	if (m_ixResrv == m_sza + m_ixa)
	{
		m_sza += size;
	}
	else
	{
		m_szb += size;
	}

	m_ixResrv = 0;
	m_szResrv = 0;

	//m_dataLock.Unlock();
}

// GetContiguousBlock
//
// Gets a pointer to the first contiguous block in the buffer, and returns the size of that block.
//
// Parameters:
//   OUT int & size            returns the size of the first contiguous block
//
// Returns:
//   BYTE*                    pointer to the first contiguous block, or NULL if empty.

const BYTE* CBipBuffer::GetContiguousBlock(int& size) const
{
	//m_dataLock.Lock();
	if (m_sza == 0)
	{
		size = 0;

	//	m_dataLock.Unlock();
		return NULL;
	}

	size = m_sza;
	return m_pBuffer + m_ixa;

}

BYTE* CBipBuffer::GetContiguousBlock(int& size)
{
	//m_dataLock.Lock();
	if (m_sza == 0)
	{
		size = 0;

		//	m_dataLock.Unlock();
		return NULL;
	}

	size = m_sza;
	return m_pBuffer + m_ixa;

}

// DecommitBlock
//
// Decommits space from the first contiguous block
//
// Parameters:
//   int size                amount of memory to decommit
//
// Returns:
//   nothing

void CBipBuffer::DecommitBlock(int size)
{
	if (size >= m_sza)
	{
		m_ixa = m_ixb;
		m_sza = m_szb;
		m_ixb = 0;
		m_szb = 0;
	}
	else
	{
		m_sza -= size;
		m_ixa += size;
	}

//	m_dataLock.Unlock();
}

// GetCommittedSize
//
// Queries how much data (in total) has been committed in the buffer
//
// Parameters:
//   none
//
// Returns:
//   int                    total amount of committed data in the buffer

int    CBipBuffer::GetCommittedSize() const
{
	return m_sza + m_szb;
}

// GetReservationSize
//
// Queries how much space has been reserved in the buffer.
//
// Parameters:
//   none
//
// Returns:
//   int                    number of bytes that have been reserved
//
// Notes:
//   A return value of 0 indicates that no space has been reserved

int CBipBuffer::GetReservationSize() const
{
	return m_szResrv;
}

// GetBufferSize
//
// Queries the maximum total size of the buffer
//
// Parameters:
//   none
//
// Returns:
//   int                    total size of buffer

int CBipBuffer::GetBufferSize() const
{
	return m_buflen;
}

// IsInitialized
//
// Queries whether or not the buffer has been allocated
//
// Parameters:
//   none
//
// Returns:
//   bool                    true if the buffer has been allocated

bool CBipBuffer::IsInitialized() const
{
	return m_pBuffer != NULL;
}


#endif //__BIP_BUFFER__
