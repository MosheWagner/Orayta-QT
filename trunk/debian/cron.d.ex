#
# Regular cron jobs for the orayta package
#
0 4	* * *	root	[ -x /usr/bin/orayta_maintenance ] && /usr/bin/orayta_maintenance
