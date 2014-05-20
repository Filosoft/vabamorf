#include "../../lib/json.h"
#include "../../../lib/etana/etmrf.h"
#include "../../../lib/proof/proof.h"

class CSettings {
public:
	CSettings() {
		m_bAnalyzeGuess=false;
		m_bAnalyzePhon=false;
		m_bSpellSuggest=false;
	}

	CFSAString m_szCommand;

	bool m_bAnalyzeGuess;
	bool m_bAnalyzePhon;

	bool m_bSpellSuggest;
};

class CMyJSONReader : public CJSONReader, public CSuggestor {
public:
	CMyJSONReader(CFSStream &Stream, ETMRFA &Morf, const CSettings &Settings, CJSONWriter &Writer) : CJSONReader(Stream), m_Morf(Morf), m_Settings(Settings), m_Writer(Writer) { }

protected:
	void OnValReadStart(const CFSAString &szKey) {
		//printf("OnObjectReadStart(%s)\n", (const char *)szKey);
		if (szKey.IsEmpty()) {
			m_Writer.ObjectStart();
		} else if (szKey=="/paragraphs") {
			m_Writer.Key("paragraphs");
			m_Writer.ArrayStart();
			m_iCollectData--;
		} else if (KeyMatch(szKey, "/paragraphs/%d")) {
			m_Writer.ObjectStart();
			m_iCollectData++;
		} else if (KeyMatch(szKey, "/paragraphs/%d/sentences")) {
			m_Writer.Key("sentences");
			m_Writer.ArrayStart();
			m_iCollectData--;
		} else if (KeyMatch(szKey, "/paragraphs/%d/sentences/%d")) {
			m_iCollectData++;
		}
	}

	void SubKeys(const CFSAString szExcept, const CFSVar &Data) {
		for (INTPTR ip=0; ip<Data.GetSize(); ip++) {
			CFSAString szKey=Data.GetKey(ip);
			if (szKey==szExcept) continue;
			m_Writer.Key(szKey);
			m_Writer.Val(Data[szKey]);
		}
	}

