// PitParser.h - PitParser Library for Samsung PIT files.
//
// Copyright (C) 2016 - Fergus Morrow (fergus@fergus.london)
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef PitParser_h
#define PitParser_h

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>


// struct pit_header
//  This structure contains is the overall header for the PIT 
//   file. Information on specific properties is hard to come
//   by - hence the 8 unknown unsigned integers. However it
//   appears that the first property, head, consistently has
//   the same value - so may be used internally for seeking on
//   the handset.
//
//  The only noteworthy property (with current knowledge) is
//   the entryCount field, which contains the number of entries
//   contained in the file overall.
//
// Type Alias: pit_header
// Length: 28 bytes (224 bits)
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


// struct pit_entry
//  This structure contains all available information pertaining
//   to an individual item in the table - i.e a partition. This
//   data includes the size, whether it's read/writable, a label
//   as well as the image file it should be flashed with. 
//
// Type Alias: pit_entry
// Length: 96 bytes (768 bits)
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


// pit_header* pit_read_file(char *pathname)
//  Read the PIT file - this is done by mapping the entire file
//   to a region of memory, and pointing a header struct to the
//   mapped region.
pit_header* pit_read_file(char *pathname);


// pit_entry* pit_get_first_entry(pit_header *hdr)
//  Retrieve the first entry of the PIT file, this is a very
//   simple function which returns a pointer (of pit_entry
//   type) that's offset to the immediate region after the 
//   header.
pit_entry* pit_get_first_entry(pit_header *hdr);


// pit_entry* pit_entry_for_idx(int idx, pit_header *hdr)
//  Retrieve the entry for a numeric index, this -should not-
//   be used for iterating through the entries; that's better
//   achieved by using array notation on the initial pit_entry
//   pointer (see pit_get_first_entry).
pit_entry* pit_entry_for_idx(int idx, pit_header *hdr);


// pit_entry* pit_entry_for_partition_identifier(uint32_t ident, pit_header *hdr)
//  Retrieves the PIT entry for a given partition identifier,
//   the identifier is a 32bit unsigned integer.
pit_entry* pit_entry_for_partition_identifier(uint32_t ident, pit_header *hdr);

#endif
