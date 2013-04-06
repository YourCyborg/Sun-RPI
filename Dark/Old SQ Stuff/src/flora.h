
/*
 * The amount above desired water a plant must be to increase its
 * health.
 */
#define FLORA_HEALTH_DELTA 100

/*
 * Plant appears simply as a sprout until this age is met.
 */
#define FLORA_RECOGNIZABLE_AGE 10


#define FLORA_TYPE_TREE   0
#define FLORA_TYPE_SHRUB  1
#define FLORA_TYPE_BUSH   2
#define FLORA_TYPE_STEM   3
#define FLORA_TYPE_CACTUS 4
#define FLORA_TYPE_MAX    5

struct flora_class
{
  struct flora_class	*next;
  char			*name;
  int			type;            /* tree, shrub, vine, etc. */
  int			health_flower;   /* Health to produce flowers/fruit */
  int			health_max;      /* Maximum Health */
  int			water_desired;   /* Water to maintain health. */
};


struct flora_species
{
  struct flora_species	*next;
  struct flora_class	*class;
  char			*name;
  int			flower_effort; /* flora updates to produce 1 fruit */
  int			flower_vnum;
  int			flower_max;    /* max flowers that plant can have  */
  int			fruit_vnum;
  int			fruit_max;     /* max fruits that plant can have   */
  int			growth_seed;   /* negative number, 0 = sprouts     */
  int			growth_flowering; /* age plant starts producing seeds */
  int			growth_max;    /* plant begins to die at this age  */
};


struct flora_instance
{
  struct flora_instance	*next;
  struct flora_species	*species;
  int			quantity;
  int			health;
  int			water;      /* plants can be watered individually */
  int			fruit;
  int			flower;
  int			growth;
  int			eiv_fruit;   /* last fruit generation */
};
