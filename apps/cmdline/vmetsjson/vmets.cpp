#include "vmets.h"
#include "vmetsjson.h"

int Tmain(int argc, FSTCHAR** argv, FSTCHAR**envp)
    {
    return MainTemplate<VMETS>(argc, argv, envp, NULL);
    }
