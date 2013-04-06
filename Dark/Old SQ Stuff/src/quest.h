

struct quest_mission
{
  struct quest_mission *next;
  struct quest_task    *tasks;
  char                 *name;             /* displayed to player */
  int                  vnum_source;       /* vnum of npc that issued */
  int                  organization;      /* what group sponsored */
  int                  prestige_success;  /* if all tasks succeed */
  int                  prestige_failure;  /* if any tasks fail */
  int                  time_expire;       /* updated on login */
};

struct quest_task
{
  struct quest_task    *next;
  char                 *name;             /* displayed to player */
  int                  status;            /* active/failed, etc. */
  int                  action;            /* what to do */
  int                  object;            /* what object to do it with */
  int                  target;            /* where/who to do it to */
  int                  quantity;          /* how many times */
  int                  prestige_success;  /* gained if this task succeeds */
};


/*
 * Basic task types.
 */
#define TASK_UNDEFINED             0
#define TASK_DELIVERY              1
#define TASK_PICKUP                2
#define TASK_DELIVERY_UNIQUE       3
#define TASK_PICKUP_UNIQUE         4
#define TASK_KILL                  5
#define TASK_SKILL_MOB             6
#define TASK_SKILL_MOB_UNIQUE      7
#define TASK_SKILL_OBJ             8
#define TASK_SKILL_OBJ_UNIQUE      9
#define TASK_ROOM                 10
#define TASK_SKILL_SECTOR         11
#define TASK_SKILL_SECTOR_UNIQUE  12

