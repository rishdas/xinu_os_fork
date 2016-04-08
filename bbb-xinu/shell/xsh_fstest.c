#include <stddef.h>
#include <fs.h>
#include <xinu.h>
#define SIZE 1200
void fs_testbitmask(void);

/**
 * @ingroup shell
 *
 * Shell command fstest.
 * @param nargs  number of arguments in args array
 * @param args   array of arguments
 * @return OK for success, SYSERR for syntax error
 */
 shellcmd xsh_fstest(int nargs, char *args[])
{
    int rval;
    int fd, i, j;
    char *buf1, *buf2;
    
    
    /* Output help, if '--help' argument was supplied */
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
    {
        printf("Usage: %s\n\n", args[0]);
        printf("Description:\n");
        printf("\tFilesystem Test\n");
        printf("Options:\n");
        printf("\t--help\tdisplay this help and exit\n");
        return OK;
    }

    /* Check for correct number of arguments */
    if (nargs > 1)
    {
        fprintf(stderr, "%s: too many arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
                args[0]);
        return SYSERR;
    }
    if (nargs < 1)
    {
        fprintf(stderr, "%s: too few arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
                args[0]);
        return SYSERR;
    }

#ifdef FS

    bs_mkdev(0, MDEV_BLOCK_SIZE, MDEV_NUM_BLOCKS); /* device "0" and default blocksize (=0) and count */
    fs_mkfs(0,DEFAULT_NUM_INODES); /* bsdev 0*/
    fs_mount(0);
    fd = fs_create("Test_File");
    buf1 = getmem(SIZE*sizeof(char));
    buf2 = getmem(SIZE*sizeof(char));

    // Fill buffer with random stuff
    /* for(i=0; i<SIZE; i++) */
    /* { */
    /*     j = i%(127-33); */
    /*     j = j+33; */
    /*     buf1[i] = (char) j; */
    /* } */
    strncpy (buf1,"TLmVDUDGDKLoNPrMSnuLdzrO4Erwdocfj55rEcR5nzG76FpAU0"	\
"jbLlEeKl7B3CbSZvhD8KP1a3mhaRWzF8kVPdBaOeroB5hq0J5mg0Hp0y0"\
"YiiMfbRFWQbDlHRNdNCOppqljGL8m9oDcDvajiJlwdOVjrKRO6fuPSujbqA"\
"JTgnbNKuEnz6Be8euhkGK6EeTSnNAVua2Quj8YqTCcdExSJBRTAuD33N8sxz"\
"mf1kc1KRDgLqkxfRbMobvQ9FQWrGZcpm0trkN1K3PcQJ4vzpbmc0xEwLjV"\
"CxxxgRpCdjAKJUX4KzNVJYCw9Z1yNsrIbwArCX3n92ZGwpOkqzpqIl6KMV"\
"sor0kacATHjHo2yTCJNATGMyYa8H2GRIqm35IwkBCfhTLrP5ozsgL2mtjN"\
"uc0NmFxNVibhXSGyx0PNaa3thiC45l8XhNfQuw0eynofGOh28ULyRPR9eZ"\
"9ZH45lGRTRRkMY6cIq33isrZylDLLhEWqmntafIkn1jMdo66nMFij5LywO"\
"1abTm7tDxriPX2KPmSa8Iou7LrRY0fcy48bSoizRwgvCTWBYOY72cGAjBf"\
"qY3IS6gWyOvXT5FU9u5cXrtbjNbHH4kmBLVNrEtOfYilSkYJmAoluvvuLv"\
"cZVycA0nSz9UamqrpSiI2Q8WJwntK7IFe5DcGqEYfuLhMI0q8abWWMHLEb"\
"U0jZLZAY1rrhZLhVMsKsb3G5a6cvsAxSeVCwZocnxr0pbr5JEPzyi0lUDJ"\
"rtSSrjtLkA2lhtHlO3kmhWCOGgi0XBfeONAEEk3wD2x3Uawdj2rVgJE8FZ"\
"Uu4oDGOFNaAxMqIAC1qgx9i1F4zJVSdmmHMuLk9Jifns6Ee1JAmLERmf8Y"\
"rhVtnjugQtvGk3RaiAl5VnFvNQeehPyvGxZH410wg5zGpE4kr8J5slPVIW"\
"rGr17bzrSVNG6qJVqqn2heynU5uxhcn3GYbb7v4I2atNd9jJGAC81JI2LZ"\
"qbiSLSxxhkg0aWBwtwdnBOv0VXTbiN8vbuYCYtPHGDadNKdI7Hv9eLMGal"\
"36OktMzZba3ocvaVyWifU3np5Bq4VnOQjaF0bpNtMxYlMDy503Rf9smW7M"\
"vZUxuele1GEnTBhG0BNpJ5jfNuWRmAftT3mq1QBLpY8RLXHK69PaaJ0rAh"\
	     "U1TOHuf9d7YgzfdZmk51gIbWBK4y7BtmgkO8nPWN6Ls8Tp",SIZE);
    
    rval = fs_write(fd,buf1,SIZE);
    if(rval == 0 || rval != SIZE )
    {
        printf("\n\r File write failed");
    }
    printf("%s\n", buf1);
    fs_close(fd);

    fd = fs_open ("Test_File", O_RDONLY);
    /* fs_seek(fd,-SIZE); */
    rval = fs_read(fd, buf2, rval);
    printf("\n+++++++++++++++++++++\n");
    printf("%s\n", buf2);
    
    fs_seek(fd,-SIZE);
    rval = fs_read(fd, buf2, rval);
    printf("\n+++++++++++++++++++++\n");
    printf("%s\n", buf2);
    fs_close(fd);

    //fs_testbitmask();
#endif

    return OK;

#ifdef FS_FUNC
    buf1 = getmem(SIZE*sizeof(char));
    buf2 = getmem(SIZE*sizeof(char));
    
    // Create test file
    fd = fs_create("Test_File");
       
    // Fill buffer with random stuff
    for(i=0; i<SIZE; i++)
    {
        j = i%(127-33);
        j = j+33;
        buf1[i] = (char) j;
    }
    
    rval = fs_write(fd,buf1,SIZE);
    if(rval == 0 || rval != SIZE )
    {
        printf("\n\r File write failed");
        goto clean_up;
    }

    // Now my file offset is pointing at EOF file, i need to bring it back to start of file
    // Assuming here implementation of fs_seek is like "original_offset = original_offset + input_offset_from_fs_seek"
    fs_seek(fd,-rval); 
    
    //read the file 
    rval = fs_read(fd, buf2, rval);
    buf2[rval] = EOF; // TODO: Write end of file symbol i.e. slash-zero instead of EOF. I can not do this because of WIKI editor limitation    

    if(rval == SYSERR)
    {
        printf("\n\r File read failed");
        goto clean_up;
    }
        
    printf("\n\rContent of file %s",buf2);
    
    rval = fs_close(fd);
    if(rval != OK)
    {
        printf("\n\rReturn val for fclose : %d",rval);
    }

clean_up:
    freemem(buf1,SIZE);
    freemem(buf2,SIZE);
#endif
/*
#else
    printf("No filesystem support\n");
#endif
*/
    return OK;
}

