CheckSDL
========

A tool evaluates security configurations of a given file based on Microsoft Security Development Lifecycle (SDL). 

This tool can be used for both defensive and offensive purposes since it can unveil PE files compiled with insecure configurations.


Usage
-----------------
    >CheckSDL.exe target_path [csv_out_path]

    target_path: A path of either a file or a directory to evaluate.
    csv_out_path: A path of a CSV file to save the result if needed. 
                  An existing file will be overwritten without warning.
    
In a nutshell, this tool checks if a PE file is:
- Compiled by a reasonably new version of compiler.
- Using /GS Stack buffer overrun detection.
- Using either /SafeSEH exception handling protection or IMAGE_DLLCHARACTERISTICS_NO_SEH DllCharacteristics.
- Using Data Execution Prevention (DEP) / No eXecute (NX).
- Using Address space layout randomization (ASLR).
- Using Heap corruption detection.
- NOT containing any shared sections.
- NOT using any of banned functions such as buffer-overrun prone functions.

These criteria are defined and explained in [Windows ISV Software Security Defenses](http://msdn.microsoft.com/en-us/library/bb430720.aspx) and the pages linked by this page. 

There are some differences between this tool and SDL. For example, this tool defines severity for each check point either Critical or High for simplicity, while SDL prioritized them as either Critical, High or Moderate, but most things are conforming to SDL. Other caveats are listed on a Note section.


Sample Output
-----------------
Let's see some sample outputs. Here is a result of analysis for a system32 directory on Windows XP SP3.

    ...
    #ofCV #ofHV (Built) NewComp (Ver) GS     SEHP   DEP    ASLR   HeapP  #ofSS #ofBF1 #ofBF2  (Path)	
    ...
    5     3     2001/ 8 false   7     false  false  false  false  false  0     10    3	      c:/WINDOWS/system32/Restore/srdiag.exe	
    ...
    0     0     2012/ 6 true    9     true   true   true   true   N/A    0     0     0       c:/WINDOWS/system32/wups2.dll	
    3.61  1.68          94.19%        32.15% 44.90%	95.14% 95.05% 99.05% 0.00  3.53  1.64	

The First line is a header. Meaning of each column is as follows:
- #ofCV: A number of Critical severity violations.
- #ofHV: A number of High severity violations.
- (Built): A date the file was compiled.
- NewComp: New Compiler, namely, if it was compiled by a reasonably new version of a compiler. If not, it is a Critical violation.
- (Ver): A version of a compiler used to compiled the file.
- GS: If it is using /GS Stack buffer overrun detection. If not, it is a Critical violation.
- SEHP: SEH Protection, namely, if it is using either /SafeSEH exception handling protection or IMAGE_DLLCHARACTERISTICS_NO_SEH DllCharacteristics. If neither, it is a Critical violation.
- DEP: If it is using Data Execution Prevention (DEP) / No eXecute (NX). If not, it is a Critical violation.
- ASLR: If it is using Address space layout randomization (ASLR). If not, it is a Critical violation.
- HeapP: Heap Protection, namely, if it is using Heap corruption detection. If not, it is a High violation.
- #ofSS: A number of shared sections. If exists, it is a High violation.
- #ofBF1: A number of imported banned functions categorized as Require. If exists, it is a High violation.  
- #ofBF2: A number of imported banned functions categorized as Recommended. If exists, it is a High violation.
- (Path): A path of the file.	

Columns with parentheses are additional information, and files are sorted by their violation levels (#ofCV and #ofHV0). 

The last line is statistics of the result. Each column means:
- An average number of Critical severity violations.
- An average number of High severity violations.
- Proportion of violations of NewComp.
- Proportion of violations of GS.
- Proportion of violations of SEHP.
- Proportion of violations of DEP.
- Proportion of violations of ASLR.
- Proportion of violations of HeapP.
- An average number of shared sections.
- An average number of imported banned functions categorized as Require.  
- An average number of imported banned functions categorized as Recommended.  

For all of them, the lower number, the better. So let's compare results for system32 directories on both Windows XP SP3 and 7 SP1. 

    #ofCV #ofHV (Built) NewComp (Ver) GS     SEHP   DEP    ASLR   HeapP  #ofSS #ofBF1 #ofBF2  (Path)	
    3.61  1.68          94.19%        32.15% 44.90%	95.14% 95.05% 99.05% 0.00  3.53   1.64	  XP SP3
    0.09  1.33           0.64%         3.02%  0.87%  2.55%  2.32% 33.12% 0.00  0.87   1.13    7 SP1

![chart](/img/chart.png)

As you can see, there are significant differences in #ofCV and many of other criteria. By using this tool, you will be able to measure security levels of a file, or the product package or detect a component that could be exploited and should be improved.


Note
-----------------
- A 64 bit file is not analyzed.
- Only .exe, .dll and .ocx are analyzed.
- A kernel mode file with the above extensions will be analyzed incorrectly since analysis is done in the same manner as a user mode file. 
- Analysis of imported banned functions are not as accurate as a result of source code based analysis since these functions may be either inlined (the tool causes false negatives) or emitted by some of StrSafe functions (the tool causes false positives).
- When a file is Managed PE file, some configurations are treated as enabled regardless of actual value in a PE header; these are GS, DEP, ASLR and Heap Protection.
- Heap Protection is only evaluated when a file is an executable file according to the SDL criteria.


Supported Platforms
-----------------
- Windows XP SP3 and later (both x86 and x64)


License
-----------------
This software is released under the MIT License, see LICENSE.
  
