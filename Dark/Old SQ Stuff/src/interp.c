/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



bool	check_social	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );



/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2

#define AFK             1

/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;

/*
 * level of person who just used interp.
 *
 * used to prevent lower level gods from seeing what higher level
 * gods do on the log channel.
 */
int				interp_doer;


/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */
    { "north",		do_north,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "east",		do_east,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "south",		do_south,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "west",		do_west,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "up",		do_up,		POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "down",		do_down,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "downtime",       do_downtime,    POS_DEAD,       40,  LOG_ALWAYS, TRUE,
        NULL, TRUE },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "afk",		do_afk,		POS_DEAD,      AFK,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "buy",		do_buy,		POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "cast",		do_cast,	POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "contact",        do_contact,     POS_RESTING,     0,  LOG_NORMAL, TRUE,
        NULL, TRUE },
    { "change",		do_change,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "exits",		do_exits,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "get",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "inventory",	do_inventory,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "kill",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "knock",          do_knock,       POS_STANDING,    0,  LOG_NORMAL, TRUE,
        NULL, TRUE },
    { "look",		do_look,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "order",		do_order,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "rest",		do_rest,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "shoot",		do_shoot,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "sit",		do_sit,		POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "stand",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "wield",		do_wield,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "goto",		do_newgoto,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "wizhelp",	do_wizhelp,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
	{ "wset",       do_wset,    POS_DEAD,   39,  LOG_ALWAYS, FALSE,
	NULL, FALSE },

    /*
     * Informational commands.
     */
    { "areas",		do_areas,	POS_DEAD,	36,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "bug",		do_bug,		POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "commands",	do_commands,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "compare",	do_compare,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "consider",	do_consider,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "credits",	do_credits,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "equipment",	do_equipment,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "ep",             do_wield,       POS_SITTING,     0, LOG_NORMAL,  FALSE,
        NULL, FALSE },
    { "es",             do_hold,        POS_SITTING,     0,  LOG_NORMAL, FALSE,
        NULL, FALSE },
    { "examine",	do_examine,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE},
    { "help",		do_help,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "idea",		do_idea,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "score",		do_score,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "socials",	do_socials,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "time",		do_time,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "typo",		do_typo,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "weather",	do_weather,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "who",		do_who,		POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },

    /*
     * Configuration commands.
     */
    { "brief",		do_brief,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "compact",	do_compact,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "channels",	do_channels,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "config",		do_config,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
/*
    { "description",	do_description,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
*/
    { "password",	do_password,	POS_DEAD,	 0,  LOG_NEVER,  FALSE,
	NULL, FALSE },
    { "set",		do_set,	POS_DEAD,		 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },

    /*
     * Communication commands.
     */
//    { "chat",		do_chat,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
//	NULL, FALSE  },
 //   { "gossip",		do_chat,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
//	NULL, FALSE  },
//    { ".",		do_chat,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
//	NULL, FALSE  },
    { "emote",		do_emote,	POS_SLEEPING,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE  },
    { ",",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE  },
/*
    { "gtell",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE  },
    { ";",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE  },
*/
    { "nmodify",	do_nmodify,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "note",		do_note,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "pemote",		do_pemote,	POS_SLEEPING,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE  },
    { "reply",		do_reply,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "wish",		do_wish,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "say",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE },
    { "semote",         do_semote,      POS_SLEEPING,    0,  LOG_NORMAL, TRUE,
      NULL, FALSE },
    { "spemote",         do_spemote,      POS_SLEEPING,    0,  LOG_NORMAL, TRUE,
      NULL, FALSE },
    { "'",		do_say,		POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE },
    { "tele",           do_tele,        POS_RESTING,     0, LOG_NORMAL, TRUE,
       NULL, TRUE },
    { "tell",		do_tell,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE },
    { "ooc",		do_ooc,		POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, FALSE },
    { "shout",		do_shout,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },

    /*
     * Object manipulation commands.
     */
    { "close",		do_close,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "draw",		do_draw,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "drink",		do_drink,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE},
    { "drop",		do_drop,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "eat",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "fill",		do_fill,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "pour",		do_pour,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "give",		do_give,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "junk",		do_junk,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "hold",		do_hold,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "list",		do_list,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "lock",		do_lock,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "open",		do_open,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "pick",		do_pick,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "put",		do_put,		POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "quaff",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "recite",		do_recite,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "remove",		do_remove,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "sell",		do_sell,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "skin",		do_skin,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "sacrifice",	do_sacrifice,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "sheathe",		do_sheath,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "unlink",         do_unlink,      POS_RESTING,     0,  LOG_NORMAL, TRUE,
        NULL, FALSE },
    { "unlock",		do_unlock,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "use",		do_use,		POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "value",		do_value,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "wear",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },

    /*
     * Combat commands.
     */
    { "backstab",	do_backstab,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "bash",		do_bash,	POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "bs",		do_backstab,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "disarm",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "flee",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "kick",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "rescue",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },

    /*
     * Miscellaneous commands.
     */
    { "dismount",	do_dismount,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "drive",		do_drive,	POS_STANDING,    0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "enter",		do_enter,	POS_STANDING,    0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "follow",		do_follow,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
/*
    { "group",		do_group,	POS_SLEEPING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
*/
    { "hide",		do_hide,	POS_STANDING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "jump",		do_jump,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "leave",		do_leave,	POS_STANDING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "prompt",		do_prompt,	POS_SLEEPING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "ride",		do_ride,	POS_STANDING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "spells",		do_spells,	POS_SLEEPING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "skills",		do_skills,	POS_SLEEPING,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "qui",		do_qui,		POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE},
    { "quit",		do_quit,	POS_DEAD,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "run",		do_run,		POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "save",		do_save,	POS_DEAD,      AFK,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "sneak",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "steal",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL, FALSE,
	predelay_steal, TRUE },
    { "visible",	do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL, TRUE,
	NULL, TRUE },
    { "walk",		do_walk,	POS_RESTING,	 0,  LOG_NORMAL, FALSE,
	NULL, TRUE },
    { "where",		do_where,	POS_RESTING,	36,  LOG_NORMAL, FALSE,
	NULL, TRUE },



    /*
     * Immortal commands.
     */
    { "approve",	do_approve,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "reject",		do_reject,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "rlist",		do_rlist,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "rload",		do_rload,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "pload",          do_pload,       POS_DEAD,       40,  LOG_ALWAYS, FALSE,
        NULL, FALSE },
    { "punload",        do_punload,     POS_DEAD,       38,  LOG_ALWAYS, FALSE,
        NULL, FALSE },
    { "advance",	do_advance,	POS_DEAD,	40,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "trust",		do_trust,	POS_DEAD,	40,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "hset",		do_hset,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "memory",		do_memory,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "newhelp",	do_newhelp,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "savehelps",	do_savehelps,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },

    { "allow",		do_allow,	POS_DEAD,	40,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "ban",		do_ban,		POS_DEAD,	40,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "deny",		do_deny,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "disconnect",	do_disconnect,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "freeze",		do_freeze,	POS_DEAD,	40,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "reboo",		do_reboo,	POS_DEAD,	39,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "reboot",		do_reboot,	POS_DEAD,	39,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "send",		do_send,	POS_DEAD,	39,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "shutdow",	do_shutdow,	POS_DEAD,	39,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "shutdown",	do_shutdown,	POS_DEAD,	39,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "users",		do_users,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "ulog",           do_ulog,        POS_DEAD,       40,  LOG_NORMAL, FALSE,
        NULL, FALSE },
    { "wizlock",	do_wizlock,	POS_DEAD,	39,  LOG_ALWAYS, FALSE,
	NULL, FALSE },

    { "echo",		do_echo,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "force",		do_force,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "load",           do_load,        POS_DEAD,       36,  LOG_ALWAYS, FALSE, 
        NULL, FALSE },
    { "log",		do_log,		POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "oset",		do_oset,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "mset",		do_mset,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "noemote",	do_noemote,	POS_DEAD,	39,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "nowish",		do_notell,	POS_DEAD,	40,  LOG_NORMAL, FALSE,
	NULL, FALSE },
//    { "randarmor",	do_randarmor,	POS_DEAD,	38,  LOG_NORMAL, FALSE,
//	NULL, FALSE },
//    { "randmagic",	do_randmagic,	POS_DEAD,	38,  LOG_NORMAL, FALSE,
//	NULL, FALSE },
//    { "randpotion",	do_randpotion,	POS_DEAD,	38,  LOG_NORMAL, FALSE,
//	NULL, FALSE },
    { "restore",	do_restore,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "return",		do_return,	POS_DEAD,	36,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "sla",		do_sla,		POS_DEAD,	38,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "slay",		do_slay,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "slookup",	do_slookup,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "snoop",		do_snoop,	POS_DEAD,	36,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "sset",		do_sset,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "switch",		do_switch,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "viewskills",	do_viewskills,	POS_DEAD,	36,  LOG_NORMAL, FALSE,
	NULL, FALSE },

    { "aset",		do_aset,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "breakexit",		do_break,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "cload",		do_cload,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "collaps",	do_collaps,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "collapse",	do_collapse,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "connect",	do_connect,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "dig",		do_dig,		POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "export",		do_export,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "flora",		do_flora,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "import",		do_import,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "exset",		do_exset,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "material",	do_material,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "mfind",		do_mfind,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "mload",		do_mload,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "mpset",		do_mpset,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "mstat",		do_mstat,	POS_DEAD,	36,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "mwhere",		do_mwhere,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "newmob",		do_newmob,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "newobj",		do_newobj,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "ofind",		do_ofind,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "oload",		do_oload,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "opset",		do_opset,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "ostat",		do_ostat,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "rstat",		do_rstat,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "purge",		do_purge,	POS_DEAD,	35,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "rdesc",		do_rdesc,	POS_DEAD,	37,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "reset",		do_reset,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "rextra",		do_rextra,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "rset",		do_rset,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "savezone",	do_savezone,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "sector",		do_sector,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
//    { "shopset",	do_shopset,	POS_DEAD,	40,  LOG_ALWAYS, FALSE,
//	NULL, FALSE },
    { "transfer",	do_transfer,	POS_DEAD,	38,  LOG_ALWAYS, FALSE,
	NULL, FALSE },
    { "vmob",		do_vmob,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "vobj",		do_vobj,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "xload",		do_xload,	POS_DEAD,	37,  LOG_ALWAYS, FALSE,
	NULL, FALSE },

    { "at",		do_at,		POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "bamfin",		do_bamfin,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "bamfout",	do_bamfout,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "history",        do_history,     POS_DEAD,       35,  LOG_NORMAL, FALSE, NULL, FALSE },
//    { "holy",           do_holy,        POS_DEAD,       35,  LOG_NORMAL, FALSE, NULL, TRUE },
    { "holylist",	do_holylist,	POS_DEAD,	35,  LOG_NORMAL, FALSE, NULL, FALSE },
    { "holylight",	do_holylight,	POS_DEAD,	35,  LOG_NORMAL, FALSE, NULL, FALSE },
    { "holyname",	do_holyname,	POS_DEAD,	35,  LOG_NORMAL, FALSE, NULL, FALSE },
    { "holyspeech", 	do_holyspeech,	POS_DEAD,	35,  LOG_NORMAL, FALSE, NULL, FALSE },
    { "holywalk",	do_holywalk,	POS_DEAD,	35,  LOG_NORMAL, FALSE, NULL, FALSE },
    { "holyvnum",	do_holyvnum,	POS_DEAD,	35,  LOG_NORMAL, FALSE, NULL, FALSE },

    { "invis",		do_invis,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "peace",		do_peace,	POS_DEAD,	36,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    /* out of order to prevent blocking peace */
    { "peaceful",	do_peaceful,	POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { "recho",		do_recho,	POS_DEAD,	36,  LOG_ALWAYS, FALSE,
	NULL, FALSE },

    { "immtalk",	do_immtalk,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },
    { ":",		do_immtalk,	POS_DEAD,	35,  LOG_NORMAL, FALSE,
	NULL, FALSE },

    /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL, FALSE,
	NULL, FALSE }
};



/*
 * The social table.
 * Add new socials here.
 * Alphabetical order is not required.
 */
const	struct	social_type	social_table [] =
{
    {
	"applaud",
	"Clap, clap, clap.",
	"$n gives a round of applause.",
	"You clap at $S actions.",
	"$n claps at $N's actions.",
	"$n gives you a round of applause.",
	"You applaud at yourself.",
	"$n applauds at $mself.",
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"blush",
	"Your cheeks are burning.",
	"$n blushes.",
	"You get all flustered up seeing $M.",
	"$n blushes as $e sees $N here.",
	"$n blushes as $e sees you here.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"bow",
	"You bow deeply.",
	"$n bows deeply.",
	"You bow before $M.",
	"$n bows before $N.",
	"$n bows before you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"burp",
	"You burp loudly.",
	"$n burps loudly.",
	"You burp loudly at $M.",
	"$n burps loudly in $N's general direction.",
	"$n burps loudly in your general direction.",
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"cackle",
	"You throw back your head and cackle with insane glee!",
	"$n throws back $s head and cackles with insane glee!",
	"You cackle gleefully at $N",
	"$n cackles gleefully at $N.",
	"$n cackles gleefully at you.",
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"chuckle",
	"You chuckle politely.",
	"$n chuckles politely.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"clap",
	"You clap your hands together.",
	"$n shows $s approval by clapping $s hands together.",
	"You clap at $S performance.",
	"$n claps at $N's performance.",
	"$n claps at your performance.",
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"comfort",
	NULL,
	NULL,
	"You comfort $M.",
	"$n comforts $N.",
	"$n comforts you.",
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING|SENSE_TOUCH
    },

    {
	"cringe",
	"You cringe in terror.",
	"$n cringes in terror!",
	"You cringe away from $M.",
	"$n cringes away from $N.",
	"$n cringes away from you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"cry",
	"Waaaaah ...",
	"$n bursts into tears.",
	"You cry on $S shoulder.",
	"$n cries on $N's shoulder.",
	"$n cries on your shoulder.",
	"You cry to yourself.",
	"$n sobs quietly to $mself.",
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"curtsey",
	"You curtsey to your audience.",
	"$n curtseys gracefully.",
	"You curtsey to $M.",
	"$n curtseys gracefully to $N.",
	"$n curtseys gracefully for you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"frown",
	"What's bothering you ?",
	"$n frowns.",
	"You frown at $E.",
	"$n frowns at $N.",
	"$n frowns at you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"gasp",
	"You gasp in astonishment.",
	"$n gasps in astonishment.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"giggle",
	"You giggle.",
	"$n giggles.",
	"You giggle at $M.",
	"$n giggles at $N.",
	"$n giggles at you.",
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"glare",
	"You glare at nothing in particular.",
	"$n glares around $m.",
	"You glare icily at $M.",
	"$n glares at $N.",
	"$n glares icily at you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"grin",
	"You grin evilly.",
	"$n grins evilly.",
	"You grin evilly at $M.",
	"$n grins evilly at $N.",
	"$n grins evilly at you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"growl",
	"Grrrrrrrrrr ...",
	"$n growls.",
	"You growl at $M.",
	"$n growls at $N.",
	"$n growls at you.",
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"grumble",
	"You grumble about something.",
	"$n grumbles about something.",
	"You grumble at $M.",
	"$n grumbles at $N.",
	"$n grumbles at you.",
	"You grumble under your breath.",
	"$n grumbles under $s breath.",
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"hug",
	"Hug whom?",
	NULL,
	"You hug $M.",
	"$n hugs $N.",
	"$n hugs you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT|SENSE_TOUCH
    },

    {
	"kiss",
	"Isn't there someone you want to kiss?",
	NULL,
	"You kiss $M.",
	"$n kisses $N.",
	"$n kisses you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT|SENSE_TOUCH
    },

    {
	"laugh",
	"You fall down laughing.",
	"$n falls down laughing.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"nod",
	"You nod.",
	"$n nods.",
	"You nod to $M.",
	"$n nods to $N.",
	"$n nods to you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"peer",
	"You peer all around.",
	"$n peers all around.",
	"You peer at $N intently.",
	"$n peers at $N intently.",
	"$n peers at you intently.",
	"How do you peer at yourself?",
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"point",
	"Point at whom?",
	NULL,
	"You point at $M.",
	"$n points at $N.",
	"$n points at you.",
	"You point at yourself.",
	"$n points at $mself.",
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"poke",
	"Poke whom?",
	NULL,
	"You poke $M in the ribs.",
	"$n pokes $N in the ribs.",
	"$n pokes you in the ribs.",
	"You poke yourself in the ribs.",
	"$n pokes $mself in the ribs.",
	SENSE_SIGHT,
	SENSE_SIGHT|SENSE_TOUCH
    },

    {
	"scream",
	"ARRRRRRRRRRGH!!!!!",
	"$n screams loudly!",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"shake",
	"You shake your head.",
	"$n shakes $s head.",
	"You shake $S hand.",
	"$n shakes $N's hand.",
	"$n shakes your hand.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT|SENSE_TOUCH
    },

    {
	"shiver",
	"Brrrrrrrrr.",
	"$n shivers uncomfortably.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"shrug",
	"You shrug.",
	"$n shrugs helplessly.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"sigh",
	"You sigh.",
	"$n sighs loudly.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"sing",
	"You raise your clear voice towards the sky.",
	"PANIC!!! $n has begun to sing.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"smile",
	"You smile happily.",
	"$n smiles happily.",
	"You smile at $M.",
	"$n beams a smile at $N.",
	"$n smiles at you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"smirk",
	"You smirk.",
	"$n smirks.",
	"You smirk at $S.",
	"$n smirks at $N.",
	"$n smirks at you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"snap",
	"You snap your fingers.",
	"$n snaps $s fingers.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"sneeze",
	"Gesundheit!",
	"$n sneezes.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"snicker",
	"You snicker softly.",
	"$n snickers softly.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"sniff",
	"You sniff sadly.",
	"$n sniffs sadly.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"snore",
	"Zzzzzzzzzzzzzzzzz.",
	"$n snores loudly.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"stare",
	"You stare at the sky.",
	"$n stares at the sky.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"thank",
	NULL,
	NULL,
	"You thank $N heartily.",
	"$n thanks $N heartily.",
	"$n thanks you heartily.",
	NULL,
	NULL,
	SENSE_SIGHT|SENSE_HEARING,
	SENSE_SIGHT|SENSE_HEARING
    },

    {
	"tickle",
	"Whom do you want to tickle?",
	NULL,
	"You tickle $N.",
	"$n tickles $N.",
	"$n tickles you - hee hee hee.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT|SENSE_TOUCH
    },

    {
	"wave",
	"You wave.",
	"$n waves happily.",
	"You wave goodbye to $N.",
	"$n waves goodbye to $N.",
	"$n waves goodbye to you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"wink",
	"You wink suggestively.",
	"$n winks suggestively.",
	"You wink suggestively at $N.",
	"$n winks at $N.",
	"$n winks suggestively at you.",
	NULL,
	NULL,
	SENSE_SIGHT,
	SENSE_SIGHT
    },

    {
	"",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0
    }
};



/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    int cmd;
    int trust;
    int position;
    bool found;

/* inserting wait check for mobiles here to get around order etc. */
    if ( ch->wait > 0 ) {
       send_to_char("You're still busy with the last thing you did!\n\r", ch );
       return;
    }

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    if ( IS_NPC( ch ) && ch->desc == NULL )
      interp_doer = 0;
    else
      interp_doer = UMIN( MAX_LEVEL, get_trust( ch ) );

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	interp_doer = 0;
	return;
    }

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    found = TRUE;
	    break;
	}
    }


    if (IS_SET(ch->act, PLR_AFK) 
	 && (cmd_table[cmd].level < AFK)) {    
        // quick & dirty hack to allow wizcmds without breaking afk
	send_to_char("You are gone, remember?\n\r", ch);
	interp_doer = 0;
	return;
    }

    /*
     * No hiding.
     */
    if ( cmd_table[cmd].motion == TRUE )
	REMOVE_BIT( ch->affected_by, AFF_HIDE );

    /*
     * Lose concentration.
     */
    if ( cmd_table[cmd].concentration == TRUE && ch->predelay_time > 0 )
    {
	send_to_char( "You stop concentrating...\n\r", ch );
	ch->predelay_time = 0;
	free_predelay( ch->predelay_info );
	ch->predelay_info = NULL;
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );

    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS )
    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    /*
     * Character not in position for command?
     */
    position = ch->position;
    if ( check_fighting( ch ) && ch->position == POS_STANDING )
	position = POS_FIGHTING;

    if ( found && position < cmd_table[cmd].position )
    {
	switch( position )
	{
	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are fighting for your life!\n\r", ch);
	    break;

	}
	interp_doer = 0;
	return;
    }

    if ( check_specials( ch, command,
	    ( found ? *cmd_table[cmd].do_fun : NULL ),
	    argument ) )
    {
        interp_doer = 0;
	return;
    }

    /*
     * Dispatch the command.
     */
    if ( found )
    {
	if ( cmd_table[cmd].delay_fun == NULL )
	    (*cmd_table[cmd].do_fun) ( ch, argument );
	else
	    (*cmd_table[cmd].delay_fun) ( ch, argument );
    }
    else
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	    send_to_char( "Huh?\n\r", ch );
        interp_doer = 0;
	return;
    }

    interp_doer = 0;
    tail_chain( );
    return;
}


