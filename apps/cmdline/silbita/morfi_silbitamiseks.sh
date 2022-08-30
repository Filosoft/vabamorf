#!/bin/sh

# 1) morf analüüs Filosofti analüsaatoriga

# sisend olgu utf-8
# väljund on läbivalt väiketäheline
# valjundis tähistab liitsõnasisest sõnapiiri _ 

# sed 's/ \([^ ]\)/ \l\1/g'     ## alustav suurtäht väikeseks
# sed '/####/s/    ####//'      ## oletamist vajavaid (sõnastikust puuduvaid) sõnad slbitame lihtsõnadena|
#                               
# sed '/    /s/^[^ ]*    / /'   ## 4 tühikut -> 1 tühik
# sed 's/ \/\/_[^\/]*\/\///g'   ## kustutame esimesest veerust algse sõna
# sed 's/+0*//g'                ## kustutame 0-lõpu
# sed 's/\([aeiouõäöü]\)=\(is[tm]\)/\1.\2/g'    # tao=ist -> tao.ist
# sed 's/=//g'                  ## võrdusmärk tüvest välja
# sed 's/$/ /'                  ## tühik realõppu
# sed 's/ \([^ ]*\) .* \1 / \1 /'               ## kustutame korduvad variandid
# sed '/ ise_enes/s/ iseenes[^ ]* //' \
# tr -s ' '                     ## kustutame korduvad tühikud
# sed 's/^ //' | sed 's/ $//'   ## kustutame rea algusest/lõpust tühikud

# sed 's/^\([^ ]*\)_\([^ _]*\) \1\2$/\1\2/'                 ## kui on liitsõna ja lihtsõna-analüüs
# sed 's/^\([^ ]*\)\([^ _]*\) \1_\2$/\1\2/'                 ## jäta alles ainult lihtsõna analüüs

# sed 's/^\([^ ]*\)se_\([^ ]*\) \1s_e\2$/\1s_e\2/'          ## mitmest analüüsist ühe valimine, valime ...s_e...
# sed 's/^\([^ ]*\)s_e\([^ ]*\) \1se_\2$/\1s_e\2/'          ## ei tea, kumb esimene, sellest 2 

# sed 's/^\([^ ]*\)\([aeiu]\)_\([^ ]*\) \1_\2\3$/\1\2_\3/'  ## mitmest valimine
# sed 's/^\([^ ]*\)_\([aeiu]\)\([^ ]*\) \1\2_\3$/\1\2_\3/'  ## mitmest valimine

# sed 's/^\([^ ]*\)_sal\([^ ]*\) \1s_alu\2$/\1_sal\2/'      ## mitmest valimine
# sed 's/^\([^ ]*\)s_alu\([^ ]*\) \1salu_\2$/\1_sal\2/'     ## mitmest valimine

# sed 's/ .*$//'        ## mitmest analüüsist võta esimene
# sed 's/./\l&/g'       ## läbivalt väiketäheliseks



vmeta --plaintext --dontguess --dontguesspropnames --stem --fs --dontaddphonetics\
| sed 's/ \([^ ]\)/ \l\1/g' \
\
| sed '/####/s/    ####//' \
| sed '/    /s/^[^ ]*    / /' \
| sed 's/ \/\/_[^\/]*\/\///g' \
| sed 's/+0*//g' \
| sed 's/\([aeiouõäöü]\)=\(is[tm]\)/\1.\2/g' \
| sed 's/\([aeiouõäöü]\)<\(is[tm]\)/\1.\2/g' \
| sed 's/\([aeiouõäöü]\)?\(is[tm]\)/\1.\2/g' \
| sed 's/=//g' \
| sed 's/$/ /' \
| sed 's/ \([^ ]*\) .* \1 / \1 /' \
| sed 's/ \([^ ]*\) .* \1 / \1 /' \
| sed 's/ \([^ ]*\) .* \1 / \1 /' \
| sed 's/ \([^ ]*\) .* \1 / \1 /' \
| sed '/ ise_enes/s/ iseenes[^ ]* //' \
| tr -s ' ' \
| sed 's/^ //' \
| sed 's/ $//' \
\
| sed 's/^\([^ ]*\)_\([^ _]*\) \1\2$/\1\2/' \
| sed 's/^\([^ ]*\)\([^ _]*\) \1_\2$/\1\2/' \
\
| sed 's/^\([^ ]*\)se_\([^ ]*\) \1s_e\2$/\1s_e\2/' \
| sed 's/^\([^ ]*\)s_e\([^ ]*\) \1se_\2$/\1s_e\2/' \
\
| sed 's/^\([^ ]*\)\([aeiu]\)_\([^ ]*\) \1_\2\3$/\1\2_\3/' \
| sed 's/^\([^ ]*\)_\([aeiu]\)\([^ ]*\) \1\2_\3$/\1\2_\3/' \
\
| sed 's/^\([^ ]*\)_sal\([^ ]*\) \1s_alu\2$/\1_sal\2/' \
| sed 's/^\([^ ]*\)s_alu\([^ ]*\) \1salu_\2$/\1_sal\2/' \
\
| sed 's/ .*$//' \
| sed 's/./\l&/g' \

exit

kui oleks --addphonetics
siis tuleks eemaldada veel need märgid:
aga enne tuleks teisendada k=kk -> kk
sest ilmselt on programmis viga...
| sed 's/[=<?]//g' \
| sed 's/\]//g' \


