/*
 * =====================================================================================
 *
 *       Filename:  remoteps_procinfo.hpp
 *
 *    Description:  Definition of Process Information class.
 *                  extract information of running procs on machine.
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#ifndef 		__REMOTEPS_PROCINFO_HPP_
#define 		__REMOTEPS_PROCINFO_HPP_	1

#include <vector>

#include "remoteps.pb.h"

namespace remoteps {
	class ProcInfo
	{
		public:
			void ReadProcs();
			std::vector<Process> procs;
	};
}

#endif 		/*  __REMOTEPS_PROCINFO_HPP_	*/
