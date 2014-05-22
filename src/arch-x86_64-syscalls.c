/**
 * Enhanced Seccomp x86_64 Syscall Table
 *
 * Copyright (c) 2012 Red Hat <pmoore@redhat.com>
 * Author: Paul Moore <pmoore@redhat.com>
 */

/*
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License as
 * published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses>.
 */

#include <string.h>

#include <seccomp.h>

#include "arch.h"
#include "arch-x86_64.h"

/* NOTE: based on Linux 3.4.7 */
const struct arch_syscall_def x86_64_syscall_table[] = { \
	{ "accept", 43 },
	{ "accept4", 288 },
	{ "access", 21 },
	{ "acct", 163 },
	{ "add_key", 248 },
	{ "adjtimex", 159 },
	{ "afs_syscall", 183 },
	{ "alarm", 37 },
	{ "arm_fadvise64_64", __PNR_arm_fadvise64_64 },
	{ "arm_sync_file_range", __PNR_arm_sync_file_range },
	{ "arch_prctl", 158 },
	{ "bdflush", __PNR_bdflush },
	{ "bind", 49 },
	{ "break", __PNR_break },
	{ "brk", 12 },
	{ "capget", 125 },
	{ "capset", 126 },
	{ "chdir", 80 },
	{ "chmod", 90 },
	{ "chown", 92 },
	{ "chown32", __PNR_chown32 },
	{ "chroot", 161 },
	{ "clock_adjtime", 305 },
	{ "clock_getres", 229 },
	{ "clock_gettime", 228 },
	{ "clock_nanosleep", 230 },
	{ "clock_settime", 227 },
	{ "clone", 56 },
	{ "close", 3 },
	{ "connect", 42 },
	{ "creat", 85 },
	{ "create_module", 174 },
	{ "delete_module", 176 },
	{ "dup", 32 },
	{ "dup2", 33 },
	{ "dup3", 292 },
	{ "epoll_create", 213 },
	{ "epoll_create1", 291 },
	{ "epoll_ctl", 233 },
	{ "epoll_ctl_old", 214 },
	{ "epoll_pwait", 281 },
	{ "epoll_wait", 232 },
	{ "epoll_wait_old", 215 },
	{ "eventfd", 284 },
	{ "eventfd2", 290 },
	{ "execve", 59 },
	{ "exit", 60 },
	{ "exit_group", 231 },
	{ "faccessat", 269 },
	{ "fadvise64", 221 },
	{ "fadvise64_64", __PNR_fadvise64_64 },
	{ "fallocate", 285 },
	{ "fanotify_init", 300 },
	{ "fanotify_mark", 301 },
	{ "fchdir", 81 },
	{ "fchmod", 91 },
	{ "fchmodat", 268 },
	{ "fchown", 93 },
	{ "fchown32", __PNR_fchown32 },
	{ "fchownat", 260 },
	{ "fcntl", 72 },
	{ "fcntl64", __PNR_fcntl64 },
	{ "fdatasync", 75 },
	{ "fgetxattr", 193 },
	{ "finit_module", __PNR_finit_module },
	{ "flistxattr", 196 },
	{ "flock", 73 },
	{ "fork", 57 },
	{ "fremovexattr", 199 },
	{ "fsetxattr", 190 },
	{ "fstat", 5 },
	{ "fstat64", __PNR_fstat64 },
	{ "fstatat64", __PNR_fstatat64 },
	{ "fstatfs", 138 },
	{ "fstatfs64", __PNR_fstatfs64 },
	{ "fsync", 74 },
	{ "ftime", __PNR_ftime },
	{ "ftruncate", 77 },
	{ "ftruncate64", __PNR_ftruncate64 },
	{ "futex", 202 },
	{ "futimesat", 261 },
	{ "get_kernel_syms", 177 },
	{ "get_mempolicy", 239 },
	{ "get_robust_list", 274 },
	{ "get_thread_area", 211 },
	{ "getcpu", 309 },
	{ "getcwd", 79 },
	{ "getdents", 78 },
	{ "getdents64", 217 },
	{ "getegid", 108 },
	{ "getegid32", __PNR_getegid32 },
	{ "geteuid", 107 },
	{ "geteuid32", __PNR_geteuid32 },
	{ "getgid", 104 },
	{ "getgid32", __PNR_getgid32 },
	{ "getgroups", 115 },
	{ "getgroups32", __PNR_getgroups32 },
	{ "getitimer", 36 },
	{ "getpeername", 52 },
	{ "getpgid", 121 },
	{ "getpgrp", 111 },
	{ "getpid", 39 },
	{ "getpmsg", 181 },
	{ "getppid", 110 },
	{ "getpriority", 140 },
	{ "getresgid", 120 },
	{ "getresgid32", __PNR_getresgid32 },
	{ "getresuid", 118 },
	{ "getresuid32", __PNR_getresuid32 },
	{ "getrlimit", 97 },
	{ "getrusage", 98 },
	{ "getsid", 124 },
	{ "getsockname", 51 },
	{ "getsockopt", 55 },
	{ "gettid", 186 },
	{ "gettimeofday", 96 },
	{ "getuid", 102 },
	{ "getuid32", __PNR_getuid32 },
	{ "getxattr", 191 },
	{ "gtty", __PNR_gtty },
	{ "idle", __PNR_idle },
	{ "init_module", 175 },
	{ "inotify_add_watch", 254 },
	{ "inotify_init", 253 },
	{ "inotify_init1", 294 },
	{ "inotify_rm_watch", 255 },
	{ "io_cancel", 210 },
	{ "io_destroy", 207 },
	{ "io_getevents", 208 },
	{ "io_setup", 206 },
	{ "io_submit", 209 },
	{ "ioctl", 16 },
	{ "ioperm", 173 },
	{ "iopl", 172 },
	{ "ioprio_get", 252 },
	{ "ioprio_set", 251 },
	{ "ipc", __PNR_ipc },
	{ "kcmp", 312 },
	{ "kexec_load", 246 },
	{ "keyctl", 250 },
	{ "kill", 62 },
	{ "lchown", 94 },
	{ "lchown32", __PNR_lchown32 },
	{ "lgetxattr", 192 },
	{ "link", 86 },
	{ "linkat", 265 },
	{ "listen", 50 },
	{ "listxattr", 194 },
	{ "llistxattr", 195 },
	{ "_llseek", __PNR__llseek },
	{ "lock", __PNR_lock },
	{ "lookup_dcookie", 212 },
	{ "lremovexattr", 198 },
	{ "lseek", 8 },
	{ "lsetxattr", 189 },
	{ "lstat", 6 },
	{ "lstat64", __PNR_lstat64 },
	{ "madvise", 28 },
	{ "mbind", 237 },
	{ "migrate_pages", 256 },
	{ "mincore", 27 },
	{ "mkdir", 83 },
	{ "mkdirat", 258 },
	{ "mknod", 133 },
	{ "mknodat", 259 },
	{ "mlock", 149 },
	{ "mlockall", 151 },
	{ "mmap", 9 },
	{ "mmap2", __PNR_mmap2 },
	{ "modify_ldt", 154 },
	{ "mount", 165 },
	{ "move_pages", 279 },
	{ "mprotect", 10 },
	{ "mpx", __PNR_mpx },
	{ "mq_getsetattr", 245 },
	{ "mq_notify", 244 },
	{ "mq_open", 240 },
	{ "mq_timedreceive", 243 },
	{ "mq_timedsend", 242 },
	{ "mq_unlink", 241 },
	{ "mremap", 25 },
	{ "msgctl", 71 },
	{ "msgget", 68 },
	{ "msgrcv", 70 },
	{ "msgsnd", 69 },
	{ "msync", 26 },
	{ "munlock", 150 },
	{ "munlockall", 152 },
	{ "munmap", 11 },
	{ "name_to_handle_at", 303 },
	{ "nanosleep", 35 },
	{ "_newselect", __PNR__newselect },
	{ "newfstatat", 262 },
	{ "nfsservctl", 180 },
	{ "nice", __PNR_nice },
	{ "oldfstat", __PNR_oldfstat },
	{ "oldlstat", __PNR_oldlstat },
	{ "oldolduname", __PNR_oldolduname },
	{ "oldstat", __PNR_oldstat },
	{ "olduname", __PNR_olduname },
	{ "open", 2 },
	{ "open_by_handle_at", 304 },
	{ "openat", 257 },
	{ "pause", 34 },
	{ "pciconfig_iobase", __PNR_pciconfig_iobase },
	{ "pciconfig_read", __PNR_pciconfig_read },
	{ "pciconfig_write", __PNR_pciconfig_write },
	{ "perf_event_open", 298 },
	{ "personality", 135 },
	{ "pipe", 22 },
	{ "pipe2", 293 },
	{ "pivot_root", 155 },
	{ "poll", 7 },
	{ "ppoll", 271 },
	{ "prctl", 157 },
	{ "pread64", 17 },
	{ "preadv", 295 },
	{ "prlimit64", 302 },
	{ "process_vm_readv", 310 },
	{ "process_vm_writev", 311 },
	{ "prof", __PNR_prof },
	{ "profil", __PNR_profil },
	{ "pselect6", 270 },
	{ "ptrace", 101 },
	{ "putpmsg", 182 },
	{ "pwrite64", 18 },
	{ "pwritev", 296 },
	{ "query_module", 178 },
	{ "quotactl", 179 },
	{ "read", 0 },
	{ "readahead", 187 },
	{ "readdir", __PNR_readdir },
	{ "readlink", 89 },
	{ "readlinkat", 267 },
	{ "readv", 19 },
	{ "reboot", 169 },
	{ "recv", __PNR_recv },
	{ "recvfrom", 45 },
	{ "recvmmsg", 299 },
	{ "recvmsg", 47 },
	{ "remap_file_pages", 216 },
	{ "removexattr", 197 },
	{ "rename", 82 },
	{ "renameat", 264 },
	{ "request_key", 249 },
	{ "restart_syscall", 219 },
	{ "rmdir", 84 },
	{ "rt_sigaction", 13 },
	{ "rt_sigpending", 127 },
	{ "rt_sigprocmask", 14 },
	{ "rt_sigqueueinfo", 129 },
	{ "rt_sigreturn", 15 },
	{ "rt_sigsuspend", 130 },
	{ "rt_sigtimedwait", 128 },
	{ "rt_tgsigqueueinfo", 297 },
	{ "sched_get_priority_max", 146 },
	{ "sched_get_priority_min", 147 },
	{ "sched_getaffinity", 204 },
	{ "sched_getparam", 143 },
	{ "sched_getscheduler", 145 },
	{ "sched_rr_get_interval", 148 },
	{ "sched_setaffinity", 203 },
	{ "sched_setparam", 142 },
	{ "sched_setscheduler", 144 },
	{ "sched_yield", 24 },
	{ "security", 185 },
	{ "select", 23 },
	{ "semctl", 66 },
	{ "semget", 64 },
	{ "semop", 65 },
	{ "semtimedop", 220 },
	{ "send", __PNR_send },
	{ "sendfile", 40 },
	{ "sendfile64", __PNR_sendfile64 },
	{ "sendmmsg", 307 },
	{ "sendmsg", 46 },
	{ "sendto", 44 },
	{ "set_mempolicy", 238 },
	{ "set_robust_list", 273 },
	{ "set_thread_area", 205 },
	{ "set_tid_address", 218 },
	{ "setdomainname", 171 },
	{ "setfsgid", 123 },
	{ "setfsgid32", __PNR_setfsgid32 },
	{ "setfsuid", 122 },
	{ "setfsuid32", __PNR_setfsuid32 },
	{ "setgid", 106 },
	{ "setgid32", __PNR_setgid32 },
	{ "setgroups", 116 },
	{ "setgroups32", __PNR_setgroups32 },
	{ "sethostname", 170 },
	{ "setitimer", 38 },
	{ "setns", 308 },
	{ "setpgid", 109 },
	{ "setpriority", 141 },
	{ "setregid", 114 },
	{ "setregid32", __PNR_setregid32 },
	{ "setresgid", 119 },
	{ "setresgid32", __PNR_setresgid32 },
	{ "setresuid", 117 },
	{ "setresuid32", __PNR_setresuid32 },
	{ "setreuid", 113 },
	{ "setreuid32", __PNR_setreuid32 },
	{ "setrlimit", 160 },
	{ "setsid", 112 },
	{ "setsockopt", 54 },
	{ "settimeofday", 164 },
	{ "setuid", 105 },
	{ "setuid32", __PNR_setuid32 },
	{ "setxattr", 188 },
	{ "sgetmask", __PNR_sgetmask },
	{ "shmat", 30 },
	{ "shmctl", 31 },
	{ "shmdt", 67 },
	{ "shmget", 29 },
	{ "shutdown", 48 },
	{ "sigaction", __PNR_sigaction },
	{ "sigaltstack", 131 },
	{ "signal", __PNR_signal },
	{ "signalfd", 282 },
	{ "signalfd4", 289 },
	{ "sigpending", __PNR_sigpending },
	{ "sigprocmask", __PNR_sigprocmask },
	{ "sigreturn", __PNR_sigreturn },
	{ "sigsuspend", __PNR_sigsuspend },
	{ "socket", 41 },
	{ "socketcall", __PNR_socketcall },
	{ "socketpair", 53 },
	{ "splice", 275 },
	{ "ssetmask", __PNR_ssetmask },
	{ "stat", 4 },
	{ "stat64", __PNR_stat64 },
	{ "statfs", 137 },
	{ "statfs64", __PNR_statfs64 },
	{ "stime", __PNR_stime },
	{ "stty", __PNR_stty },
	{ "swapoff", 168 },
	{ "swapon", 167 },
	{ "symlink", 88 },
	{ "symlinkat", 266 },
	{ "sync", 162 },
	{ "sync_file_range", 277 },
	{ "sync_file_range2", __PNR_sync_file_range2 },
	{ "syncfs", 306 },
	{ "syscall", __PNR_syscall },
	{ "_sysctl", 156 },
	{ "sysfs", 139 },
	{ "sysinfo", 99 },
	{ "syslog", 103 },
	{ "tee", 276 },
	{ "tgkill", 234 },
	{ "time", 201 },
	{ "timer_create", 222 },
	{ "timer_delete", 226 },
	{ "timer_getoverrun", 225 },
	{ "timer_gettime", 224 },
	{ "timer_settime", 223 },
	{ "timerfd_create", 283 },
	{ "timerfd_gettime", 287 },
	{ "timerfd_settime", 286 },
	{ "times", 100 },
	{ "tkill", 200 },
	{ "truncate", 76 },
	{ "truncate64", __PNR_truncate64 },
	{ "tuxcall", 184 },
	{ "ugetrlimit", __PNR_ugetrlimit },
	{ "ulimit", __PNR_ulimit },
	{ "umask", 95 },
	{ "umount", __PNR_umount },
	{ "umount2", 166 },
	{ "uname", 63 },
	{ "unlink", 87 },
	{ "unlinkat", 263 },
	{ "unshare", 272 },
	{ "uselib", 134 },
	{ "ustat", 136 },
	{ "utime", 132 },
	{ "utimensat", 280 },
	{ "utimes", 235 },
	{ "vfork", 58 },
	{ "vhangup", 153 },
	{ "vm86", __PNR_vm86 },
	{ "vm86old", __PNR_vm86old },
	{ "vmsplice", 278 },
	{ "vserver", 236 },
	{ "wait4", 61 },
	{ "waitid", 247 },
	{ "waitpid", __PNR_waitpid },
	{ "write", 1 },
	{ "writev", 20 },
	{ NULL, __NR_SCMP_ERROR },
};

