#include "x_files.hpp"
#include "x_threads.hpp"
#include "virtualmap.hpp"

vm_pager* s_pPager;

//-----------------------------------------------------------------------------
void s_PagerStart(void)
{
	s_pPager->PeriodicUpdate();
}


//-----------------------------------------------------------------------------
void vm_pager::Init(virtual_map* pMapper)
{
	m_pMapper = pMapper;

	m_RequestReadIndex = 0;
	m_RequestWriteIndex = 0;
	s_pPager = this;
	m_pPagerThread = new xthread(s_PagerStart,"Pager Update",8192,4);
}

//-----------------------------------------------------------------------------
void vm_pager::Kill(void)
{
	delete m_pPagerThread;
}

//-----------------------------------------------------------------------------
xbool vm_pager::StartPage(page_entry* pPage,const virtual_type TargetType)
{
	page_entry* pDest;
	s32 newindex;

	newindex = m_RequestWriteIndex+1;
	if (newindex >= VM_MAX_REQUESTS)
	{
		newindex=0;
	}

	// **** DANGER ***** NEED TO CHECK FOR POTENTIAL LOCKING ISSUES HERE
	// - Worried about a page request completing just prior to this one starting
	// and another being performed when not required.

	// If the request pool is full or the page is currently waiting on
	// a request to complete then just discard the request.
	if ( (newindex == m_RequestReadIndex) ||
		 (pPage->LRUNext == VM_UNAVAILABLE_PAGE) )
	{
		return FALSE;
	}

	// Acquire a page from the specific target type's available list. If
	// we cannot get one, then just bail for now

	pDest = m_pMapper->LRU_Remove(TargetType,VM_QUEUE_AVAIL);
	if (!pDest)
		return FALSE;

	// We know that the contents pointed to by 'pDest' are discardable
	ASSERT(pDest->Type == TargetType);
	ASSERT(pDest->LockCount == VM_INVALID_PAGE);

	m_RequestPool[m_RequestWriteIndex].pSource = pPage;
	m_RequestPool[m_RequestWriteIndex].pDest   = pDest;

	// Advance to next entry in paging list.
	m_RequestWriteIndex=newindex;

	// Returns true if a request was actually queued
	return TRUE;
}

//-----------------------------------------------------------------------------
void vm_pager::PeriodicUpdate(void)
{
	while(1)
	{
		while (m_RequestReadIndex != m_RequestWriteIndex)
		{
			m_RequestReadIndex++;
			if (m_RequestReadIndex >= VM_MAX_REQUESTS)
			{
				m_RequestReadIndex = 0;
			}
		}
		x_DelayThread(5);
	}
}