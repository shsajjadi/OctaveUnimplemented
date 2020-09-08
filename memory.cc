// Copyright (C) 2020 Seyyed Hossein Sajjadi
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.

#include <octave/oct.h>
#include <windows.h>
#include <psapi.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

// compile with -DPSAPI_VERSION=1 -lpsapi

namespace memory
{
  struct OctaveMemoryInfo
  {
    size_t SystemMemoryAvailable;
    size_t VirtualAddressSpaceAvailable;
    size_t VirtualAddressSpaceTotal;
    size_t MaxPossibleArrayBytes;
    size_t MemAvailableAllArrays;
    size_t MemUsedOctave;
    size_t PhysicalMemoryAvailable;
    size_t PhysicalMemoryTotal;
    bool MaxPossibleLimited;
    bool MemAvailableLimited;
  };

  // PagefileUsage = (committed and private) + (committed and image and writable) + page table
  // Protion of WorkingSet that is not part of  PagefileUsage = (WorkingSet from committed and image and not writable) + (WorkingSet from memory mapped)
  // MemUsedOctave = PagefileUsage + Protion of WorkingSet that is not part of  PagefileUsage

  size_t
  non_private_woking_set (HANDLE hProcess, size_t page_size, const std::vector<std::pair <ULONG_PTR, SIZE_T>>& non_private_virtual)
  {
    size_t pagecount = 0;

    ::PSAPI_WORKING_SET_INFORMATION buf = {};

    BOOL ret = ::QueryWorkingSet (hProcess, &buf, sizeof (buf));

    if (! ret && GetLastError () == ERROR_BAD_LENGTH)
      {
        size_t bufsize = sizeof (::PSAPI_WORKING_SET_INFORMATION) + sizeof (::PSAPI_WORKING_SET_BLOCK) * (buf.NumberOfEntries + 100);

        ::PSAPI_WORKING_SET_INFORMATION* buf1 = (::PSAPI_WORKING_SET_INFORMATION *)std::malloc (bufsize);

        BOOL ret = ::QueryWorkingSet (hProcess, buf1, bufsize);

        if (ret)
          {
            for (size_t i = 0 ; i < buf1->NumberOfEntries; i++)
              {
                ULONG_PTR ws_page_address = buf1->WorkingSetInfo[i].VirtualPage * page_size;

                auto it = std::lower_bound (
                  non_private_virtual.begin (),
                  non_private_virtual.end (),
                  ws_page_address,
                  [] (const std::pair <ULONG_PTR, SIZE_T>& virt, const ULONG_PTR& wrk)
                  {
                    return virt.first < wrk;
                  }
                );

                if (it == non_private_virtual.end ())
                  {
                    continue;
                  }

                if (it->first == ws_page_address)
                  {
                    pagecount++;
                  }
                else
                  {
                    if
                      (
                        it != non_private_virtual.begin ()
                        &&
                        ws_page_address < (std::prev(it)->first + std::prev(it)->second)
                      )
                      {
                        pagecount++;
                      }
                  }
              }
          }

        std::free (buf1);
      }

    return pagecount * page_size;
  }

