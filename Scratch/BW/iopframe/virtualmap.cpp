#include "x_files.hpp"
#include "x_threads.hpp"
#include "virtualmap.hpp"

//*****************************************************************************
void virtual_map::Init(s32 Length,s32 PageSize)
{
	m_PageSize			 = PageSize;
	m_PageCount			 = LengthToPages(Length);
	m_pPageTable		 = (page_entry*)x_malloc(sizeof(page_entry)*m_PageCount);
	m_pVirtualToPhysical = (s32*)x_malloc(sizeof(s32)*m_PageCount);
	m_DonateIndex		 = 0;

	x_memset(m_pPageTable,0,sizeof(page_entry)*m_PageCount);
	x_memset(m_pVirtualToPhysical,0,sizeof(s32)*m_PageCount);
	m_LockedHead[MEMTYPE_LOCAL]=VM_INVALID_PAGE;
	m_AvailHead [MEMTYPE_LOCAL]=VM_INVALID_PAGE;
	m_DirtyHead [MEMTYPE_LOCAL]=VM_INVALID_PAGE;

	m_LockedHead[MEMTYPE_REMOTE]=VM_INVALID_PAGE;
	m_AvailHead [MEMTYPE_REMOTE]=VM_INVALID_PAGE;
	m_DirtyHead [MEMTYPE_REMOTE]=VM_INVALID_PAGE;

	m_LockedHead[MEMTYPE_AUDIO]=VM_INVALID_PAGE;
	m_AvailHead [MEMTYPE_AUDIO]=VM_INVALID_PAGE;
	m_DirtyHead [MEMTYPE_AUDIO]=VM_INVALID_PAGE;

	m_Pager.Init(this);
}


//*****************************************************************************
void virtual_map::Donate(const void* pBase, const s32 Length, const virtual_type MemType)
{
	s32 i;
	s32 PageCount = LengthToPages(Length);

	x_BeginAtomic();
	for (i=0;i<PageCount;i++)
	{
		m_pPageTable[m_DonateIndex].Type		= MemType;
		m_pPageTable[m_DonateIndex].Flags		= 0;
		m_pPageTable[m_DonateIndex].LockCount	= 0;
		m_pPageTable[m_DonateIndex].Address		= (u8*)pBase+(i*m_PageSize);
		m_pPageTable[m_DonateIndex].LRUPrev		= VM_INVALID_PAGE;
		m_pPageTable[m_DonateIndex].LRUNext		= VM_INVALID_PAGE;
		m_pPageTable[m_DonateIndex].TwinIndex	= VM_INVALID_PAGE;

		m_pVirtualToPhysical[m_DonateIndex]		= m_DonateIndex;
		LRU_Insert(&m_pPageTable[m_DonateIndex],VM_QUEUE_AVAIL);
		m_DonateIndex++;
	}
	x_EndAtomic();

	static char* types[]=
	{
		"MEMTYPE_LOCAL",
		"MEMTYPE_REMOTE",
		"MEMTYPE_AUDIO"
	};

	x_DebugMsg("virtual_map::Donate() Address: 0x%08x, Length: %d pages, Type: %s\n",pBase, PageCount, types[MemType]);
}

//*****************************************************************************
page_entry* virtual_map::VirtualToPhysical(const void* pVirtual)
{
	s32 page;
	page_entry* pEntry;

	page = (s32)pVirtual / m_PageSize;

	if ( (page < 0) || (page >= m_PageCount) )
		return NULL;
	pEntry = &m_pPageTable[m_pVirtualToPhysical[page]];
	if (pEntry->LockCount == VM_UNAVAILABLE_PAGE)
		return NULL;
	return pEntry;
}

//*****************************************************************************
void virtual_map::SwapMapping(const void* pSrc,const void* pDst)
{
	s32 srcpage,dstpage,temp;

	x_BeginAtomic();
	srcpage = (s32)pSrc / m_PageSize;
	dstpage = (s32)pDst / m_PageSize;

	temp = m_pVirtualToPhysical[srcpage];
	m_pVirtualToPhysical[srcpage]=m_pVirtualToPhysical[dstpage];
	m_pVirtualToPhysical[dstpage]=temp;
	x_EndAtomic();
}

