#include "vmetajson.h"
#include <jsoncpp/json/json.h> 

#include <iostream>
int Tmain(int argc, FSTCHAR** argv, FSTCHAR**envp)
    {
    return MainTemplate<VMETAJSON>(argc, argv, envp, NULL);
    }