  OctaveMemoryInfo
  compute_memory_info ()
  {
    HANDLE hProcess;

    if ((hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, ::GetCurrentProcessId ())) == NULL)
      {
  			if ((hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, 0, ::GetCurrentProcessId ())) == NULL)
  				{
  					return {};
          }
      }

    ::MEMORY_BASIC_INFORMATION info {};

    char* lpAddress = nullptr;

    size_t MaxArray = 0;

    auto is_not_writable  = [](DWORD p)
    {
      constexpr const DWORD mask = PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_READWRITE | PAGE_WRITECOPY;

      return ~(p & mask);
    };

    std::vector <std::pair<ULONG_PTR, SIZE_T>> non_private_virtual;

    while (::VirtualQueryEx (hProcess, lpAddress, std::addressof (info), sizeof (info)))
      {
        if (info.State & MEM_FREE)
          {
            MaxArray = std::max ((size_t)info.RegionSize, MaxArray);
          }

        if ((info.State & MEM_COMMIT && info.Type == MEM_IMAGE && is_not_writable (info.Protect)) || info.Type == MEM_MAPPED)
          non_private_virtual.emplace_back ((ULONG_PTR)info.BaseAddress, (SIZE_T)info.RegionSize);

        lpAddress += info.RegionSize;
      }

    ::PERFORMANCE_INFORMATION perfinfo {};

    ::GetPerformanceInfo (std::addressof (perfinfo), sizeof (perfinfo));

    ::MEMORYSTATUSEX memstatus {};

    memstatus.dwLength = sizeof (memstatus);

    ::GlobalMemoryStatusEx (std::addressof (memstatus));

    ::PROCESS_MEMORY_COUNTERS memcounter {};

    ::GetProcessMemoryInfo (hProcess, std::addressof (memcounter), sizeof (memcounter));

    size_t non_private_ws = non_private_woking_set (hProcess, perfinfo.PageSize, non_private_virtual);

    size_t SystemMemoryAvailable =  (perfinfo.CommitLimit - perfinfo.CommitTotal) * perfinfo.PageSize;

    size_t VirtualAddressSpaceTotal = memstatus.ullTotalVirtual;

    size_t VirtualAddressSpaceAvailable = memstatus.ullAvailVirtual;

    size_t MaxPossibleArrayBytes = std::min (MaxArray, SystemMemoryAvailable);

    size_t MemAvailableAllArrays = std::min (VirtualAddressSpaceAvailable, SystemMemoryAvailable);

    size_t MemUsedOctave = memcounter.PagefileUsage + non_private_ws;

    size_t PhysicalMemoryAvailable = perfinfo.PhysicalAvailable * perfinfo.PageSize;

    size_t PhysicalMemoryTotal = perfinfo.PhysicalTotal * perfinfo.PageSize;

    bool MaxPossibleLimited =  MaxPossibleArrayBytes == SystemMemoryAvailable;

    bool MemAvailableLimited = MemAvailableAllArrays == SystemMemoryAvailable;

    return
      {
        SystemMemoryAvailable,
        VirtualAddressSpaceAvailable,
        VirtualAddressSpaceTotal,
        MaxPossibleArrayBytes,
        MemAvailableAllArrays,
        MemUsedOctave,
        PhysicalMemoryAvailable,
        PhysicalMemoryTotal,
        MaxPossibleLimited,
        MemAvailableLimited
      };
  }

  void
  print_mem_info (const OctaveMemoryInfo& mem_info)
  {
    octave_stdout << "\n";

    octave_stdout
      << "Maximum possible array:            "
      << mem_info.MaxPossibleArrayBytes /1024/1024
      << " MB ("
      << double(mem_info.MaxPossibleArrayBytes)
      <<" bytes)";

    if (mem_info.MaxPossibleLimited)
      octave_stdout << " *\n";
    else
      octave_stdout << "\n";

    octave_stdout
      << "Memory available for all arrays:            "
      << mem_info.MemAvailableAllArrays /1024/1024
      << " MB ("
      << double(mem_info.MemAvailableAllArrays)
      <<" bytes)";

    if (mem_info.MemAvailableLimited)
      octave_stdout << " *\n";
    else
      octave_stdout << "\n";

    octave_stdout
      << "Memory used by Octave:            "
      << mem_info.MemUsedOctave /1024/1024
      << " MB ("
      << double(mem_info.MemUsedOctave)
      <<" bytes)\n";

    octave_stdout
      << "Physical Memory (RAM):            "
      << mem_info.PhysicalMemoryTotal /1024/1024
      << " MB ("
      << double(mem_info.PhysicalMemoryTotal)
      <<" bytes)\n";

    if (mem_info.MaxPossibleLimited || mem_info.MemAvailableLimited)
      octave_stdout << "\n*  Limited by System Memory (physical + swap file) available.\n";

    octave_stdout << "\n";
  }
}