void
fs_testbitmask(void) {

    fs_setmaskbit(31); fs_setmaskbit(95); fs_setmaskbit(159);
    fs_setmaskbit(223);
    fs_setmaskbit(287); fs_setmaskbit(351); fs_setmaskbit(415);
    fs_setmaskbit(479);
    fs_setmaskbit(90); fs_setmaskbit(154); fs_setmaskbit(218);
    fs_setmaskbit(282);
    fs_setmaskbit(346); fs_setmaskbit(347); fs_setmaskbit(348);
    fs_setmaskbit(349);
    fs_setmaskbit(350); fs_setmaskbit(100); fs_setmaskbit(164);
    fs_setmaskbit(228);
    fs_setmaskbit(292); fs_setmaskbit(356); fs_setmaskbit(355);
    fs_setmaskbit(354);
    fs_setmaskbit(353); fs_setmaskbit(352);
    
    fs_printfreemask();

    fs_clearmaskbit(31);  fs_clearmaskbit(95); 
    fs_clearmaskbit(159); fs_clearmaskbit(223);
    fs_clearmaskbit(287); fs_clearmaskbit(351); 
    fs_clearmaskbit(415); fs_clearmaskbit(479);
    fs_clearmaskbit(90);  fs_clearmaskbit(154); 
    fs_clearmaskbit(218); fs_clearmaskbit(282);
    fs_clearmaskbit(346); fs_clearmaskbit(347);
    fs_clearmaskbit(348); fs_clearmaskbit(349);
    fs_clearmaskbit(350); fs_clearmaskbit(100); 
    fs_clearmaskbit(164); fs_clearmaskbit(228);
    fs_clearmaskbit(292); fs_clearmaskbit(356); 
    fs_clearmaskbit(355); fs_clearmaskbit(354);
    fs_clearmaskbit(353); fs_clearmaskbit(352);

    fs_printfreemask();

}