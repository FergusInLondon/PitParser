// PitParser.h - PitParser Example Utility
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

#include "lib/PitParser.h"

// print_pit_entry(int idx, pit_entry *e)
//  Prints out some data from a pit_entry struct.
void print_pit_entry(int idx, pit_entry *e);

// print_pit_header(pit_header *p)
//  Prints out some dara from a pit_header struct.
void print_pit_header(pit_header *p);

int main(int argc, char **argv){
    if (argc < 2) {
        printf("Usage: %s <filename.pit>\n", argv[0]);
        return -1;
    }
    
    pit_header *hdr = pit_read_file(argv[1]);
    if (hdr == 0) {
        printf("Failed to read PIT file.\n");
        return -1;
    }

    print_pit_header(hdr);

    pit_entry *entry = pit_get_first_entry(hdr);
    for (int i=0; i < hdr->entryCount; i++) {
        print_pit_entry(i, &entry[i]);
    }
    
    return 0;
}

void print_pit_header(pit_header *p){
    printf("Number of entries: %u\n\n", p->entryCount);
}

void print_pit_entry(int idx, pit_entry *e){
    printf("%02d - %s\n\t", ++idx, e->partitionName);
    printf("Partition Identifier: %u\n\t", e->identifier);
    printf("Device Type: %u\n\t", e->deviceType);
    printf("Block Count: %u\n\t", e->blockCount);
    printf("Attributes: %u\n\n\n", e->attributes);
}