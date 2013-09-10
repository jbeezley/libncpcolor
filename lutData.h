
namespace lut {

#include "luts/Accent.h"
#include "luts/afmhot.h"
#include "luts/autumn.h"
#include "luts/binary.h"
#include "luts/Blues.h"
#include "luts/bone.h"
#include "luts/BrBG.h"
#include "luts/brg.h"
#include "luts/BuGn.h"
#include "luts/BuPu.h"
#include "luts/bwr.h"
#include "luts/CMRmap.h"
#include "luts/cool.h"
#include "luts/coolwarm.h"
#include "luts/copper.h"
#include "luts/cubehelix.h"
#include "luts/Dark2.h"
#include "luts/flag.h"
#include "luts/gist_earth.h"
#include "luts/gist_gray.h"
#include "luts/gist_heat.h"
#include "luts/gist_ncar.h"
#include "luts/gist_rainbow.h"
#include "luts/gist_stern.h"
#include "luts/gist_yarg.h"
#include "luts/GnBu.h"
#include "luts/gnuplot.h"
#include "luts/gnuplot2.h"
#include "luts/gray.h"
#include "luts/Greens.h"
#include "luts/Greys.h"
#include "luts/hot.h"
#include "luts/hsv.h"
#include "luts/jet.h"
#include "luts/ocean.h"
#include "luts/Oranges.h"
#include "luts/OrRd.h"
#include "luts/Paired.h"
#include "luts/Pastel1.h"
#include "luts/Pastel2.h"
#include "luts/pink.h"
#include "luts/PiYG.h"
#include "luts/PRGn.h"
#include "luts/prism.h"
#include "luts/PuBu.h"
#include "luts/PuBuGn.h"
#include "luts/PuOr.h"
#include "luts/PuRd.h"
#include "luts/Purples.h"
#include "luts/rainbow.h"
#include "luts/RdBu.h"
#include "luts/RdGy.h"
#include "luts/RdPu.h"
#include "luts/RdYlBu.h"
#include "luts/RdYlGn.h"
#include "luts/Reds.h"
#include "luts/seismic.h"
#include "luts/Set1.h"
#include "luts/Set2.h"
#include "luts/Set3.h"
#include "luts/Spectral.h"
#include "luts/spring.h"
#include "luts/summer.h"
#include "luts/terrain.h"
#include "luts/winter.h"
#include "luts/YlGn.h"
#include "luts/YlGnBu.h"
#include "luts/YlOrBr.h"
#include "luts/YlOrRd.h"

    static const int NTables = 69;
    struct lutItem {
        const char *name;
        const unsigned char* data;
    };
    static const struct lutItem lookupTables[] =
            {

                {"Accent", Accent},
                {"afmhot", afmhot},
                {"autumn", autumn},
                {"binary", binary},
                {"Blues", Blues},
                {"bone", bone},
                {"BrBG", BrBG},
                {"brg", brg},
                {"BuGn", BuGn},
                {"BuPu", BuPu},
                {"bwr", bwr},
                {"CMRmap", CMRmap},
                {"cool", cool},
                {"coolwarm", coolwarm},
                {"copper", copper},
                {"cubehelix", cubehelix},
                {"Dark2", Dark2},
                {"flag", flag},
                {"gist_earth", gist_earth},
                {"gist_gray", gist_gray},
                {"gist_heat", gist_heat},
                {"gist_ncar", gist_ncar},
                {"gist_rainbow", gist_rainbow},
                {"gist_stern", gist_stern},
                {"gist_yarg", gist_yarg},
                {"GnBu", GnBu},
                {"gnuplot", gnuplot},
                {"gnuplot2", gnuplot2},
                {"gray", gray},
                {"Greens", Greens},
                {"Greys", Greys},
                {"hot", hot},
                {"hsv", hsv},
                {"jet", jet},
                {"ocean", ocean},
                {"Oranges", Oranges},
                {"OrRd", OrRd},
                {"Paired", Paired},
                {"Pastel1", Pastel1},
                {"Pastel2", Pastel2},
                {"pink", pink},
                {"PiYG", PiYG},
                {"PRGn", PRGn},
                {"prism", prism},
                {"PuBu", PuBu},
                {"PuBuGn", PuBuGn},
                {"PuOr", PuOr},
                {"PuRd", PuRd},
                {"Purples", Purples},
                {"rainbow", rainbow},
                {"RdBu", RdBu},
                {"RdGy", RdGy},
                {"RdPu", RdPu},
                {"RdYlBu", RdYlBu},
                {"RdYlGn", RdYlGn},
                {"Reds", Reds},
                {"seismic", seismic},
                {"Set1", Set1},
                {"Set2", Set2},
                {"Set3", Set3},
                {"Spectral", Spectral},
                {"spring", spring},
                {"summer", summer},
                {"terrain", terrain},
                {"winter", winter},
                {"YlGn", YlGn},
                {"YlGnBu", YlGnBu},
                {"YlOrBr", YlOrBr},
                {"YlOrRd", YlOrRd}
            };
}