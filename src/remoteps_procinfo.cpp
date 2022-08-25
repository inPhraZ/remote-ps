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

void ProcInfo::readProcs()
{
	procs.clear();

	int flags = PROC_FILLMEM | PROC_FILLSTAT |
		PROC_FILLSTATUS | PROC_FILLCOM |
		PROC_FILLARG | PROC_EDITCMDLCVT |
		PROC_FILLENV | PROC_EDITENVRCVT |
		PROC_FILLUSR;

	PROCTAB* proc = openproc(flags);

	proc_t proc_info;
	memset(&proc_info, 0, sizeof(proc_t));

	Process p;
	while(readproc(proc, &proc_info) != NULL) {
		p.Clear();
		p.set_cmd(proc_info.cmd);
		p.set_pid(proc_info.tid);
		p.set_ppid(proc_info.ppid);
		p.set_user(proc_info.euser);
		p.set_priority(proc_info.priority);
		extractCmdline(p, std::string(*proc_info.cmdline));
		extractEnviron(p, std::string(*proc_info.environ));
		procs.push_back(p);
	}
}

std::vector<Process> ProcInfo::readByCmd(const std::string& cmd)
{
	readProcs();

	std::vector<Process> result;
	for (const auto p : procs) {
		if (p.cmd() == cmd)
			result.push_back(p);
	}
	return result;
}

Process ProcInfo::readByPid(const uint32_t pid)
{
	readProcs();
	Process tmp;
	tmp.Clear();
	for (const auto p : procs)
		if (p.pid() == pid)
			tmp = p;
	return tmp;
}

const std::vector<Process>& ProcInfo::getProcs() const
{
	return procs;
}

void ProcInfo::extractCmdline(Process& proc, const std::string& cmdline)
{
	std::stringstream ss(cmdline);
	std::string tmp;

	proc.clear_cmdline();
	while (ss >> tmp)
		proc.add_cmdline(tmp);
}

void ProcInfo::extractEnviron(Process& proc, const std::string& environ)
{
	std::stringstream ss(environ);
	std::string tmp;

	proc.clear_environ();
	while(ss >> tmp)
		proc.add_environ(tmp);
}
