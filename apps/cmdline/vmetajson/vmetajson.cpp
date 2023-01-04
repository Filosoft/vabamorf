#include "vmetajson.h"
#include <jsoncpp/json/json.h> 

#include <iostream>
int Tmain(int argc, FSTCHAR** argv, FSTCHAR**envp)
    {
    return MTemplateJson<VMETAJSON>(argc, argv);
    }
