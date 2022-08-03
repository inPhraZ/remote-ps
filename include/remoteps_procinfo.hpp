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
			void readProcs();
			std::vector<Process> readByCmd(const std::string& cmd);
			const std::vector<Process>& getProcs() const;

		protected:
			void extractCmdline(Process& proc, const std::string& cmdline);
			void extractEnviron(Process& proc, const std::string& environ);
			std::vector<Process> procs;
	};
}

#endif 		/*  __REMOTEPS_PROCINFO_HPP_	*/
