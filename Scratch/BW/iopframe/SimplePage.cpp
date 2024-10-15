#include "x_files.hpp"
#include "x_threads.hpp"
#include "simplepage.hpp"

//-----------------------------------------------------------------------------
void simple_page::Init(void* IopMemBase, void* EEMemBase, const s32 IopMemSize, const s32 EEMemSize)
{
	s32 TotalLength = IopMemSize+EEMemSize;
	s32 PageCount;
	s32 i,index;

	PageCount = TotalLength / PAGE_SIZE;

	m_pPageTable = (aux_page*)x_malloc(PageCount * sizeof(aux_page));

	index=0;

	for (i=0;i<EEMemSize;i+=PAGE_SIZE)
	{
		m_pPageTable[index].SourceType = TYPE_EE;
		m_pPageTable[index].Address	   = (void*)((s32)EEMemBase+i);
		m_pPageTable[index].SpuPage	   = INVALID_PAGE;
		index++;
	}

	for (i=0;i<IopMemSize;i+=PAGE_SIZE)
	{
		m_pPageTable[index].SourceType = TYPE_IOP;
		m_pPageTable[index].Address	   = (void*)((s32)IopMemBase+i);
		m_pPageTable[index].SpuPage	   = INVALID_PAGE;
		index++;
	}
	m_PageCount=index;

	index=0;

	// Create initial LRU list
	for (i=SPU_MEM_BASE;i<SPU_MEM_END;i+=PAGE_SIZE)
	{
		m_LRU[index].pNext		= &m_LRU[index+1];
		m_LRU[index].pPrev		= &m_LRU[index-1];
		m_LRU[index].Address	= i;
		m_LRU[index].BackPage	= INVALID_PAGE;
		m_LRU[index].LockCount  = 0;
		index++;
	}
	m_pLRUHead = m_LRU;
	m_pLRUHead->pPrev = &m_LRU[index-1];
	m_pLRUHead->pPrev->pNext = m_pLRUHead;
}

//-----------------------------------------------------------------------------
// Unlink this page from the LRU list regardless of where it is. If it's at the
// head of the LRU list, advance the head of the LRU list
void simple_page::Unlink(spu_page* pPage)
{
	pPage->pPrev->pNext = pPage->pNext;
	pPage->pNext->pPrev = pPage->pPrev;

	if (pPage == m_pLRUHead)
	{
		m_pLRUHead = pPage->pNext;
	}

	pPage->pNext = NULL;
	pPage->pPrev = NULL;
}

//-----------------------------------------------------------------------------
// Link this page to the head of the lru list
void simple_page::Link(spu_page* pPage)
{
	pPage->pNext = m_pLRUHead->pNext;
	pPage->pPrev = m_pLRUHead->pPrev;

	m_pLRUHead->pNext = pPage;
	m_pLRUHead->pPrev = pPage;
	m_pLRUHead = pPage;
}

//-----------------------------------------------------------------------------
spu_page* simple_page::Acquire(aux_page* pAuxPage)
{
	spu_page* pPage;
	x_BeginAtomic();
	
	if (pAuxPage->SpuPage != INVALID_PAGE)
	{
		pPage = m_LRU[pAuxPage->SpuPage];
		pPage->LockCount++;
		// Move from where ever it is in the lru
		// to the head of the lru
		Unlink(pPage);
		return pPage;
	}

	pPage = m_pLRUHead->pPrev;
	ASSERT(!pPage->Locked);

	// Unlink it from the list
	Unlink(pPage);

	pPage->Locked = TRUE;
	x_EndAtomic();
	return pPage;
}

//-----------------------------------------------------------------------------
void	simple_page::Release(spu_page* pPage)
{
	x_BeginAtomic();

	pPage->Locked = FALSE;
	Link(pPage);
	x_EndAtomic();
}