	void OnValReadEnd(const CFSAString &szKey, CFSVar &Data) {
		//printf("OnObjectReadEnd(%s, ...)\n", (const char *)szKey);
		if (szKey.IsEmpty()) {
			SubKeys("paragraphs", Data);
			m_Writer.ObjectEnd();
		} else if (szKey=="/paragraphs") {
			m_Writer.ArrayEnd();
			m_iCollectData++;
		} else if (KeyMatch(szKey, "/paragraphs/%d")) {
			SubKeys("sentences", Data);
			m_Writer.ObjectEnd();
			m_iCollectData--;
		} else if (KeyMatch(szKey, "/paragraphs/%d/sentences")) {
			m_Writer.ArrayEnd();
			m_iCollectData++;
		} else if (KeyMatch(szKey, "/paragraphs/%d/sentences/%d")) {
			if (Data.KeyExist("words")) {
				CFSVar &Words=Data["words"];

				if (m_Settings.m_szCommand=="spell") {

					m_Morf.SetFlags(MF_DFLT_SPL);
					m_Morf.SetMaxTasand();
					m_Morf.Clr();

					for (INTPTR ip=0; ip<Words.GetSize(); ip++) {
						CFSVar &Word=Words[ip];
						CFSWString szText=Word["text"].GetWString();
						m_Morf.Set1(szText);
						m_Morf.Tag<int>((int)ip, PRMS_TAGSINT);
					}

					LYLI Lyli;
					bool bSpelling=true;
					while (m_Morf.Flush(Lyli)) {
						if (Lyli.lipp & PRMS_TAGSINT){
							INTPTR ipPos=Lyli.ptr.arv;
							Words[ipPos]["spelling"]=bSpelling;
						} else if (Lyli.lipp & PRMS_MRF) {
							bSpelling=Lyli.ptr.pMrfAnal->on_tulem();
						}
					}

					if (m_Settings.m_bSpellSuggest) {
						for (INTPTR ip=0; ip<Words.GetSize(); ip++) {
							if (!Words[ip]["spelling"].GetBool()) {
								m_Morf.SetMaxTasand();
								CFSVar Suggs;
								Suggs.Cast(CFSVar::VAR_ARRAY);
								Suggest(Words[ip]["text"].GetWString(), ip==0);
								for (INTPTR ipSugg=0; ipSugg<GetSize(); ipSugg++) {
									CFSWString szSugg;
									GetItem(ipSugg, szSugg);
									Suggs[ipSugg]=szSugg;
								}
								Words[ip]["suggestions"]=Suggs;
							}
						}
					}

				} else if (m_Settings.m_szCommand=="analyze") {

					MRF_FLAGS_BASE_TYPE Flags=MF_DFLT_MORFA;
					if (m_Settings.m_bAnalyzeGuess) Flags|=MF_OLETA;
					if (m_Settings.m_bAnalyzePhon) Flags|=MF_KR6NKSA;
					m_Morf.SetFlags(Flags);
					m_Morf.SetMaxTasand();
					m_Morf.Clr();

					CFSArray<CFSVar> Words2;
					for (INTPTR ip=0; ip<Words.GetSize(); ip++) {
						CFSVar &Word=Words[ip];
						Words2.AddItem(Word);
						CFSWString szText=Word["text"].GetWString();
						m_Morf.Set1(szText);
						m_Morf.Tag<int>((int)ip, PRMS_TAGSINT);
					}
					Words.Cleanup();
					Words.Cast(CFSVar::VAR_ARRAY);

					LYLI Lyli;
					CFSVar Analysis;
					Analysis.Cast(CFSVar::VAR_ARRAY);
					INTPTR ipLastPos=-1;
					INTPTR ipDeleted=0;

					while (m_Morf.Flush(Lyli)) {
						if (Lyli.lipp & PRMS_TAGSINT){
							INTPTR ipPos=-ipDeleted+Lyli.ptr.arv;
							if (ipLastPos==-1) {
								Words2[ipPos]["analysis"]=Analysis;
								ipLastPos=ipPos;
							} else {
								Words2[ipLastPos]["text"]=Words2[ipLastPos]["text"].GetAString()+" "+Words2[ipPos]["text"].GetAString();
								Words2.RemoveItem(ipPos);
								ipDeleted++;
							}
						} else if (Lyli.lipp & PRMS_MRF) {
							Analysis.Cleanup();
							Analysis.Cast(CFSVar::VAR_ARRAY);
							ipLastPos=-1;
							Lyli.ptr.pMrfAnal->StrctKomadLahku();
							for (INTPTR ipTul=0; ipTul<Lyli.ptr.pMrfAnal->idxLast; ipTul++){
								MRFTUL Tul=*(*Lyli.ptr.pMrfAnal)[(int)ipTul];
								CFSVar Analysis1;
								Analysis1["root"]=Tul.tyvi;
								Analysis1["ending"]=Tul.lopp;
								Analysis1["clitic"]=Tul.kigi;
								Analysis1["partofspeech"]=Tul.sl;
								CFSWString szForm=Tul.vormid; szForm.TrimRight(L", ");
								Analysis1["form"]=szForm;
								Analysis[ipTul]=Analysis1;
							}
						}
					}

					for (INTPTR ip=0; ip<Words2.GetSize(); ip++) {
						Words[ip]=Words2[ip];
					}
				}

			}
			m_Writer.Val(Data);
			m_iCollectData--;
		}
	}

	// Speller & suggestor callbacks
	SPLRESULT SpellWord(const CFSWString &szWord, CFSWString &szWordReal, long *pLevel) {
		m_Morf.Clr();
		m_Morf.Set1(szWord);
		LYLI Lyli;
		if (!m_Morf.Flush(Lyli)) return SPL_NOERROR;
		ASSERT((Lyli.lipp & PRMS_MRF) && Lyli.ptr.pMrfAnal);
		if (!Lyli.ptr.pMrfAnal->on_tulem()) return SPL_INVALIDWORD;

		if (pLevel) *pLevel=Lyli.ptr.pMrfAnal->tagasiTasand;

		MRF_FLAGS Flags(MF_DFLT_SPL); FSXSTRING XString;
		(*Lyli.ptr.pMrfAnal)[0]->Strct2Strng(&XString, &Flags);
		szWordReal=XString;
		szWordReal.Remove(FSWSTR('_'));
		szWordReal.Trim();

		return SPL_NOERROR;
	}

