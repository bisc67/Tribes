#ifndef __SIMPLEPAGE_HPP
#define __SIMPLEPAGE_HPP
#include "x_types.hpp"


#define PAGE_SIZE (2*1024)
#define SPU_MEM_BASE	(32*1024)
#define SPU_MEM_END		(2*1024*1024)
#define SPU_MEM_SIZE	(SPU_MEM_END-SPU_MEM_BASE)
#define INVALID_PAGE	(-1)

enum auxmem_type
{
	TYPE_EE,
	TYPE_IOP,
};

typedef struct aux_page
{
	auxmem_type	SourceType;
	void*		Address;
	s32			SpuPage;
};

typedef struct spu_page
{
	spu_page*	pPrev;			// Ptr to prev in LRU list
	spu_page*	pNext;			// Ptr to next in LRU list
	s32			Address;		// Address in SPU memory
	s32			BackPage;		// Index to page containing original data
	s32			Locked;			// Number of times locked (should only be once)
};

class simple_page
{
public:
	void		Init(void* IopMemBase, void* EEMemBase, const s32 IopMemSize, const s32 EEMemSize);
	void		Kill(void);

private:
	void		Link(spu_page* pPage);
	void		Unlink(spu_page* pPage);
	spu_page*	Acquire(aux_page* pAuxPage);
	void		Release(spu_page* pPage);

	s32			m_PageCount;
	aux_page*	m_pPageTable;
	spu_page*	m_pLRUHead;
	spu_page	m_LRU[SPU_MEM_SIZE/PAGE_SIZE];
};

class spu_voice
{

};


#endif