
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <cstring>
#include <string>
#include <iostream>

#include "lookupTable.h"
#include "norm.h"
#include "ncFileReader.h"

using namespace std;

// Display help information
void help(){
    printf("nc2pcolor - Output a binary pseudo-color raster of a 2D slice of a variable\n");
    printf("            in a netCDF file.  Note: this program uses C-style indexing, so\n");
    printf("            the fastest incrementing index (typically the x-coordinate) is last.\n\n");
    printf("Usage: ");
    printf("nc2pcolor [ options ] filename variable\n\n");
    printf("Options:\n");
    printf("    -h or --help:          Displays this information.\n");
    printf("    -v or --verbose:       Verbose mode on.\n");
    printf("    -V or --version:       Displays the current version number.\n");
    printf("    -x or --xDim=integer:  Specify the horizontal dimension, must be in the range:\n");
    printf("                               0 <= x <= nDims-1 [default nDims-1].\n");
    printf("    -y or --yDim=integer:  Specify the vertical dimension, must be in the range:\n");
    printf("                               0 <= y <= nDims-1 [default nDims-2].\n");
    printf("    -o or --output=string: Output filename [default: output.bin].\n");
    printf("    -i or --index=string:  Comma separated list of indices to display\n");
    printf("                               sliced indices ignored. [default \"0,0,0,...\"].\n\n");
    printf("Example:\n");
    printf("    nc2pcolor -x 2 -y 1 -index=\"10,0,0,1\" --output=wind_10_1.bin file.nc U\n");
    exit(1);
}

int main(int argc, char *argv[]){
    // Here your var definition
    
    // Here flags (options without arguments) and arguments with defined type
    char verbose=0;
    char version=0;
    int xDim;
    int yDim;
    char *output;
    char *index;

    // GetOpt option definition
    char *opt_xDim=0;
    char *opt_yDim=0;
    char *opt_output=0;
    char *opt_index=0;

    int next_option;
    const char* const short_options = "hvVx:y:o:i:" ;
    const struct option long_options[] =
        {
            { "help", 0, NULL, 'h' },
            { "verbose", 0, NULL, 'v' },
            { "version", 0, NULL, 'V' },
            { "xDim", 1, NULL, 'x' },
            { "yDim", 1, NULL, 'y' },
            { "output", 1, NULL, 'o' },
            { "index", 1, NULL, 'i' },
            { NULL, 0, NULL, 0 }
        };

    // Parse options
    while (1) {
        // Obtain a option
        next_option = getopt_long (argc, argv, short_options, long_options, NULL);

        if (next_option == -1)
            break; // No more options. Break loop.

        switch (next_option){

            case 'h' : // -h or --help 
                help();
                break;

            case 'v' : // -v or --verbose
                verbose=1;
                break;

            case 'V' : // -V or --version
                version=1;
                break;

            case 'x' : // -x or --xDim
                opt_xDim=optarg;
                xDim=atoi(opt_xDim);
                break;

            case 'y' : // -y or --yDim
                opt_yDim=optarg;
                yDim=atoi(opt_yDim);
                break;

            case 'o' : // -o or --output
                opt_output=optarg;
                output=(char *)malloc(sizeof(char) * (strlen(opt_output)+1));
                strcpy(output,opt_output);
                break;

            case 'i' : // -i or --index
                opt_index=optarg;
                index=(char *)malloc(sizeof(char) * (strlen(opt_index)+1));
                strcpy(index,opt_index);
                break;

            case '?' : // Invalid option
                help(); // Return help

            case -1 : // No more options
                break;

            default : // Something unexpected? Aborting
                return(1);
        }
    }
    
    // Iterate over rest arguments called argv[optind]
    while (optind < argc){
        // Your code here 
        
        optind++;
    }

    return 0;
}

