#!/bin/bash

function crtl_core() {
# run with it, generate a core.XXXX file.
	echo -e ""
	echo -e "Try to run with core generater."
	echo -e "	Execute this shell before you execute your binary file, "
	echo -e "  if there are some \"SIGXXX\" happen, will generate a core.XXX file."
	echo -e "  Where there is a SIGSEGV, there is a core.XXX."
	echo -e "  core.XXX: XXX -> pid "
	echo -e "            path -> current directory."
	echo -e " "

	echo 1 > /proc/sys/kernel/core_uses_pid
#echo "./core.%t.%p" >/proc/sys/kernel/core_pattern
#echo "/usr/libexec/abrt-hook-ccpp %s %c %p %u %g %t e %P %I %h" >/proc/sys/kernel/core_pattern
	echo "./core.%p" >/proc/sys/kernel/core_pattern
	echo 0 > /proc/sys/kernel/nmi_watchdog
	echo 1 > /sys/module/rcupdate/parameters/rcu_cpu_stall_suppress
#sysctl -w kernel.sched_rt_runtime_us=-1
}



