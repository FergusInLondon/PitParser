# PitParser

A pure C library for the processing of PIT files, as found when rooting Samsung Galaxy handsets.

This was based heavily on the previous work of [libpit](https://github.com/Benjamin-Dobell/Heimdall/tree/master/libpit), which @Benjamin-Dobell wrote in C++ for use in (the excellent) [Heimdall](https://github.com/Benjamin-Dobell/Heimdall). For most uses (read *nearly any use*) you're better off using that (unless you're limited to C only) as it's more feature rich, is cross platform and has seen extensive use.

**This was largely something for me to kill time with one evening.**

## About

PIT, or Partition Information Table, files contain information about the layout of partitions on a handset.

These files have a very simple structure, beginning with a header region (of approx # bytes) followed by several individual entry structures.

## API

The best form of documentation is arguably [PitParser.h](lib/PitParser.h), combined with the example application.

### Structures

There are two different data structures contained in a PIT file, these are:

#### `struct pit_header`
>This structure contains is the overall header for the PIT file. Information on specific properties is hard to come by - hence the 8 unsigned integers of unknown value.
>
>However it appears that the first property, head, consistently has the same value - so may be used internally for seeking on the handset.
>
>  The only noteworthy property (with current knowledge) is the entryCount field, which contains the number of entries contained in the file overall.

**Type Alias:** `pit_header` <br />
**Length:** 28 bytes (224 bits)

    typedef struct pit_header {
        uint32_t head;          // 0x00
        uint32_t entryCount;    // 0x04
        uint32_t unknown1;      // 0x08
        uint32_t unknown2;      // 0x0C
    
        uint16_t unknown3;      // 0x10
        uint16_t unknown4;      // 0x12
        uint16_t unknown5;      // 0x14
        uint16_t unknown6;      // 0x16
        uint16_t unknown7;      // 0x18
        uint16_t unknown8;      // 0x1C
    }  __attribute__((packed)) pit_header;


#### `struct pit_entry`
> This structure contains all available information pertaining to an individual item in the table - i.e a partition. This data includes the size, whether it's read/writable, a label as well as the name of the image file it should be flashed with.

**Type Alias:** pit_entry <br />
**Length:** 96 bytes (768 bits)

    typedef struct pit_entry {
        uint32_t binaryType;            // 0x00
        uint32_t deviceType;            // 0x04
        uint32_t identifier;            // 0x08
        uint32_t attributes;            // 0x0C
        uint32_t updateAttributes;      // 0x10
        uint32_t blockSizeOrOffset;     // 0x14
        uint32_t blockCount;            // 0x18
        uint32_t fileOffset;            // 0x1C
        uint32_t fileSize;              // 0x20
        
        char partitionName[32];         // 0x24
        char flashFileName[32];         // 0x44
        char fotaFileName[32];          // 0x64
    }  __attribute__((packed)) pit_entry;

### Functions

Unless otherwise stated, all functions return `-1` upon error.

#### `pit_header* pit_read_file(char *pathname)`
> Read the PIT file - this is done by mapping the entire file to a region of memory, and pointing a header struct to the mapped region.

#### `pit_entry* pit_get_first_entry(pit_header *hdr)`
> Retrieve the first entry of the PIT file, this is a very simple function which returns a pointer (of pit_entry type) that's offset to the immediate region after the header.

#### `pit_entry* pit_entry_for_idx(int idx, pit_header *hdr)`
> Retrieve the entry for a numeric index, this -should not- be used for iterating through the entries; that's better achieved by using array notation on the initial pit_entry pointer (see pit_get_first_entry).


#### `pit_entry* pit_entry_for_partition_identifier(uint32_t ident, pit_header *hdr)`
> Retrieves the PIT entry for a given partition identifier, the identifier is a 32bit unsigned integer.


## Example

There's an [example utility](example.c) in this repository. If you wish to run this utility then simply use the shell script to automatically compile and link the library.

    ▶ ./make-example.sh
    
    ▶ ./example
    Usage: ./example <filename.pit>

    ▶ ./example ../SamsungS6PITLatest.pit
    Number of entries: 22
    
    01 - BOOTLOADER
    	Partition Identifier: 80
	    Device Type: 8
    	Block Count: 1024
    	Attributes: 2
    
    
    02 - CM
    	Partition Identifier: 90
    	Device Type: 8
    	Block Count: 1024
    	Attributes: 2

    [...]

## Testing

This has only been tested on PIT files taken from a 32GB Samsung Galaxy S6 (SM-G920F). It *should* work reading PIT files from other devices, but without devices to test it's hard to confirm that with any certainty.

## Licensing & Disclaimer 

This source code is available under the GNU General Public License, please take specific notice of the following disclaimer:

> This program is distributed in the hope that it will be useful, but WITHOUT
> ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
> FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
> more details.

If you modify your PIT file and attempt to do something clever with it... and you brick your phone, I'm not responsible. If you manage to wangle this source code in to anything mildly interesting however, please drop me a line (@FergusInLondon) so I can take a look - I'd be very interested! :)

## Work In Progress

This is still a work in progress, albeit without any real goal in mind. However, there are still a couple of tasks I would like to complete:

- [ ] Deal with endian-ness
- [ ] Write some helper functions for gaining usable information (i.e size in units of `x`) from the values
- [ ] Allow modifications and the ability to write a modified PIT file

These are mostly trivial tasks; requiring a mixture of pre-processor trickery (for the endian-ness; notably using platform specific methods (`OSSwapInt*` etc)) and some changes to the file opening and mapping (for the ability to write modified files).
