#ifndef __GRAPHENE_BENCH_UTILS_H__
#define __GRAPHENE_BENCH_UTILS_H__

#include <glib.h>
#include <graphene.h>

G_BEGIN_DECLS

#define GRAPHENE_BENCH_OPT_IMPLEMENTATION       "graphene-bench-impl"

typedef gpointer (* GrapheneBenchSetupFunc) (void);
typedef void (* GrapheneBenchFunc) (gpointer fixture);
typedef void (* GrapheneBenchTeardownFunc) (gpointer fixture);

void    graphene_bench_init                     (int    *argc,
                                                 char ***argv,
                                                 ...) G_GNUC_NULL_TERMINATED;
void    graphene_bench_set_fixture_setup        (GrapheneBenchSetupFunc     func);
void    graphene_bench_set_fixture_teardown     (GrapheneBenchTeardownFunc  func);
void    graphene_bench_add_func                 (const char                *path,
                                                 GrapheneBenchFunc          func);
int     graphene_bench_run                      (void);

double  graphene_bench_get_factor               (void);

G_END_DECLS

#endif /* __GRAPHENE_BENCH_UTILS_H__ */
