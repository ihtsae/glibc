/* lxstat using old-style Unix stat system call.
   Copyright (C) 2004-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <https://www.gnu.org/licenses/>.  */

#define __lxstat64 __lxstat64_disable

#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <kernel_stat.h>
#include <sysdep.h>
#include <sys/syscall.h>
#include <xstatconv.h>

#undef __lxstat64


/* Get information about the file NAME in BUF.  */
int
__lxstat (int vers, const char *name, struct stat *buf)
{
  int result;
  struct kernel_stat kbuf;

  if (vers == _STAT_VER_KERNEL64)
    {
      result = INTERNAL_SYSCALL_CALL (lstat64, name, buf);
      if (__glibc_likely (!INTERNAL_SYSCALL_ERROR_P (result)))
	return result;
      __set_errno (INTERNAL_SYSCALL_ERRNO (result));
      return -1;
    }

  result = INTERNAL_SYSCALL_CALL (lstat, name, &kbuf);
  if (__glibc_likely (!INTERNAL_SYSCALL_ERROR_P (result)))
    return __xstat_conv (vers, &kbuf, buf);
  __set_errno (INTERNAL_SYSCALL_ERRNO (result));
  return -1;
}
hidden_def (__lxstat)
weak_alias (__lxstat, _lxstat);
strong_alias (__lxstat, __lxstat64);
hidden_ver (__lxstat, __lxstat64)
