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

#include "PitParser.h"

pit_header* pit_read_file(char *pathname) {
    
    // Attempt to open file; handle common errors.
    int fd = open(pathname, O_RDONLY);
    if (-1 == fd) {
        // Take care of any potential errors here, despite the verbosity this actually
        //  pre-empts most of the errors that can occur with fstat() and mmap() too.
        switch (errno) {
            case EBADF:
            case EACCES:
            case ENOENT:
                printf("Unable to open file: permission denied or file does not exist!\n");
                break;
                
            case ENOTDIR:
                printf("Unable to open file: Invalid path.\n");
                break;
                
            case ELOOP:
                printf("Unable to open file: is it a symbolic link?\n");
                break;
                
            case EISDIR:
                printf("Unable to open file: supplied path is a directory.\n");
                break;
                
            case EOVERFLOW:
                printf("Unable to open file: invalid file (too large)\n");
                break;
                
            case ENAMETOOLONG:
                printf("Unable to open file: supplied path is too long.");
                break;
                
            case EAGAIN:
            case EFAULT:
            case EINTR:
            case EINVAL:
            case EMFILE:
            case ENFILE:
            case ETXTBSY:
            case EROFS:
            case ENXIO:
            case EOPNOTSUPP:
                printf("Unable to open file: something has gone wrong.. something which makes very little sense. (%i)", errno);
                break;
                
            default:
                printf("Unable to open file: an unknown error has occured. (%i)", errno);
                break;
        }
        
        return (pit_header*)-1;
    }
    
    // Retrieve file statistics for PIT file.
    struct stat buf;
    if (fstat(fd, &buf) < 0) {
        // Nothing should actually go wrong here, as by virtue of successfully
        //  pening the file above we should already have encountered any issues.
        printf("Error occurred whilst trying to retrieve file statistics! (%i)", errno);
        return (pit_header*)-1;
    }
    
    // Map contents of PIT file to memory, and point a PIT* at it.
    pit_header *hdr = mmap( (caddr_t)0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0 );
    if (hdr == MAP_FAILED) {
        switch (errno) {
            case EINVAL:
                printf("Unable to read file: is the file empty?\n");
                break;
            default:
                printf("Error mapping PIT file (%i).\n", errno);
                break;
        }
        
        return (pit_header*)-1;
    }
    
    return hdr;
}

pit_entry* pit_get_first_entry(pit_header *hdr) {
    return (pit_entry*)(hdr+1);
}

pit_entry* pit_entry_for_idx(int idx, pit_header *hdr) {
    if (idx > hdr->entryCount) {
        return (pit_entry*)-1;
    }
    
    pit_entry *e = pit_get_first_entry(hdr);
    return &e[idx];
}

pit_entry* pit_entry_for_partition_identifier (uint32_t ident, pit_header *hdr) {
    pit_entry *e = pit_get_first_entry(hdr);
    
    for (int i = 0; i<hdr->entryCount; i++, e++) {
        if(ident == e->identifier) {
            return e;
        }
    }
    
    return (pit_entry*)-1;
}