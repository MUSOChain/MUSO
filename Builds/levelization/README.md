# Levelization

Levelization is the term used to describe efforts to prevent MUSO from
having or creating cyclic dependencies.

MUSO code is organized into directories under `src/MUSO` (and
`src/test`) representing modules. The modules are intended to be
organized into "tiers" or "levels" such that a module from one level can
only include code from lower levels. Additionally, a module
in one level should never include code in an `impl` folder of any level
other than it's own.

Unfortunately, over time, enforcement of levelization has been
inconsistent, so the current state of the code doesn't necessarily
reflect these rules. Whenever possible, developers should refactor any
levelization violations they find (by moving files or individual
classes). At the very least, don't make things worse.

The table below summarizes the _desired_ division of modules. The levels
are numbered from the bottom up with the lower level, lower numbered,
more independent modules listed first, and the higher level, higher
numbered modules with more dependencies listed later.

| Level / Tier | Module(s)                                                                                                |
| ------------ | -------------------------------------------------------------------------------------------------------- |
| 01           | MUSO/beast MUSO/unity                                                                                    |
| 02           | MUSO/basics                                                                                              |
| 03           | MUSO/json MUSO/crypto                                                                                    |
| 04           | MUSO/protocol                                                                                            |
| 05           | MUSO/core MUSO/conditions MUSO/consensus MUSO/resource MUSO/server                                       |
| 06           | MUSO/peerfinder MUSO/ledger MUSO/nodestore MUSO/net                                                      |
| 07           | MUSO/shamap MUSO/overlay                                                                                 |
| 08           | MUSO/app                                                                                                 |
| 09           | MUSO/rpc                                                                                                 |
| 10           | test/jtx test/beast test/csf                                                                             |
| 11           | test/unit_test                                                                                           |
| 12           | test/crypto test/conditions test/json test/resource test/shamap test/peerfinder test/basics test/overlay |
| 13           | test                                                                                                     |
| 14           | test/net test/protocol test/ledger test/consensus test/core test/server test/nodestore                   |
| 15           | test/rpc test/app                                                                                        |

(Note that `test` levelization is _much_ less important and _much_ less
strictly enforced than `MUSO` levelization, other than the requirement
that `test` code should _never_ be included in `MUSO` code.)

## Validation

The [levelization.sh](levelization.sh) script takes no parameters,
reads no environment variables, and can be run from any directory,
as long as it is in the expected location in the MUSO repo.
It can be run at any time from within a checked out repo, and will
do an analysis of all the `#include`s in
the MUSO source. The only caveat is that it runs much slower
under Windows than in Linux. It hasn't yet been tested under MacOS.
It generates many files of [results](results):

- `rawincludes.txt`: The raw dump of the `#includes`
- `paths.txt`: A second dump grouping the source module
  to the destination module, deduped, and with frequency counts.
- `includes/`: A directory where each file represents a module and
  contains a list of modules and counts that the module _includes_.
- `includedby/`: Similar to `includes/`, but the other way around. Each
  file represents a module and contains a list of modules and counts
  that _include_ the module.
- [`loops.txt`](results/loops.txt): A list of direct loops detected
  between modules as they actually exist, as opposed to how they are
  desired as described above. In a perfect repo, this file will be
  empty.
  This file is committed to the repo, and is used by the [levelization
  Github workflow](../../.github/workflows/levelization.yml) to validate
  that nothing changed.
- [`ordering.txt`](results/ordering.txt): A list showing relationships
  between modules where there are no loops as they actually exist, as
  opposed to how they are desired as described above.
  This file is committed to the repo, and is used by the [levelization
  Github workflow](../../.github/workflows/levelization.yml) to validate
  that nothing changed.
- [`levelization.yml`](../../.github/workflows/levelization.yml)
  Github Actions workflow to test that levelization loops haven't
  changed. Unfortunately, if changes are detected, it can't tell if
  they are improvements or not, so if you have resolved any issues or
  done anything else to improve levelization, run `levelization.sh`,
  and commit the updated results.

The `loops.txt` and `ordering.txt` files relate the modules
using comparison signs, which indicate the number of times each
module is included in the other.

- `A > B` means that A should probably be at a higher level than B,
  because B is included in A significantly more than A is included in B.
  These results can be included in both `loops.txt` and `ordering.txt`.
  Because `ordering.txt`only includes relationships where B is not
  included in A at all, it will only include these types of results.
- `A ~= B` means that A and B are included in each other a different
  number of times, but the values are so close that the script can't
  definitively say that one should be above the other. These results
  will only be included in `loops.txt`.
- `A == B` means that A and B include each other the same number of
  times, so the script has no clue which should be higher. These results
  will only be included in `loops.txt`.

The committed files hide the detailed values intentionally, to
prevent false alarms and merging issues, and because it's easy to
get those details locally.

1. Run `levelization.sh`
2. Grep the modules in `paths.txt`.
   - For example, if a cycle is found `A ~= B`, simply `grep -w A Builds/levelization/results/paths.txt | grep -w B`
