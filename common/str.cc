#include "furry/common/str.h"

#include <algorithm>
#include <cstring>

using namespace std;

namespace furry
{

// This function currently creates two additional copies of the input
std::vector<std::string>
Split(const std::string & str, const std::string & delimiters)
{
  std::vector<std::string> results;
  char * piece;
  const char * d = delimiters.c_str();
  int length = str.size();
  char * c_str = new char[length+1];
  c_str[str.size()] = '\0';
  std::strncpy(c_str, str.c_str(), length);

  piece = std::strtok(c_str, d);
  while (piece != NULL)
  {
    results.push_back(piece);
    piece = std::strtok(NULL, d);
  }

  delete [] c_str;

  return results;

}

std::string
tolower(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

void StringAppendV(string* dst, const char* format, va_list ap) {
  // First try with a small fixed size buffer
  char space[1024];

  // It's possible for methods that use a va_list to invalidate
  // the data in it upon use.  The fix is to make a copy
  // of the structure before using it and use that copy instead.
  va_list backup_ap;
  va_copy(backup_ap, ap);
  int result = vsnprintf(space, sizeof(space), format, backup_ap);
  va_end(backup_ap);

  if ((result >= 0) && (result < sizeof(space))) {
    // It fit
    dst->append(space, result);
    return;
  }

  // Repeatedly increase buffer size until it fits
  int length = sizeof(space);
  while (true) {
    if (result < 0) {
      // Older behavior: just try doubling the buffer size
      length *= 2;
    } else {
      // We need exactly "result+1" characters
      length = result+1;
    }
    char* buf = new char[length];

    // Restore the va_list before we use it again
    va_copy(backup_ap, ap);
    result = vsnprintf(buf, length, format, backup_ap);
    va_end(backup_ap);

    if ((result >= 0) && (result < length)) {
      // It fit
      dst->append(buf, result);
      delete[] buf;
      return;
    }
    delete[] buf;
  }
}

string StringPrintf(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  string result;
  StringAppendV(&result, format, ap);
  va_end(ap);
  return result;
}

} // furry
