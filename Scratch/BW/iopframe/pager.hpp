#ifndef PAGER_HPP
#define PAGER_HPP

#include "x_types.hpp"
#include "x_threads.hpp"
#include "virtualmap.hpp"

#define VM_MAX_REQUESTS		8

class virtual_map;
class page_entry;

void s_PagerStart(void);

class vm_pager
{
public:
		void			Init					(virtual_map* pMapper);
		void			Kill					(void);
		xbool			StartPage				(page_entry* pPage,const virtual_type TargetType);

private:
		void			PeriodicUpdate			(void);
		struct vm_map_request
		{
			page_entry*	pSource;
			page_entry*	pDest;
		};

		vm_map_request	m_RequestPool[VM_MAX_REQUESTS];
		s32				m_RequestReadIndex;
		s32				m_RequestWriteIndex;
		virtual_map*	m_pMapper;

		vm_map_request*	AcquireRequest			(void);
		xthread*		m_pPagerThread;

		friend			void s_PagerStart(void);
};




#endif