#ifndef __VIRTUALMAP_HPP
#define __VIRTUALMAP_HPP
#include "x_types.hpp"

enum virtual_type
{
	MEMTYPE_LOCAL,
	MEMTYPE_REMOTE,
	MEMTYPE_AUDIO,
	// Has to be last
	MEMTYPE_MAX,
};

// The Prev and Next links get set to INVALID_PAGE when it is
// removed from the LRU list
#define VM_INVALID_PAGE (0xffff)
// LockCount is set to VM_UNAVAILABLE_PAGE when the page is
// about to be written to.
#define VM_UNAVAILABLE_PAGE		(0xfffe)
#define VM_DISCARDABLE_PAGES	(256)

// We're using u16 entries for the lru indexes instead of pointers to save some space.
// If we are mapping 8MB, this will require 12 bytes per page (each page 2KB) which
// totals 48K just for the page table.
struct page_entry
{
	virtual_type Type;
	u8		Flags;
	u8		OwningQueue;
	u16		LockCount;
	u16		LRUPrev;
	u16		LRUNext;
	u16		TwinIndex;
	void*	Address;
};

enum vm_queue
{
	VM_QUEUE_DIRTY,					// Pending swap
	VM_QUEUE_AVAIL,					// Available for use
	VM_QUEUE_LOCKED,				// Locked for now, may be changed to dirty
};

#include "pager.hpp"

class virtual_map
{
public:
		//*********************************************************************
		// IMPORTANT WARNING::
		// You MUST be within a x_BeginAtomic and x_EndAtomic block to make sure
		// that no other threads attempt to manipulate the list while these
		// operations are taking place. If you know for certain that all these
		// activities will be taking place in one thread context and NEVER within
		// another, then you will not need to be within an atomic section. It'll
		// be a lot faster this way.
		//*********************************************************************
		void			Init					(const s32 Length, const s32 PageSize);
		void			Kill					(void);
		// Adds a block of memory to the vm mapping tables. The VM mapper will decide where
		// it goes in virtual space.
		void			Donate					(const void* pBase, const s32 Length, const virtual_type MemType);
		// Swap the virtual mapping between two pages. 
		void			SwapMapping				(const void* pSrc, const void* pDst);
		// Locks a page. This means there it is actually being referenced
		// Returns TRUE if it was able to be locked, FALSE if it was busy
		xbool			Lock					(const void* pVirtual, const virtual_type Type);
		// Unlocks a page. The page must have been previously locked by the same
		// resource holder.
		xbool			Unlock					(const void* pVirtual, const s32 Length);
		s32				LengthToPages			(const s32 Length) { return Length / m_PageSize;};
		void			SetPageSize				(const s32 PageSize) { m_PageSize = PageSize;};
		s32				GetPageSize				( void ) { return m_PageSize;};
		s32				GetMemSize				( void ) { return m_PageCount * m_PageSize; };
		// Returns the physical page associated with a virtual address. This will return NULL
		// if the page is currently locked for a transfer.
		page_entry*		VirtualToPhysical		(const void* pVirtual);

		//---------------------------------------------------------------------
		// The least recently used list is the mechanism used to make sure that a page isn't 
		// removed from a region too soon after it has been placed in there.

		// LRU_Remove will give a page entry that is at the tail of the list for the page type. It
		// will unlink this page from the LRU list. The page returned will have no references. Any
		// currently referenced pages will be "touched" to move them to the head of the LRU.
		page_entry*		LRU_Remove				(const virtual_type PageType,const vm_queue Queue=VM_QUEUE_AVAIL);

		// LRU_Insert will add the given page to the head of the LRU list for that page type. This 
		// page MUST NOT be part of the LRU list at this time. The page MUST be in the same LRU list
		// as it was originally. The page must NOT be locked at this time (only LRU_Touch is allowed
		// to insert pages if they're not locked since Inserting and Removing makes no difference to
		// the contents of the data).
		void			LRU_Insert				(page_entry* pPage,vm_queue Queue);

		// LRU_Touch will take any arbitrary page and move it to the head of the LRU list. This is
		// the mechanism that keeps the pages floating around the "cache" areas as long as possible.
		// This page can be moved even if it is locked.
		void			LRU_Touch				(page_entry* pPage);

//*****************************************************************************
private:
		vm_queue 		LRU_Remove				(page_entry* pPage);

		// When we need to allocate a page, we try to steal from the m_AvailHead list. If there are
		// no pages present on that list, we can then steal from the dirty head. In the process of
		// doing this tho, we need to ensure that the page associated with this one (a copy of it)
		// is moved from the avail list back to the locked list so it is not discarded when needed.
		//
		// At the moment, we should always have a combined VM_DISCARDABLE_PAGES within all memory
		// block types (this will be IOP and SPU memory only) free and available for use at any
		// time. When a page is removed from the Avail list, another page will need to be retired
		// from the "Locked" list to the "Dirty" list. The "Dirty" list will get consumed in two
		// situations:
		// 1. A page out occurs. When the paging thread is idle for around 10ms, it will swap a
		//    page out from the dirty list. This page, once the transfer is complete, will be moved
		//    from the Dirty list to discardable on the source side of the page and it it's mirror
		//    will be placed in the discardable list on the target side of the page.
		// 2. The Avail pool is totally empty. If this is the case, a transfer page out MUST happen
		//    as soon as possible to make a page available on the next pass.
		//
		// It is important that the code attempting to acquire a block does NOT assume it will
		// automatically get one. It must be able to cope with only getting, say, 6 out of 8 blocks
		// it needs to continue.
		//
		u16				m_LockedHead[MEMTYPE_MAX];	// Pages that cannot be swapped out
		u16				m_DirtyHead[MEMTYPE_MAX];	// Pages that are pending a page out
		u16				m_AvailHead[MEMTYPE_MAX];	// Pages that are available for immediate use
		s32				m_PageSize;
		s32				m_PageCount;
		s32*			m_pVirtualToPhysical;	// Virtual page number to audmem_entry index table
		s32				m_DonateIndex;
		vm_pager		m_Pager;
		page_entry*		m_pPageTable;			// List of audmem_entry for each memory block

		friend			vm_pager;

};


#endif