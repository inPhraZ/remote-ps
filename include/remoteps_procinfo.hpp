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
			const std::vector<Process>& GetProcs() const;

		protected:
			void ExtractCmdline(Process& proc, const std::string& cmdline);
			void ExtractEnviron(Process& proc, const std::string& environ);
			std::vector<Process> procs;
	};
}

#endif 		/*  __REMOTEPS_PROCINFO_HPP_	*/