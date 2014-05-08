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
  BENCH_ROUND_TIME,
  BENCH_RUNNING,

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
static const char *bench_fast_path = "<unset>";
static GrapheneBenchSetupFunc bench_fixture_setup;
static GrapheneBenchTeardownFunc bench_fixture_teardown;
static gpointer bench_fixture;
static GHashTable *bench_units;
static double bench_factor;
static int bench_output;

static gboolean bench_verbose = FALSE;
static int bench_warm_up_runs = 50;
static int bench_round_runs = 5;
static int bench_duration = 10;
static char *bench_format = NULL;

static GOptionEntry bench_options[] = {
  { "verbose", 'v', 0, G_OPTION_ARG_NONE, &bench_verbose, "Print extra information", NULL },
  { "warm-up-runs", 0, 0, G_OPTION_ARG_INT, &bench_warm_up_runs, "Number of warm up cycles", "ITERATIONS" },
  { "estimate-runs", 0, 0, G_OPTION_ARG_INT, &bench_round_runs, "Number of estimation cycles", "ITERATIONS" },
  { "duration", 'd', 0, G_OPTION_ARG_INT, &bench_duration, "Benchmark duration", "SECONDS" },
  { "output-format", 'f', 0, G_OPTION_ARG_STRING, &bench_format, "Format of the output", "FORMAT" },

  { NULL, }
};

void
graphene_bench_init (int    *argc_p,
                     char ***argv_p,
                     ...)
{
  GOptionContext *context;
  const char *opt;
  va_list args;
  char **argv = argv_p != NULL ? *argv_p : NULL;
  int argc = argc_p != NULL ? *argc_p : 0;

  if (argc != 0)
    bench_argv0 = argv[0];

  va_start (args, argv_p);
  opt = va_arg (args, const char *);
  while (opt != NULL && *opt != '\0')
    {
      if (g_strcmp0 (opt, GRAPHENE_BENCH_OPT_IMPLEMENTATION) == 0)
        bench_fast_path = va_arg (args, const char *);

      opt = va_arg (args, const char *);
    }

  context = g_option_context_new ("Graphene benchmark options");
  g_option_context_add_main_entries (context, bench_options, NULL);
  g_option_context_set_help_enabled (context, TRUE);
  g_option_context_set_ignore_unknown_options (context, TRUE);
  g_option_context_parse (context, argc_p, argv_p, NULL);

  if (bench_format == NULL)
    bench_format = "csv";

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
graphene_bench_warm_up (const char *path,
                        GrapheneBenchFunc func)
{
  int i;

  if (bench_verbose)
    g_print ("# warm up bench '[%s]:%s' (runs:%d)\n",
             bench_fast_path,
             path,
             bench_warm_up_runs);

  for (i = 0; i < bench_warm_up_runs; i += 1)
    func (bench_fixture);
}

#define TARGET_ROUND_TIME 0.004

static gint64
graphene_bench_round_time (const char *path,
                           GrapheneBenchFunc func)
{
  GTimer *timer = g_timer_new ();
  double min_elapsed = 0;
  int i;

  for (i = 0; i < bench_round_runs; i += 1)
    {
      double elapsed;

      g_timer_start (timer);
      func (bench_fixture);
      g_timer_stop (timer);

      elapsed = g_timer_elapsed (timer, NULL);
      if (i == 0)
        min_elapsed = elapsed;
      else
        min_elapsed = MIN (min_elapsed, elapsed);
    }

  g_timer_destroy (timer);

  bench_factor = TARGET_ROUND_TIME / min_elapsed;

  if (bench_verbose)
    g_print ("# estimated '[%s]:%s' (runs:%d): %.4f secs (correction: %.2f)\n",
             bench_fast_path,
             path,
             bench_round_runs,
             min_elapsed,
             bench_factor);

  return (bench_duration / TARGET_ROUND_TIME) + 1;
}

static GArray *
graphene_bench_run_test (gint64 num_rounds,
                         const char *path,
                         GrapheneBenchFunc func)
{
  GArray *res = g_array_sized_new (FALSE, FALSE, sizeof (double), num_rounds);
  GTimer *timer = g_timer_new ();
  double min_elapsed;
  int i;

  for (i = 0; i < num_rounds; i += 1)
    {
      double elapsed;

      g_timer_start (timer);
      func (bench_fixture);
      g_timer_stop (timer);

      elapsed = g_timer_elapsed (timer, NULL);

      if (i == 0)
        min_elapsed = elapsed;
      else
        min_elapsed = MIN (min_elapsed, elapsed);

      g_array_insert_val (res, i, min_elapsed);
    }

  g_timer_destroy (timer);

  if (bench_verbose)
    g_print ("# '[%s]:%s': %.4f secs after %" G_GINT64_FORMAT " rounds\n",
             bench_fast_path,
             path,
             min_elapsed,
             num_rounds);

  return res;
}

static int
result_sort (gconstpointer p1,
             gconstpointer p2)
{
  const double *d1 = p1;
  const double *d2 = p2;

  if (*d1 < *d2)
    return -1;

  if (*d1 > *d2)
    return 1;

  return 0;
}

static void
graphene_bench_print_results (const char *path,
                              GArray     *results)
{
  double avg;
  int i;

  /* skip the first and last results */
  for (i = 1; i < results->len - 2; i += 1)
    avg += g_array_index (results, double, i);

  avg /= (double) (results->len - 2);

  switch (bench_output)
    {
    case BENCH_FORMAT_CSV:
      /* path, impl, min, max, avg */
      g_print ("%s,%s,%.6f,%.6f,%.6f\n",
               path,
               bench_fast_path,
               g_array_index (results, double, 0),
               g_array_index (results, double, results->len - 1),
               avg);
      break;

    case BENCH_FORMAT_JSON:
      g_print ("{ \"%s\": { \"implementation\": \"%s\", \"times\": [ %.6f, %.6f, %.6f ] } }\n",
               path,
               bench_fast_path,
               g_array_index (results, double, 0),
               g_array_index (results, double, results->len - 1),
               avg);
      break;

    default:
      break;
    }
}

int
graphene_bench_run (void)
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
          gint64 num_rounds;
          GArray *res;

          bench_state = BENCH_WARM_UP;
          graphene_bench_warm_up (path, func);

          bench_state = BENCH_ROUND_TIME;
          num_rounds = graphene_bench_round_time (path, func);

          bench_state = BENCH_RUNNING;
          res = graphene_bench_run_test (num_rounds, path, func);

          /* sort the results */
          g_array_sort (res, result_sort);
          graphene_bench_print_results (path, res);
          g_array_unref (res);
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

double
graphene_bench_get_factor (void)
{
  if (bench_state == BENCH_WARM_UP || bench_state == BENCH_ROUND_TIME)
    return 1.0;

  return bench_factor;
}
