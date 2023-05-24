#include <iostream>

#include "speller.h"

int Tmain(int argc, FSTCHAR** argv, FSTCHAR**envp)
    {
    return vabamorf::MTemplateJson<vabamorf::SPELLERJSON>(argc, argv);
    }