bool check_specials( CHAR_DATA *ch, char *cmd, DO_FUN *fnptr, char *argument )
{
    CHAR_DATA *mob;
    OBJ_DATA *obj;

    /* worn */
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	if ( obj->wear_loc != gn_wear_none && obj->spec_fun != NULL )
	    if ( (*obj->spec_fun) (ch, cmd, fnptr, argument, obj ) )
		return TRUE;

    /* inventory */
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	if ( obj->wear_loc == gn_wear_none && obj->spec_fun != NULL )
	    if ( (*obj->spec_fun) (ch, cmd, fnptr, argument, obj ) )
		return TRUE;

    /* stuff in room */
    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
	if ( obj->spec_fun != NULL )
	    if ( (*obj->spec_fun) (ch, cmd, fnptr, argument, obj ) )
		return TRUE;

    /* mobiles */
    for ( mob = ch->in_room->people; mob != NULL; mob = mob->next_in_room )
	if ( mob->spec_fun != NULL )
	    if ( (*mob->spec_fun) ( ch, cmd, fnptr, argument, mob ) )
		return TRUE;

    /* room */
    if ( ch->in_room->spec_fun != NULL )
	if ( (*ch->in_room->spec_fun)( ch, cmd, fnptr, argument, ch->in_room ) )
	    return TRUE;

    return FALSE;
}


bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_NO_EMOTE) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, victim, NULL, NULL, NULL, TO_ROOM, social_table[cmd].room_sense );
	act( social_table[cmd].char_no_arg, ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH);
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto, ch, victim, NULL, NULL, NULL, TO_ROOM, social_table[cmd].room_sense );
	act( social_table[cmd].char_auto, ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
    }
    else
    {
	act( social_table[cmd].others_found, ch, victim, NULL, NULL, NULL, TO_NOTVICT, social_table[cmd].room_sense );
	act( social_table[cmd].char_found, ch, victim, NULL, NULL, NULL, TO_CHAR, SENSE_SIXTH );
	act( social_table[cmd].vict_found, ch, victim, NULL, NULL, NULL, TO_VICT, social_table[cmd].vict_sense );
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit(*arg) && *arg != '-' )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}


char *strip_punct( char *arg )
{
/* strip trailing punctuation */
    int i;

    for( i = strlen(arg); --i >= 0; )
	if (ispunct(arg[i]))
	    arg[i]= '\0';
	else
	    break;

    return arg;
}

/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

char *one_emote_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}

	*arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

char *plural( char *arg )
{
  static char buf[MAX_STRING_LENGTH];
  int len;

  strcpy( buf, arg );
  len = strlen( arg );
  buf[len] = '\0';

  if ( buf[len-1] == 's' )
  {
    if ( buf[len-2] == 'u' )  /* cactus -> cacti */
    {
      buf[len-2] = 'i';
      buf[len-1] = '\0';
    }
    else  /* already plural */
      return arg;
  }
  else if ( buf[len-1] == 'h' )
    strcat( buf, "es" );
  else  /* anything else */
    strcat( buf, "s" );

  return buf;
}
