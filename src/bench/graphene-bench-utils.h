/* graphene-bench-utils.h: Benchmarking suite
 *
 * Copyright © 2014  Emmanuele Bassi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __GRAPHENE_BENCH_UTILS_H__
#define __GRAPHENE_BENCH_UTILS_H__

#include <glib.h>
#include <graphene.h>

G_BEGIN_DECLS

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

void    graphene_bench_set_rounds_per_unit      (int n_rounds);
int     graphene_bench_get_rounds_per_unit      (void);

G_END_DECLS

#endif /* __GRAPHENE_BENCH_UTILS_H__ */
