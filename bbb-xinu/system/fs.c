#include <xinu.h>
#include <kernel.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


#if FS
#include <fs.h>

static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;


#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock);

/* YOUR CODE GOES HERE */
int fs_write(int fd, void *buf, int nbytes)
{
    int bl,f_bl, f_off, cache_len;
    struct filetable *oftptr;
    int len = nbytes;
    if ((fd < 0) || (fd > (NUM_FD-1))) {
	printf ("Invalid file descriptor!\n");
	return SYSERR;
    }
    if ((buf == NULL) || (nbytes < 1 )) {
	return SYSERR;
    }
    
    oftptr = &oft[fd];
    if (oftptr->state == FSTATE_CLOSED) {
	printf ("File descriptor not open!\n");
	return SYSERR;
    }
    while (len != 0) {
	f_bl = oftptr->fileptr/fsd.blocksz;
	f_off = oftptr->fileptr%fsd.blocksz;
	if (f_off == 0) {
	    if (f_bl!=0) {
		f_bl++;
	    }
	    bl = fs_get_next_free_data_block();
	    oftptr->in.blocks[f_bl] = bl;
	} else {
	    bl = fs_fileblock_to_diskblock(dev0, fd, f_bl);
	}
	bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
	if (f_off + len > fsd.blocksz) {
	    cache_len = fsd.blocksz - f_off;
	    len = len - cache_len;
	} else {
	    cache_len = len;
	    len = 0;
	}
	memcpy(&block_cache[(f_off*sizeof(char))], buf, 
	       fsd.blocksz-f_off);
	bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);
	oftptr->fileptr += cache_len;
    }
}
int fs_read(int fd, void *buf, int nbytes)
{
    struct filetable *oftptr;
    int blocksz;
    int fileblock;
    int offset;
    int diskblock;
    
    
    if ((fd < 0) || (fd > (NUM_FD-1))) {
	printf ("Invalid file descriptor!\n");
	return SYSERR;
    }
    if ((buf == NULL) || (nbytes < 1 )) {
	return SYSERR;
    }
    
    oftptr = &oft[fd];
    if (oftptr->state == FSTATE_CLOSED) {
	printf ("File descriptor not open!\n");
	return SYSERR;
    }
    if (oftptr->fileptr >= oftptr->in.size) {
	printf ("Attempted to read beyond EOF!\n");
	return EOF;
    }
    /* if nbytes + 'cursor' > file size, */
    /* then adjust nbytes to amount of bytes to the file's end */
    if ((oftptr->fileptr + nbytes) > oftptr->in.size) {
	nbytes = oftptr->in.size - oftptr->fileptr;
    }

    blocksz = fsd.blocksz;
    /* get the file block that the 'cursor' points */
    fileblock = oftptr->fileptr / blocksz;
    /* get the file offset */
    offset = oftptr->fileptr % blocksz;
    /* get the actual disk block */
    diskblock = fs_fileblock_to_diskblock (0, fd, fileblock);
    /* read the disk block */
    if ( (bs_bread (0, diskblock, offset, buf, nbytes)) 
	 == SYSERR ) {
	printf ("bs_read() error!\n");
	return SYSERR;
    }
    /* advances the 'cursor' */
    oftptr->fileptr += nbytes;
    
    return nbytes;
}

int fs_get_next_free_data_block()
{
    int i = NUM_INODE_BLOCKS+2;//2 For Super and Freemask block
    while (i<fsd.nblocks) {
	if (fs_getmaskbit(i) == 0) {
	    return i;
	}
	i++;
    }
    return SYSERR;
}

int fs_close(int fd)
{
    struct filetable *oftptr;

    if ((fd < 0) || (fd > (NUM_FD-1))) {
	printf ("Invalid file descriptor!\n");
	return SYSERR;
    }
    oftptr = &oft[fd];
    if (oftptr->state == FSTATE_CLOSED) {
	printf ("File descriptor not open!\n");
	return SYSERR;
    }
    oftptr->state = FSTATE_CLOSED;

    return OK;
}
int fs_find_file(char *filename)
{
    int i = 0;
    for (i = 0; i < fsd.root_dir.numentries; i++) {
	if (strncmp(fsd.root_dir.entry[i].name, 
		    filename, strlen(filename)) == 0) {
	    return i;
	}
    }
    return -1;
}

