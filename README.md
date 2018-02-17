### Sorting string with operators and elements

Operators: '(' , ')' and '+'

Elements: symbols

Sorting by alphabetically, for example:

Input: (X + (V+M+A) + C +(M+L + (C + C) + C)) (1)

After sort: ((A+M+V)+C+(C + (C+C) + L + M)+X)

1. build.bat -- build Debug | Release Win32 (MS VisualStudio 2008)
2. app_generate.exe [numbers_of_string] [path_to_file] -- generate random string (1)
3. app_sort.exe [path_to_file] -- sorting string from file txt.Result will been written to file with suffix '_sorted' to the input name
4. exports.dll -- sort algorithm implementation
