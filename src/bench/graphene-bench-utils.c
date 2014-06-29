/* graphene-bench-utils.c: Benchmarking suite
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

#include "config.h"

#include <math.h>
#include <float.h>

#include "graphene-bench-utils.h"

enum {
  /* initial state */
  BENCH_START,

  /* fixture set up */
  BENCH_SETUP,

  /* run */
  BENCH_WARM_UP,
  BENCH_RUNNING,
  BENCH_OUTPUT,

  /* fixture teardown */
  BENCH_TEARDOWN,

  /* final state */
  BENCH_STOP
};

enum
{
  BENCH_FORMAT_NONE,
  BENCH_FORMAT_CSV,
  BENCH_FORMAT_JSON
};

static int bench_state;
static int bench_exit_status;
static const char *bench_argv0;
static const char *bench_fast_path;
static GrapheneBenchSetupFunc bench_fixture_setup;
static GrapheneBenchTeardownFunc bench_fixture_teardown;
static gpointer bench_fixture;
static GHashTable *bench_units;
static int bench_unit_rounds = 10000;
static int bench_output;

static gboolean bench_verbose = FALSE;
static int bench_warm_up_runs = 50;
static int bench_runs = 100;
static char *bench_format = NULL;

static GOptionEntry bench_options[] = {
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &bench_verbose, "Print extra information", NULL },
  { "warm-up-runs", 0, 0, G_OPTION_ARG_INT, &bench_warm_up_runs, "Number of warm up cycles", "ITERATIONS" },
  { "bench-runs", 0, 0, G_OPTION_ARG_INT, &bench_runs, "Number of bench cycles", "ITERATIONS" },
  { "output-format", 'f', 0, G_OPTION_ARG_STRING, &bench_format, "Format of the output (csv,json)", "FORMAT" },

  { NULL, }
};

void
graphene_bench_init (int    *argc_p,
                     char ***argv_p,
                     ...)
{
  GOptionContext *context;
  char **argv = argv_p != NULL ? *argv_p : NULL;
  int argc = argc_p != NULL ? *argc_p : 0;
  const char *opt = NULL;
  va_list opts;

  if (argc != 0)
    bench_argv0 = argv[0];

  va_start (opts, argv_p);

  opt = va_arg (opts, const char *);
  while (opt != NULL)
    {
      if (g_strcmp0 (opt, "implementation") == 0)
        bench_fast_path = va_arg (opts, const char *);

      opt = va_arg (opts, const char *);
    }

  va_end (opts);

  context = g_option_context_new ("Graphene benchmark options");
  g_option_context_add_main_entries (context, bench_options, NULL);
  g_option_context_set_help_enabled (context, TRUE);
  g_option_context_set_ignore_unknown_options (context, TRUE);
  g_option_context_parse (context, argc_p, argv_p, NULL);

  if (g_strcmp0 (bench_format, "csv") == 0)
    bench_output = BENCH_FORMAT_CSV;

  if (g_strcmp0 (bench_format, "json") == 0)
    bench_output = BENCH_FORMAT_JSON;
}

void
graphene_bench_set_fixture_setup (GrapheneBenchSetupFunc func)
{
  g_assert (bench_fixture_setup == NULL);
  bench_fixture_setup = func;
}

void
graphene_bench_set_fixture_teardown (GrapheneBenchTeardownFunc func)
{
  g_assert (bench_fixture_teardown == NULL);
  bench_fixture_teardown = func;
}

void
graphene_bench_add_func (const char        *path,
                         GrapheneBenchFunc  func)
{
  if (bench_units == NULL)
    bench_units = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

  g_hash_table_insert (bench_units, g_strdup (path), func);
}

static void
graphene_bench_warm_up (const char *impl,
                        const char *path,
                        GrapheneBenchFunc func,
                        int num_rounds)
{
  int i;

  if (bench_verbose)
    g_printerr ("# warm up bench '[%s]:%s' (runs:%d)\n",
                bench_fast_path,
                path,
                bench_warm_up_runs);

  for (i = 0; i < num_rounds; i += 1)
    func (bench_fixture);
}

