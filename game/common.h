#include "typedefines.h"

#define MIN(x, y)		((x)<(y)? (x):(y))
#define MAX(x, y)		((x)>(y)? (x):(y))
#define ABS(x)			((x)>0? (x) : -(x))
#define ABSMIN(x, y)	(ABS(x)<ABS(y) ? (x) : (y))
#define ABSMAX(x, y)	(ABS(x)>ABS(y) ? (x) : (y))

bool InterSection(RECT &rect1, RECT &rect2);
bool Overrange(RECT &rect1, RECT &rect2);
short RandomValue(void);
bool RandomValueBinary(void);
bool RandomValueBinaryWithProbability(int prob);
void print_string_argb(char *buf, int stride, char *str);
void print_string_byte(char *buf, int stride, char *str);
void print_string_16bit(char *buf, int stride, char *str);
int Distance2(RECT &rect1, RECT &rect2);
int Distance2(GPOINT &pos1, GPOINT &pos2);
void LimitWithinBoundary(int &x, int &y, RECT &bound);
int root2(int x);
int VectorDirection(GPOINT &pos1, GPOINT &pos2);
RECT border_upon(RECT &rect, int dir);