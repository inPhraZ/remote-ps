/*
 * =====================================================================================
 *
 *       Filename:  remoteps_procinfo.cpp
 *
 *    Description:  Implementation of ProcInfo class
 *
 *         Author:  Farzin Monsef 
 *
 * =====================================================================================
 */

#include <vector>
#include <cstdlib>
#include <proc/readproc.h>

#include "remoteps_procinfo.hpp"

using remoteps::ProcInfo;
using remoteps::Process;

void ProcInfo::ReadProcs()
{
	procs.clear();

	PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);

	proc_t proc_info;
	memset(&proc_info, 0, sizeof(proc_t));

	Process p;
	while(readproc(proc, &proc_info) != NULL) {
		p.set_name(proc_info.cmd);
		p.set_pid(proc_info.tid);
		p.set_ppid(proc_info.ppid);
		procs.push_back(p);
	}
}

const std::vector<Process>& ProcInfo::GetProcs() const
{
	return procs;
}