//*****************************************************************************
void virtual_map::LRU_Insert(page_entry* pPage,vm_queue QueueType)
{
	page_entry* pHead;
	page_entry* pTail;
	u16* pQueue;

	// Must be in an atomic section
	ASSERT(x_IsAtomic());
	pQueue=NULL;
	switch (QueueType)
	{
	case VM_QUEUE_DIRTY:
		pQueue = m_DirtyHead;
		break;
	case VM_QUEUE_AVAIL:
		pQueue = m_AvailHead;
		break;
	case VM_QUEUE_LOCKED:
		pQueue = m_LockedHead;
		break;
	default:
		ASSERT(FALSE);
	}

	u16 Index = ((s32)pPage - (s32)m_pPageTable)/sizeof(page_entry);
	ASSERT(pPage->LRUNext == VM_INVALID_PAGE);
	ASSERT(pPage->LRUPrev == VM_INVALID_PAGE);

	ASSERT(&m_pPageTable[Index] == pPage);
	if (pPage->LockCount == VM_UNAVAILABLE_PAGE)
	{
		pPage->LockCount = 0;
	}
	// Link this page to the head of the LRU list
	// for this page type. If that list was empty, then
	// we assign the start of the list to this page and
	// make the page point back to itself
	if (pQueue[pPage->Type]==VM_INVALID_PAGE)
	{
		pPage->LRUNext = Index;
		pPage->LRUPrev = Index;
	}
	else
	{
		pHead = &m_pPageTable[pQueue[pPage->Type]];
		pTail = &m_pPageTable[pHead->LRUPrev];
		pPage->LRUNext = pTail->LRUNext;
		pPage->LRUPrev = pHead->LRUPrev;
		pTail->LRUNext = Index;
		pHead->LRUPrev = Index;
	}
	pQueue[pPage->Type] = Index;
}

//*****************************************************************************
// This is a private version of LRU_Remove. It does all the hard work!
vm_queue virtual_map::LRU_Remove(page_entry* pPage)
{
	page_entry* pPrev;
	page_entry* pNext;
	vm_queue	QueueType;
	u16 Index = ((s32)pPage - (s32)m_pPageTable)/sizeof(page_entry);

	ASSERT(x_IsAtomic());

	ASSERT(pPage->LRUPrev != VM_INVALID_PAGE);
	ASSERT(pPage->LRUNext != VM_INVALID_PAGE);

	QueueType = (vm_queue)pPage->OwningQueue;
	// If we're pointing back to ourselves, then the list will be
	// empty once this element is removed so mark it as so.
	if (pPage->LRUNext == Index)
	{
		ASSERT(pPage->LRUPrev == Index);
		switch (QueueType)
		{
		case VM_QUEUE_DIRTY:
			m_DirtyHead[pPage->Type] = VM_INVALID_PAGE;
			break;
		case VM_QUEUE_AVAIL:
			m_AvailHead[pPage->Type] = VM_INVALID_PAGE;
			break;
		case VM_QUEUE_LOCKED:
			ASSERT(FALSE);					// The locked queue should never be empty!
			m_LockedHead[pPage->Type] = VM_INVALID_PAGE;
			break;
		default:
			ASSERT(FALSE);
		}
	}
	else
	{
		ASSERT(m_pPageTable[pPage->LRUPrev].LRUNext == Index);
		ASSERT(m_pPageTable[pPage->LRUNext].LRUPrev == Index);

		pPrev = &m_pPageTable[pPage->LRUPrev];
		pNext = &m_pPageTable[pPage->LRUNext];
		pPrev->LRUNext = pPage->LRUNext;
		pNext->LRUPrev = pPage->LRUPrev;

	}
	pPage->LRUNext = VM_INVALID_PAGE;
	pPage->LRUPrev = VM_INVALID_PAGE;
	return QueueType;

}

