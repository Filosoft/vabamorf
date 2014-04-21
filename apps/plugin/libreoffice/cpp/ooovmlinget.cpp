#include "../../../../lib/proof/proof.h"
#include "../../../../lib/fsc/fsjni/fsjni.h"

FSDLL_ENTRYPOINT;

#if defined (GNULDFIX)
#define GNULDFIX_EXPORT FSDLL_EXPORT
#else
#define GNULDFIX_EXPORT
#endif

class CMyLinguistic : public CLinguistic {
public:
};

class CMySuggestor : public CSuggestor{
public:
	CMySuggestor(CMyLinguistic *pLinguistic) {
		m_pLinguistic=pLinguistic;
	}

protected:
	virtual SPLRESULT SpellWord(const CFSWString &szWord, CFSWString &szWordReal, long *pLevel) {
		return m_pLinguistic->SpellWord(szWord, &szWordReal, pLevel);
	}

	virtual void SetLevel(long lLevel) {
		m_pLinguistic->SetLevel(lLevel);
	}

	CMyLinguistic *m_pLinguistic;
};

extern "C" {

GNULDFIX_EXPORT jboolean JNICALL Java_ee_vabamorf_ooo_linguistic_et_Linguistic_init
  (JNIEnv *env, jobject jobj,
  jstring fileName, jboolean reserved)
{
	CMyLinguistic *pLinguistic=(CMyLinguistic *)FSJNIGetLongField(env, jobj, "lingId");

	if (pLinguistic) {
		FSJNISetLongField(env, jobj, "lingId", 0);
		IGNORE_FSEXCEPTION( delete pLinguistic; )
	}

#if defined (UNICODE)
	CFSFileName FileName=FSJNIStrtoW(env, fileName);
#else
	CFSFileName FileName=FSJNIStrtoA(env, fileName);
#endif

	try {
		pLinguistic=new CMyLinguistic();
		FSJNISetLongField(env, jobj, "lingId", (jlong)pLinguistic);
		pLinguistic->Open(FileName);
		return JNI_TRUE;
	} catch(...) {
		return JNI_FALSE;
	}
}

GNULDFIX_EXPORT void JNICALL Java_ee_vabamorf_ooo_linguistic_et_Linguistic_terminate
  (JNIEnv *env, jobject jobj)
{
	CMyLinguistic *pLinguistic=(CMyLinguistic *)FSJNIGetLongField(env, jobj, "lingId");

	if (pLinguistic) {
		FSJNISetLongField(env, jobj, "lingId", 0);
		IGNORE_FSEXCEPTION( delete pLinguistic; )
	}
}

GNULDFIX_EXPORT jboolean JNICALL Java_ee_vabamorf_ooo_linguistic_et_Linguistic_spell
  (JNIEnv *env, jobject jobj,
  jstring sWord, jboolean bIsSpellWithDigits, jboolean bIsSpellUpperCase, jboolean bIsSpellCapitalization)
{
	CMyLinguistic *pLinguistic=(CMyLinguistic *)FSJNIGetLongField(env, jobj, "lingId");

	CPTWord Word(FSJNIStrtoW(env, sWord));
	Word.Trim();
	Word.RemoveHyphens();
	Word.RemovePunctuation();
	if (Word.m_szWord.IsEmpty()) {
		return JNI_TRUE;
	}

	try {
		if (!bIsSpellWithDigits && Word.m_szWord.FindOneOf(FSWSTR("0123456789"))>=0) {
			return JNI_TRUE;
		}

		MRF_FLAGS_BASE_TYPE Flags=MF_DFLT_SPL|MF_LUBAMITMIKUO;
		if (!bIsSpellUpperCase) {
			CFSWString szLower=Word.m_szWord.ToLower();
			CFSWString szUpper=Word.m_szWord.ToUpper();
			if (szUpper==Word.m_szWord && szLower!=Word.m_szWord) return JNI_TRUE;
			Flags&=(~MF_LYHREZH);
		}
		else Flags|=MF_LYHREZH;
		pLinguistic->SetFlags(Flags);
		pLinguistic->SetLevel(100);
		if (pLinguistic->SpellWord(Word.m_szWord)==0) return JNI_TRUE;
		return JNI_FALSE;
	} catch (...) {
		return JNI_TRUE;
	}
}

GNULDFIX_EXPORT jobjectArray JNICALL Java_ee_vabamorf_ooo_linguistic_et_Linguistic_suggest
  (JNIEnv *env, jobject jobj,
  jstring sWord)
{
	CMyLinguistic *pLinguistic=(CMyLinguistic *)FSJNIGetLongField(env, jobj, "lingId");

	CPTWord Word(FSJNIStrtoW(env, sWord));
	Word.Trim();
	Word.RemoveHyphens();
	Word.RemovePunctuation();
	if (Word.m_szWord.IsEmpty()) {
		return 0;
	}

	try {
		pLinguistic->SetFlags(MF_DFLT_SUG|MF_LUBAMITMIKUO);
		pLinguistic->SetLevel(100);
		CMySuggestor Suggestor(pLinguistic);
		Suggestor.Suggest(Word.m_szWord);
		jobjectArray suggs=(jobjectArray)env->NewObjectArray(Suggestor.GetSize(), env->FindClass("java/lang/String"), 0);
		for (INTPTR ip=0; ip<Suggestor.GetSize(); ip++) {
			CFSWString szWord;
			Suggestor.GetItem(ip, szWord);
			env->SetObjectArrayElement(suggs, ip, FSJNIWtoStr(env, szWord));
		}
		return suggs;
	} catch(...) {
		return 0;
	}
}

} // extern "C"