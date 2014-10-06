LibreOffice'i spelleri kompileerimiseks:

1. Kompileeri native kood kataloogist cpp ja kopeeri väljundfailid kataloogi oxt/
1.1 Windowsil MSVC 2007-ga
  Projektifail: cpp/ooofslinget.sln
  Väljundfail: ooofslinget.win_x86
1.2 Macil XCode 3-ga
  Projektifail: cpp/ooofslinget.xcodeproj
  Väljundfail: ooofslinget.mac
1.3.1 x86-32 Linuxil gcc-ga
  Käivitage make -f unix/Makefile_cpp
  Väljundfail: oofslinget.lin -> ooofslinget.lin_x86
1.3.2 x86-64 Linuxil gcc-ga
  Käivitage make -f unix/Makefile_cpp
  Väljundfail: oofslinget.lin -> ooofslinget.lin_x64

2. Kompileeri LibreOffice arenduskeskkonnas java kood
  Käivitage make -f unix/Makefile_java
  Kopeerige väljundfailid *.jar oxt/ kataloogi
  
3. Lisage sõnastik
  Kopeerige /dct/et.dct -> oxt/fslinget.lex

4. Valmistage installipakk
  Kui 7za ei asu PATHis, kopeerige see oxt kataloogi
  Käivitage oxt kataloogis _build.bat
