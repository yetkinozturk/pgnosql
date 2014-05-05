#ifndef __COMMANDS_H__
#define __COMMANDS_H__

// Define max/min length of commands
#define MINCMDLEN 6
#define MAXCMDLEN 1024*1024*1024

// Prefix definitions.
#define PREFIXCOMMIT		"COMMIT"
#define PREFIXFLUSHALL		"FLUSHALL"
#define PREFIXFLUSHKV		"FLUSHKV"
#define PREFIXSETKV			"SETKV"
#define PREFIXGETKV			"GETKV"
#define PREFIXDELKV			"DELKV"
#define PREFIXGETKEYS		"GETKEYS"
#define PREFIXGETHOLDERS	"GETHOLDERS"
#define PREFIXNEWHOLDER		"NEWHOLDER"
#define PREFIXEXISTHOLDER   "EXISTHOLDER"
#define PREFIXMODHOLDER		"MODHOLDER"
#define PREFIXOPHOLDER		"OPHOLDER"
#define PREFIXDELHOLDER		"DELHOLDER"

// Max number of arguments.
#define MAXARGCOMMIT		1
#define MAXARGFLUSHALL	    1
#define MAXARGFLUSHKV		1
#define MAXARGSETKV         4
#define MAXARGGETKV         2
#define MAXARGDELKV         2
#define MAXARGGETKEYS       2
#define MAXARGGETHOLDERS	2
#define MAXARGNEWHOLDER     3
#define MAXARGEXISTHOLDER   2
#define MAXARGMODHOLDER     5
#define MAXARGOPHOLDER		5
#define MAXARGDELHOLDER     2

// Min number of arguments.
#define MINARGCOMMIT		1
#define MINARGFLUSHALL      1
#define MINARGFLUSHKV		1
#define MINARGSETKV         3
#define MINARGGETKV         2
#define MINARGDELKV         2
#define MINARGGETKEYS       2
#define MINARGGETHOLDERS	2
#define MINARGNEWHOLDER     3
#define MINARGEXISTHOLDER   2
#define MINARGMODHOLDER     4
#define MINARGOPHOLDER		3
#define MINARGDELHOLDER     2

#endif