static double
graphene_bench_run_test (const char *impl,
                         const char *path,
                         GrapheneBenchFunc func,
                         gint64 num_rounds)
{
  GTimer *timer = g_timer_new ();
  double elapsed;
  int i;

  g_timer_start (timer);

  for (i = 0; i < num_rounds; i += 1)
    func (bench_fixture);

  elapsed = g_timer_elapsed (timer, NULL) * 1000000000.0;
  g_timer_destroy (timer);

  if (bench_verbose)
    g_printerr ("# '[%s]:%s': %.6f usecs/round after %" G_GINT64_FORMAT " rounds\n",
                impl,
                path,
                elapsed,
                num_rounds);

  return elapsed;
}

static double
format_time (double       d,
             const char **unit)
{
  if (d > 1000000000)
    {
      *unit = "s";
      return d / 1000000000;
    }

  if (d > 1000000)
    {
      *unit = "us";
      return d / 1000000;
    }

  if (d > 1000)
    {
      *unit = "ms";
      return d / 1000;
    }

  *unit = "ns";

  return d;
}

static void
graphene_bench_print_results (const char *impl,
                              const char *path,
                              double      elapsed,
                              int         num_rounds)
{
  const char *d_unit, *iter_unit, *item_unit;
  double d = format_time (elapsed, &d_unit);
  double iter = format_time (elapsed / num_rounds, &iter_unit);
  double item = format_time (elapsed / num_rounds / bench_unit_rounds, &item_unit);

  switch (bench_output)
    {
    case BENCH_FORMAT_NONE:
      g_print ("### unit '%s' (using %s implementation) ###\n"
               "Duration: %.3f %s\n"
               "Per iteration: %.6f %s\n"
               "Per item: %.6f %s\n",
               path, impl,
               d, d_unit,
               iter, iter_unit,
               item, item_unit);
      break;

    case BENCH_FORMAT_CSV:
      g_print ("%s,%s,%.6f,%.6f,%.6f\n",
               path, impl,
               elapsed,
               elapsed / num_rounds,
               elapsed / num_rounds / bench_unit_rounds);
      break;

    case BENCH_FORMAT_JSON:
      g_print ("{\"%s\":{\"impl\":\"%s\",\"total\":%.6f,\"iteration\":%.6f,\"round\":%.6f}}\n",
               path, impl,
               elapsed,
               elapsed / num_rounds,
               elapsed / num_rounds / bench_unit_rounds);
      break;

    default:
      break;
    }
}

static int
graphene_bench_round_run (const char *impl)
{
  bench_state = BENCH_START;

  if (bench_fixture_setup != NULL)
    {
      bench_state = BENCH_SETUP;
      bench_fixture = bench_fixture_setup ();
    }

  if (bench_units != NULL)
    {
      GHashTableIter iter;
      gpointer key, value;

      g_hash_table_iter_init (&iter, bench_units);
      while (g_hash_table_iter_next (&iter, &key, &value))
        {
          const char *path = key;
          GrapheneBenchFunc func = value;
          double elapsed;

          bench_state = BENCH_WARM_UP;
          graphene_bench_warm_up (impl, path, func, bench_warm_up_runs);

          bench_state = BENCH_RUNNING;
          elapsed = graphene_bench_run_test (impl, path, func, bench_runs);

          bench_state = BENCH_OUTPUT;
          graphene_bench_print_results (impl, path, elapsed, bench_runs);
        }
    }

  if (bench_fixture_teardown != NULL)
    {
      bench_state = BENCH_TEARDOWN;
      bench_fixture_teardown (bench_fixture);
      bench_fixture = NULL;
    }

  bench_state = BENCH_STOP;

  return bench_exit_status;
}

int
graphene_bench_run (void)
{
  return graphene_bench_round_run (bench_fast_path);
}

void
graphene_bench_set_rounds_per_unit (int n_checks)
{
  bench_unit_rounds = n_checks;
}

int
graphene_bench_get_rounds_per_unit (void)
{
  return MAX (bench_unit_rounds, 1);
}