//*****************************************************************************
// See comments in header file about use of these functions. Remove a page from
// the end of the LRU list
page_entry*	virtual_map::LRU_Remove(const virtual_type PageType, const vm_queue Queue)
{
	page_entry* pPage;
	page_entry* pFirst;
	s32 index;

	x_BeginAtomic();
	// Get page index for base of this list
	switch(Queue)
	{
	case VM_QUEUE_DIRTY:
		index = m_DirtyHead[PageType];
		break;
	case VM_QUEUE_AVAIL:
		index = m_AvailHead[PageType];
		break;
	case VM_QUEUE_LOCKED:
		index = m_LockedHead[PageType];
		break;
	default:
		ASSERT(FALSE);
	}

	// If it's not set, then there are no pages available so
	// barf!
	if (index==VM_INVALID_PAGE)
	{
		x_EndAtomic();
		return NULL;
	}

	pPage = &m_pPageTable[m_pPageTable[index].LRUPrev];
	pFirst = pPage;

	// Page should not be locked in any manner
	ASSERT(pPage->LockCount==0);

	LRU_Remove(pPage);
	pPage->LockCount = VM_UNAVAILABLE_PAGE;

	x_EndAtomic();
	return pPage;
}

//*****************************************************************************
void virtual_map::LRU_Touch(page_entry* pPage)
{
	vm_queue QueueType;
	x_BeginAtomic();
	QueueType = LRU_Remove(pPage);
	LRU_Insert(pPage,QueueType);
	x_EndAtomic();
}

//*****************************************************************************
xbool virtual_map::Lock(const void* pVirtual,const virtual_type Type)
{
	page_entry* pPage;
	page_entry* pTwin;
	// we ignore the length for now
	x_BeginAtomic();
	pPage = VirtualToPhysical(pVirtual);
	// No page mapping, just exit. If the LRUnext is set to
	// VM_PAGE_INVALID then this means that it is in the process
	// of being moved.
	if ( !pPage || (pPage->LRUNext == VM_INVALID_PAGE) )
	{
		x_EndAtomic();
		return FALSE;
	}
	// Do we have the right type on this page?
	// Yes, lock it and exit with success code
	if (pPage->Type == Type)
	{
		pPage->LockCount++;
		x_EndAtomic();
		return TRUE;
	}
	// Do we have a twin to this page?
	if (pPage->TwinIndex != VM_INVALID_PAGE)
	{
		pTwin = &m_pPageTable[pPage->TwinIndex];
		// Cannot have twins both locked
		ASSERT(pPage->LockCount==0);
		ASSERT(pTwin->LockCount==0);
		// Yes, check it's type
		if (pTwin->Type == Type)
		{
			pTwin->LockCount++;
			// Swap the twins so that the most recently locked twin is the first
			// in the lookup
			u32 idx;
			idx = pPage->TwinIndex;
			pPage->TwinIndex = pTwin->TwinIndex;
			pTwin->TwinIndex = idx;
			x_EndAtomic();
			return TRUE;
		}
	}
	// So, we never found an available mapping in the correct
	// address space so we initiate a transfer to move data from
	// the wrong address space to the correct one.
	m_Pager.StartPage(pPage,Type);

	x_EndAtomic();
	return FALSE;
}

//*****************************************************************************
xbool virtual_map::Unlock(const void* pVirtual, const s32 Length)
{
	page_entry* pPage;
	// we ignore the length for now
	x_BeginAtomic();
	pPage = VirtualToPhysical(pVirtual);
	ASSERT(Length == m_PageSize);
	ASSERT(pPage->LockCount != VM_UNAVAILABLE_PAGE);
	ASSERT(pPage->LockCount > 0);
	pPage->LockCount--;
	x_EndAtomic();
	return (pPage->LockCount == 0);
}