/**
 * Resolve a syscall name to a number
 * @param name the syscall name
 *
 * Resolve the given syscall name to the syscall number using the syscall table.
 * Returns the syscall number on success, including negative pseudo syscall
 * numbers; returns __NR_SCMP_ERROR on failure.
 *
 */
int x86_64_syscall_resolve_name(const char *name)
{
	unsigned int iter;
	const struct arch_syscall_def *table = x86_64_syscall_table;

	/* XXX - plenty of room for future improvement here */
	for (iter = 0; table[iter].name != NULL; iter++) {
		if (strcmp(name, table[iter].name) == 0)
			return table[iter].num;
	}

	return __NR_SCMP_ERROR;
}

/**
 * Resolve a syscall number to a name
 * @param num the syscall number
 *
 * Resolve the given syscall number to the syscall name using the syscall table.
 * Returns a pointer to the syscall name string on success, including pseudo
 * syscall names; returns NULL on failure.
 *
 */
const char *x86_64_syscall_resolve_num(int num)
{
	unsigned int iter;
	const struct arch_syscall_def *table = x86_64_syscall_table;

	/* XXX - plenty of room for future improvement here */
	for (iter = 0; table[iter].num != __NR_SCMP_ERROR; iter++) {
		if (num == table[iter].num)
			return table[iter].name;
	}

	return NULL;
}

/**
 * Iterate through the syscall table and return the syscall name
 * @param spot the offset into the syscall table
 *
 * Return the syscall name at position @spot or NULL on failure.  This function
 * should only ever be used internally by libseccomp.
 *
 */
const char *x86_64_syscall_iterate_name(unsigned int spot)
{
	/* XXX - no safety checks here */
	return x86_64_syscall_table[spot].name;
}

