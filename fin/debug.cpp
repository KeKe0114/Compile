#include "debug.h"
void DEBUG(int level, string log_info, string file, int line)
{
#ifdef DEBUGLEVEL
    if (DEBUGLEVEL >= level)
    {
        for (int i = 0; i < level - 1; i++)
        {
            clog << "  ";
        }
        clog << "in " << file << ":: ";
        clog << "line " << line << ": ";
        clog << log_info << endl;
#ifdef FLUSH_FORCE
        if (FLUSH_FORCE)
            clog.flush();
#endif
    }
#endif
}