DEFUN_DLD (memory, args, nargout,
           R"helpdoc(-*- texinfo -*-
@deftypefn {Loadable Function} memory
@deftypefnx {Loadable Function} {userview =} memory
@deftypefnx {Loadable Function} {[userview, systemview] =} memory

Report memory information (Available only on Microsoft Windows)

Without requestting any output the function will dispaly the following information:@*
Maximum Possible Array@*
Memory Available for All Arrays@*
Memory Used By Octave@*
Physical Memory (RAM)

If one output argument is requested the following information will be returned in a structure 'userview':@*
Maximum Possible Array@*
Memory Available for All Arrays@*
Memory Used By Octave@*

If 'systemview' also is requested the following information is returned:@*
Available Virtual Address Space@*
Total Virtual Address Space@*
Available System Memory@*
Available Physical Memory (RAM)@*
Total Physical Memory (RAM)@*

All values are of type double and are measured in bytes.

Explanation of memory measures *:

@table @asis
@item Total Virtual Address Space
The size of the user-mode portion of the virtual address space of Octave. This value depends on the type of process, the type of processor, and the configuration of the operating system. For example, this value is approximately 2 GB for most 32-bit processes on an x86 processor and approximately 3 GB for 32-bit processes that are large address aware running on a system with 4-gigabyte tuning enabled.
@item Available Virtual Address Space
The amount of free (i.e. unreserved and uncommitted) memory currently in the user-mode portion of the virtual address space of Octave.
@item Total Physical Memory (RAM)
The amount of actual physical memory (RAM).
@item Available Physical Memory (RAM)
The amount of physical memory currently available. This is the amount of physical memory that can be immediately reused without having to write its contents to disk first.
@item Available System Memory
The amount of memory (from overall size of RAM and page file) available to be committed by the system without extending the pagefiles.
@item Maximum Possible Array
The minimum of the largest contiguous free space in virtual address space and the available system memory.
@item Memory Available for All Arrays
It is the minimum of the available virtual address space and the available system memory.
@item Memory Used By Octave
Total amount of memory that has been privately committed for Octave plus non-writable physical memory related to exe and dlls plus physical memory related to memory mapped files.

@end table

* For more information refer to Win32 API documentation.

@end deftypefn)helpdoc")
{
  octave_value_list retval;

  auto mem_info = memory::compute_memory_info ();

  auto nargin = args.length ();

  if (nargin > 0)
    error ("no input is required");

  if (nargout == 0)
    {
      memory::print_mem_info (mem_info);
    }

  if (nargout >= 1)
    {
      octave_value user;

      user = user.subsasgn (".", {ovl ("MaxPossibleArrayBytes")}, double (mem_info.MaxPossibleArrayBytes));
      user = user.subsasgn (".", {ovl ("MemAvailableAllArrays")}, double (mem_info.MemAvailableAllArrays));
      user = user.subsasgn (".", {ovl ("MemUsedOctave")}, double (mem_info.MemUsedOctave));

      retval.append (user);
    }

  if (nargout == 2)
    {
      octave_value sys;

      sys = sys.subsasgn ("..", {ovl ("VirtualAddressSpace"), ovl ("Available")}, double (mem_info.VirtualAddressSpaceAvailable));
      sys = sys.subsasgn ("..", {ovl ("VirtualAddressSpace"), ovl ("Total")}, double (mem_info.VirtualAddressSpaceTotal));
      sys = sys.subsasgn ("..", {ovl ("SystemMemory"), ovl ("Available")}, double (mem_info.SystemMemoryAvailable));
      sys = sys.subsasgn ("..", {ovl ("PhysicalMemory"), ovl ("Available")}, double (mem_info.PhysicalMemoryAvailable));
      sys = sys.subsasgn ("..", {ovl ("PhysicalMemory"), ovl ("Total")}, double (mem_info.PhysicalMemoryTotal));

      retval.append (sys);
    }

  if (nargout > 2)
    error ("number of outputs should be less than or equal to 2");

  return retval;
}