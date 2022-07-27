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
#include <sstream>
#include <cstdlib>
#include <proc/readproc.h>

#include "remoteps_procinfo.hpp"

using remoteps::ProcInfo;
using remoteps::Process;

void ProcInfo::ReadProcs()
{
	procs.clear();

	int flags = PROC_FILLMEM | PROC_FILLSTAT |
		PROC_FILLSTATUS | PROC_FILLCOM |
		PROC_FILLARG | PROC_EDITCMDLCVT |
		PROC_FILLENV | PROC_EDITENVRCVT;

	PROCTAB* proc = openproc(flags);

	proc_t proc_info;
	memset(&proc_info, 0, sizeof(proc_t));

	Process p;
	while(readproc(proc, &proc_info) != NULL) {
		p.Clear();
		p.set_cmd(proc_info.cmd);
		p.set_pid(proc_info.tid);
		p.set_ppid(proc_info.ppid);
		ExtractCmdline(p, std::string(*proc_info.cmdline));
		ExtractEnviron(p, std::string(*proc_info.environ));
		procs.push_back(p);
	}
}

const std::vector<Process>& ProcInfo::GetProcs() const
{
	return procs;
}

void ProcInfo::ExtractCmdline(Process& proc, const std::string& cmdline)
{
	std::stringstream ss(cmdline);
	std::string tmp;

	proc.clear_cmdline();
	while (ss >> tmp)
		proc.add_cmdline(tmp);
}

void ProcInfo::ExtractEnviron(Process& proc, const std::string& environ)
{
	std::stringstream ss(environ);
	std::string tmp;

	proc.clear_environ();
	while(ss >> tmp)
		proc.add_environ(tmp);
}
