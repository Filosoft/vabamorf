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

		if (!bIsSpellUpperCase) {
			CFSWString szLower=Word.m_szWord.ToLower();
			CFSWString szUpper=Word.m_szWord.ToUpper();
			if (szUpper==Word.m_szWord && szLower!=Word.m_szWord) return JNI_TRUE;
			pLinguistic->m_bAbbrevations=false;
		}
		else pLinguistic->m_bAbbrevations=true;;

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
		CFSWStringArray Suggestions=pLinguistic->Suggest(Word.m_szWord);
		jobjectArray suggs=(jobjectArray)env->NewObjectArray(Suggestions.GetSize(), env->FindClass("java/lang/String"), 0);
		for (INTPTR ip=0; ip<Suggestions.GetSize(); ip++) {
			env->SetObjectArrayElement(suggs, ip, FSJNIWtoStr(env, Suggestions[ip]));
		}
		return suggs;
	} catch(...) {
		return 0;
	}
}

} // extern "C"