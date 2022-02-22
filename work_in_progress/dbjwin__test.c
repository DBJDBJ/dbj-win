/// Ц++
/// https : //gist.github.com/bkaradzic/2e39896bc7d8c34e042b#gistcomment-4034204
/// (c) 2022 by dbj at dbj dot org -- https://dbj.org/license_dbj

#include "dbjwin_log_file.h"
#include "dbjwin_string.h"
#include "dbjwin_strsafe.h"

#include "dbjwin_ctype.h"

int main(const int argc, const char* argv[]) {
  DBJWIN_LOG("%s", "This goes to the log file");
  DBJWIN_LOG("%s", "And this too");

  dbjwin_ctype_test(argc, argv);

  DBJWIN_VERIFY(1==2); // kick the bucket

  return 42;
}