int fs_create_file_inode(struct inode *in)
{
    fsd.inodes_used += 1;
    in->id = fsd.inodes_used;
    in->type = INODE_TYPE_FILE;
    in->device = 0;
    in->size = 0;
    fs_put_inode_by_num(0, in->id, in);
    return OK;
}
int fs_create(char *filename, int mode)
{
    int ret_val = 0;
    struct inode in;
    if (mode != O_CREAT) {
	return SYSERR;
    }
    ret_val = fs_find_file(filename);
    if (ret_val >= 0) {
	return SYSERR;
    }
    fs_create_file_inode(&in);
    fsd.root_dir.numentries += 1;
    fsd.root_dir.entry[fsd.root_dir.numentries-1].inode_num =
	in.id;
    strncpy(fsd.root_dir.entry[fsd.root_dir.numentries-1].name, 
	    filename, strlen(filename));
    return OK;
}
int fs_open(char *filename, int mode)
{
    int inode_num;
    int ret_val;
    ret_val = fs_find_file(filename);
    if (ret_val < 0) {
	ret_val = fs_create(filename, mode);
	if (ret_val == SYSERR) {
	    return SYSERR;
	}
    }
    inode_num = fsd.root_dir.entry[ret_val].inode_num;
    fs_get_inode_by_num(0, inode_num, &oft[next_open_fd].in);
    oft[next_open_fd].state = FSTATE_OPEN;
    oft[next_open_fd].fileptr = 0;
    oft[next_open_fd].de = &fsd.root_dir.entry[ret_val];
    return next_open_fd++;
}
int fs_mount(int dev)
{
    struct inode root_dir_inode;
    int ret_val;
    if (dev != 0) {
	printf("Unsupported device\n");
	return SYSERR;
    }
    fsd.inodes_used += 1;
    fsd.root_dir.numentries = 1;
    fsd.root_dir.entry[0].inode_num = 
	fsd.inodes_used;
    root_dir_inode.id = fsd.root_dir.entry[0].inode_num;
    root_dir_inode.type = INODE_TYPE_DIR;
    root_dir_inode.device = dev;
    root_dir_inode.size = MDEV_BLOCK_SIZE;
    ret_val = fs_get_next_free_data_block();
    if (ret_val == SYSERR) {
	return SYSERR;
    }
    fs_setmaskbit(ret_val);
    root_dir_inode.blocks[
	(root_dir_inode.size/MDEV_BLOCK_SIZE)-1] = ret_val;
    fs_put_inode_by_num(0, 
			fsd.root_dir.entry[0].inode_num,
	                &root_dir_inode);
    strncpy(fsd.root_dir.entry[0].name, 
	    "/mnt_my_fs", strlen("/mnt_my_fs"));
    
    return OK;
}

int fs_fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;

  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }

  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address

  return diskblock;
}

/* read in an inode and fill in the pointer */
int
fs_get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_get_inode_by_num: inode %d out of range\n", 
	   inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  inode_off = inn * sizeof(struct inode);

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  printf("inn*sizeof(struct inode): %d\n", inode_off);
  */

  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));

  return OK;

}

int
fs_put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("fs_put_inode_by_num: inode %d out of range\n", 
	   inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;

  /*
  printf("in_no: %d = %d/%d\n", inode_number, bl, inn);
  */
  if (fs_getmaskbit(bl) == 0) {
      fs_setmaskbit(bl);
  }
  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], 
	 in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);

  return OK;
}
     
int fs_mkfs(int dev, int num_inodes) {
  int i;
  
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }

  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8; 
  
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) 
      == (void *)SYSERR) {
    printf("fs_mkfs memget failed.\n");
    return SYSERR;
  }
  
  /* zero the free mask */
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }
  
  fsd.inodes_used = 0;
  
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);

  return 1;
}

void
fs_print_fsd(void) {

  printf("fsd.ninodes: %d\n", fsd.ninodes);
  printf("sizeof(struct inode): %d\n", sizeof(struct inode));
  printf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  printf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}

/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;

  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;

}

/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}

/* This is maybe a little overcomplicated since the lowest-numbered
   block is indicated in the high-order bit.  Shift the byte by j
   positions to make the match in bit7 (the 8th bit) and then shift
   that value 7 times to the low-order bit to print.  Yes, it could be
   the other way...  */
void fs_printfreemask(void) {
  int i,j;

  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

#endif /* FS */
