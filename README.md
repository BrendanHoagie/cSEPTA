# cSEPTA

## Created By
Brendan Hoag <realbrendanhoag@gmail.com>

## Version 1.0

## About
This hosts the code to to track live (or close to live) positions of all SEPTA metro vehicles + PATCO. The eventual goal is to create a circuit board stylized to look like the SEPTA metro map, to be displayed as wall art.

## Notes

### General
- No logical association between order in the station bits, they are in numerical order by station ID, which is a bus route spec, so adjacent stations in the uint64_t are not necessarily adjacent on the route

### T Trolley Lines
- Some (but not all) stations have two IDs. 19th st, for example, has 20646 & 20660, one for eastbound & one for westbound. Easiest way to deal with this might be to manually find them all (`T_stations_unique.txt`) and write rules in the display to crush them to one station? Seems easier than doing it on the data processing side, at least

### D Trolley Lines
- D1, D2
- same duplication problem

### M Trolley Lines
- M1
-same duplication problem



## External Libraries
This project relies on 2 external libraries
- [cURL](https://curl.se/)
- [cJSON](https://github.com/DaveGamble/cJSON)