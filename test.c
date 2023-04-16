#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int pstrcmp( const void* a, const void* b )
{
  return strcmp( *(const char**)a, *(const char**)b );
}

int main()
{
  const char* xs[] =
  {
    "nyancat_007.tga",
    "nyancat_008.tga",
    "nyancat_009.tga",
    "nyancat_012.tga",
    "nyancat_006.tga",
    "nyancat_002.tga",
    "nyancat_003.tga",
    "nyancat_004.tga",
    "nyancat_005.tga",
    "nyancat_010.tga",
    "nyancat_000.tga",
    "nyancat_001.tga",
    "nyancat_011.tga",
    "nyancat_007.tga",
    "nyancat_008.tga",
    "nyancat_009.tga",
  };
  const size_t N = sizeof(xs) / sizeof(xs[0]);

  puts( "(unsorted)" );
  for (int n = 0; n < N; n++)
    puts( xs[ n ] );

  // Do the thing!
  qsort( xs, N, sizeof(xs[0]), pstrcmp );

  puts( "\n(sorted)" );
  for (int n = 0; n < N; n++)
    puts( xs[ n ] );
}
