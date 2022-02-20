The symbol sets to be edited are stored in the src subdirectory in the verbose
.xmap format. The symbol sets to be distributed are stored in the more compact
.omap format in one subdirectory per scale. The test subdirectory builds (under
CMake) an executable (test) `symbol_set_t` which updates compressed files from
a list of known files in the src directory, thus automating the following steps.

Symbol set for different scales for ISOM and ISSprOM are proportional enlargements of 
the basic scales which are 1:15000 for ISOM and 1:4000 for ISSprOM.

For ISMTBOM the scaling is more nuanced. While 1:15000 is the basic scale, the 
enlargement for symbol set and course setting symbols is 1.2 for 1:12500 and 
1.5 for 1:10000, 1:7500 and 1:5000.[3]

For ISSkiOM the scaling is also more nuanced. 1:15000 is the basic scale for symbol
set (SS) and course setting (CS). For 1:12500 the factors are 1.0 (SS) and 1.2 (CS).
For 1:10000 the factors are 1.0 (SS) and 1.5 (CS). For 1:7500 the factors are 1.33 (SS) and 1.5 (CS).
For 1:5000 the factors are 1.5 (SS) and 1.5 (CS). All enlargement factors are relative 
to the basic scale symbol set.[4]

When applying proportional enlargement there may be symbols that shall not be enlarged 
(e.g., the 999 OpenOrienteering Logo). To create scaled copies of the basic
scale sets proceed the following way:

- Select all symbols to be enlarged uniformly. Right click -> Scale (choose enlargement percentage, e.g, 1.5 = 150%).
- Repeat for other symbols to be enlarged by another factor.
- Do not (!!) enlarge the 999 OpenOrienteering Logo.
- Change the scale of the symbol set: Map -> Change Map scale **unchecking the Scale symbol sizes checkbox
**.
- Save the file following the established naming convention.

[1] ISOM 2019-2 https://1drv.ms/b/s!As4LDA11gDVmgt8CiJdF8MdoLshsLg
[2] ISSprOM 2019-2 https://1drv.ms/b/s!As4LDA11gDVmgvJrcMQn6mK9JsdRgA
[3] ISMTBOM 2022 https://1drv.ms/b/s!As4LDA11gDVmgvJnTyKq5MzVljdQMw
[4] ISSkiOM 2019 https://1drv.ms/b/s!As4LDA11gDVmgt9HH965LY27OyWsfg
