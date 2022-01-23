# 1. Run programs
## 1.1. Generate files to be sorted

Each line of the file uses this format:
```sh
line_number|date_of_week month day HH:MM:SS YYYY
```
For example,
```sh
1|Wed Jan  4 06:52:57 2012
2|Tue Feb 11 08:14:43 2020
3|Wed Dec 15 04:09:13 2010
4|Tue Jan 18 17:42:37 2011
5|Wed Jul 22 11:04:17 2009
6|Thu Dec 20 08:21:18 2007
7|Fri Apr  6 02:21:36 2018
8|Thu Nov 11 05:22:03 2010
9|Wed Jun 23 18:35:17 2004
10|Fri Oct  6 15:44:18 2017
```
To run the program,
```sh
cd MergeSortLargeFile/generate_file
./generate_file ["10"|"100"|"10K"|"1M"|"100M"]
```
The command line argument indicates the number of lines in the generated file.

The generated file is saved to MergeSortLargeFile/data/input.

## 1.2. Sort files
The sorting steps are:

1. Locate the file to be sorted from MergeSortLargeFile/data/input.
2. Load the file by chunks so that the size of each chunk is less than the available application memory.
3. Sort each chuck in ascending order and write the result to temporary file in MergeSortLargeFile/data/temp.
4. Load one line at a time from the sorted temporary file into priority queue and pop up the top element into one final sorted file.

To run the program,
```sh
cd MergeSortLargeFile/sort_file
./sort_file ["10"|"100"|"10K"|"1M"|"100M"]
```
The command line argument indicates the corresponding generated file to sort.

The sorted file is saved to MergeSortLargeFile/data/output.

## 1.3. Test sorting result
The testing will check:

1. The original file and sorted file contains the same number of lines.
2. All lines from the original file are present in the sorted file.
3. All lines in the sorted files appear in ascending order.

To run the program,
```sh
cd MergeSortLargeFile/test_file_sort
./test_file_sort ["10"|"100"|"10K"|"1M"|"100M"]
```
The command line argument indicates the corresponding original and sorted files.

The testing reesult is saved to MergeSortLargeFile/data/report.

# 2. Development notes

## 2.1. Dev environment
The programs are developed on macOS Monterey Version 12.1.

The IDE is VSCode 1.62.3, with C/C++ Extension v1.7.1. The C++ version is C++17.

## 2.2. The FileLine class
The FileLine class is defined to hold three meta data of each line from the file:

1. The line number extracted from the line.
2. The key - number of seconds from the Epoch as represented by the time stamp extracted from the line.
3. The file index - index of the chunck file the line is read from.

The criteria for sorting is:
```js
auto compareFileLine = [](const std::shared_ptr<FileLine>& a, const std::shared_ptr<FileLine>& b) {
    return a->key() != b->key() ? a->key() < b->key() : a->lineNumber() < b->lineNumber();
};
```

## 2.3. K-way merge
To merge the K sorted files, a C++/STL priority_queue is used. Since the priority_queue always puts the smallest line (by the above criteria) on the top, it takes O(1) to output it to the merged file.

New line inserted to the back of the priority_queue will trigger the heapify process to put the newly smallest line on the top again. The heapify takes O(logN).