	void SetLevel(long lLevel) {
		m_Morf.SetMaxTasand(lLevel);
	}

protected:
	ETMRFA &m_Morf;
	CSettings m_Settings;
	CJSONWriter m_Writer;
};

int PrintUsage() {
	fprintf(stderr, "Use: etana command -options\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Commands:\n");
	fprintf(stderr, " analyze   -- morphological analysis\n");
	fprintf(stderr, " spell     -- check spelling\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, " -in file  -- read input from file, default input from stdin\n");
	fprintf(stderr, " -out file -- write output to file, default output stdout\n");
	fprintf(stderr, " -lex path -- path to lexicon files, default active directory\n");
	fprintf(stderr, " -guess    -- guess forms for unknown words (analyze only)\n");
	fprintf(stderr, " -phonetic -- add phonetic markup (analyze only)\n");
	fprintf(stderr, " -suggest  -- suggest correct forms (spell only)\n");
	fprintf(stderr, "\n");
	return -1;
}

int main(int argc, char* argv[])
{
	int iRes=-1;
	FSCInit();

	try {
		// Command line parsing
		if (argc<=2) {
			return PrintUsage();
		}

		CSettings Settings;
		Settings.m_szCommand=argv[1];
		if (Settings.m_szCommand!="analyze" && Settings.m_szCommand!="spell") {
			return PrintUsage();
		}

		CFSAString InFileName, OutFileName, LexPath=".";
		for (int i=2; i<argc; i++) {
			if (CFSAString("-lex")==argv[i]) {
				if (i+1<argc) {
					LexPath=argv[++i];
				} else {
					return PrintUsage();
				}
			} else if (CFSAString("-in")==argv[i]) {
				if (i+1<argc) {
					InFileName=argv[++i];
				} else {
					return PrintUsage();
				}
			} else if (CFSAString("-out")==argv[i]) {
				if (i+1<argc) {
					OutFileName=argv[++i];
				} else {
					return PrintUsage();
				}
			} else if (CFSAString("-guess")==argv[i]) {
				Settings.m_bAnalyzeGuess=true;
			} else if (CFSAString("-phonetic")==argv[i]) {
				Settings.m_bAnalyzePhon=true;
			} else if (CFSAString("-suggest")==argv[i]) {
				Settings.m_bSpellSuggest=true;
			} else {
				return PrintUsage();
			}
		}

		// Initialize streams and set up analyzer
		CFSFile InFile, OutFile;
		if (InFileName.IsEmpty()) {
			InFile.Attach(stdin, false);
		} else {
			InFile.Open(InFileName, "rb");
		}
		if (OutFileName.IsEmpty()) {
			OutFile.Attach(stdout, false);
		} else {
			OutFile.Open(OutFileName, "wb");
		}

		ETMRFA Morf;
		Morf.Start(LexPath, MF_DFLT_MORFA);

		CJSONWriter JSONWriter(OutFile);
		CMyJSONReader JSONReader(InFile, Morf, Settings, JSONWriter);

		// Run the loop
		JSONReader.Read();

		// Success
		iRes=0;
	} catch (const CJSONException &e) {
		fprintf(stderr, "JSON error: %s\n", (const char *)FSStrTtoA(e.GetText()));
	} catch (const VEAD &e) {
		fprintf(stderr, "Morph engine error: "); e.Print();
	} catch (const CFSFileException &e) {
		fprintf(stderr, "I/O error: %d\n", e.m_nError);
	} catch (const CFSException &) {
		fprintf(stderr, "Internal error\n");
	} catch (...) {
		fprintf(stderr, "Unexpected error\n");
	}

	FSCTerminate();
	return iRes;
}
