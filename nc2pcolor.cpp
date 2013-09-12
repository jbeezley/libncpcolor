
#include <stdio.h>
#include <cstdlib>
#include <getopt.h>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

#include <png.h>

#include "lookupTable.h"
#include "norm.h"
#include "ncFileReader.h"
#include "lutData.h"

#define _MAX_VARDIMS 16
#define HAVE_PNG 1

#ifdef HAVE_PNG
#include "write_png.h"
#define OUTPUT_EXT "png"
#else
#define OUTPUT_EXT "bin"
#endif

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
    printf("    -l or --help-color:    List color lookup tables.\n");
    printf("    -x or --xDim=integer:  Specify the horizontal dimension, must be in the range:\n");
    printf("                               0 <= x <= nDims-1 [default nDims-1].\n");
    printf("    -y or --yDim=integer:  Specify the vertical dimension, must be in the range:\n");
    printf("                               0 <= y <= nDims-1 [default nDims-2].\n");
    printf("    -o or --output=string: Output filename [default: output.");
    printf(OUTPUT_EXT);
    printf("].\n");
    printf("    -i or --index=string:  Comma separated list of indices to display\n");
    printf("                               sliced indices ignored. Invalid entries are\n");
    printf("                               set to zero [default \"0,0,0,...\"].\n");
    printf("    -c or --color=string:  Color lookup table to use (see --help-color) [default jet]\n\n");
    printf("Example:\n");
    printf("    nc2pcolor -x 2 -y 1 -index=\"10,0,0,1\" --output=wind_10_1.bin file.nc U\n");
    exit(1);
}

static bool verbose_output = false;

void list_colors() {
    printf("Color tables available:\n");
    for(int i=0; i<lut::NTables; i++) {
        cout << lut::lookupTables[i].name << endl;
    }
    exit(1);
}

bool parse_index(const string& index, int& n, int vals[_MAX_VARDIMS]) {
    size_t i, k=0;
    n=0;
    string elipses("...");
    for(i=0; i<_MAX_VARDIMS; i++) vals[i] = 0;
    while( n < _MAX_VARDIMS ) {
        i = min(index.find(',', k), index.size());
        string sub(index.substr(k,i-k));
        if(sub == elipses) break;
        try {
            vals[n++] = max(atoi(sub.c_str()),0);
        }
        catch (int e)
        {
            return false;
        }
        k=i+1;
        if(i >= index.size()) return true;
    }
}

bool write_raster(const string& filename,
                  const string& variable,
                  const int xDim,
                  const int yDim, 
                  const int *index,
                  const string& color,
                  const string& output) {
    
    NcSliceFile file(filename);
    if(!file.isOpen()) {
        cout << "Could not open file: " << filename << endl;
        return false;
    }
    const BaseVariable* var = file.getVariable(variable);
    if(!var) {
        cout << "Variable " << variable << " not found." << endl;
        return false;
    }
    BaseVariable::sliceType slice=var->defaultSlice();
    if(verbose_output) {
        cout << "Found variable:" << endl;
        cout << *var << endl;
    }
    if(xDim >= 0) slice.setXDim(xDim);
    if(yDim >= 0) slice.setYDim(yDim);
    for(int i=0;i<slice.size(); i++) slice[i] = index[i];
    if(!slice.isValid()) {
        cout << "Invalid slice description: " << endl;
        cout << slice << endl;
        return false;
    }
    if(verbose_output) {
        cout << "Setting slice to:" << endl;
        cout << slice << endl;
    }
    LinearNorm<double> norm;
    LookupTable lut;
    lut.loadTable(color);
    
    const size_t N = var->sliceSize(slice);
    double *A = new double[N];
    if(!A) {
        cout << "Could not allocate memory." << endl;
        return 0;
    }
    
    var->readSlice(slice, A);
    norm.setMinMaxValFromArray(N, A);
    uint8_t *B = new uint8_t [N]; 
    norm.normalize(N,A,B);
    uint8_t *C = new uint8_t [lut.imageSize(N)];

    lut.makePColor(N,B,C);
    
#ifndef HAVE_PNG
    ofstream fout(output.c_str(), ios::out | ios::binary | ios::trunc);
    for(int i=0;i<lut.imageSize(N);i++) fout << C[i];
    fout.close();
    fout.open((output + ".hdr").c_str(), ios::out | ios::trunc);
    fout << var->shape()[slice.xDim()] << endl;
    fout << var->shape()[slice.yDim()] << endl;
    fout << 4;
    fout.close();
#else
    write_png(output, var->shape()[slice.xDim()], var->shape()[slice.yDim()], C);
#endif
    delete [] A;
    delete [] B;
    delete [] C;
    return true;
}

int main(int argc, char *argv[]){
    // Here your var definition
    
    // Here flags (options without arguments) and arguments with defined type
    char verbose=0;
    char version=0;
    int xDim = -1;
    int yDim = -1;
    string output("output.");
    output += OUTPUT_EXT;
    string index("");
    string color("jet");

    // GetOpt option definition
    char *opt_xDim=0;
    char *opt_yDim=0;
    char *opt_output=0;
    char *opt_index=0;

    int next_option;
    const char* const short_options = "hvVlx:y:o:i:c:" ;
    const struct option long_options[] =
        {
            { "help", 0, NULL, 'h' },
            { "help-color", 0, NULL, 'l' },
            { "verbose", 0, NULL, 'v' },
            { "version", 0, NULL, 'V' },
            { "xDim", 1, NULL, 'x' },
            { "yDim", 1, NULL, 'y' },
            { "output", 1, NULL, 'o' },
            { "index", 1, NULL, 'i' },
            { "color", 1, NULL, 'c' },
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
            
            case 'l' : // -l or --help-color
                list_colors();
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
                output=string(opt_output);//(char *)malloc(sizeof(char) * (strlen(opt_output)+1));
                break;

            case 'i' : // -i or --index
                opt_index=optarg;
                index=string(opt_index);//(char *)malloc(sizeof(char) * (strlen(opt_index)+1));
                break;
            
            case 'c' : // -c or --color
                opt_index=optarg;
                color=string(opt_index);//(char *)malloc(sizeof(char) * (strlen(opt_index)+1));
                break;

            case '?' : // Invalid option
                help(); // Return help

            case -1 : // No more options
                break;

            default : // Something unexpected? Aborting
                return(1);
        }
    }
    verbose_output = verbose;    
    // Iterate over rest arguments called argv[optind]
    int nargs = argc - optind;
    if(nargs != 2) {
        cout << "Invalid arguments." << endl;
        help();
    }
    int nindex, Aindex[_MAX_VARDIMS];
    if(!parse_index(index,nindex,Aindex)) {
        cout << "Error reading index specification: " << index << endl;
    }
    else {
        if(verbose_output) {
            cout << "Read index as: ";
            for(int i=0;i <= nindex; i++) cout << Aindex[i] << " ";
            cout << "0 0 0 ..." << endl;
        }
    }

    string filename(argv[optind]);
    string variable(argv[optind+1]);
    if(write_raster(filename, variable, xDim, yDim, Aindex, color, output)) {
        if(verbose_output) cout << "Success!" << endl;
        return 0;
    }
    else
        return 1;
}

