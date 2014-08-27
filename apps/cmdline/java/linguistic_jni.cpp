#include "../../../lib/proof/proof.h"
#include "../../../lib/fsc/fsjni/fsjni.h"

FSDLL_ENTRYPOINT;

#define FUNCTION_HEADER \
	CLinguistic *linguistic = (CLinguistic *)FSJNIGetLongField(env, jobj, "lingId"); \
	if (linguistic) { \
		linguistic->m_bAbbrevations = (FSJNIGetBooleanField(env, jobj, "abbrevations") != JNI_FALSE); \
		linguistic->m_bRomanNumerals = (FSJNIGetBooleanField(env, jobj, "romanNumerals") != JNI_FALSE); \
		linguistic->m_bGuess = (FSJNIGetBooleanField(env, jobj, "guess") != JNI_FALSE); \
		linguistic->m_bPhonetic = (FSJNIGetBooleanField(env, jobj, "phonetic") != JNI_FALSE); \
	}

extern "C" {

JNIEXPORT jboolean JNICALL Java_ee_filosoft_vabamorf_Linguistic_open(JNIEnv *env, jobject jobj, jstring fileName)
{
	FUNCTION_HEADER;

	if (linguistic) {
		FSJNISetLongField(env, jobj, "lingId", (jlong)0);
		IGNORE_FSEXCEPTION( delete linguistic; )
	}

	try {
#if defined (UNICODE)
		CFSFileName fileNameT=FSJNIStrtoW(env, fileName);
#else
		CFSFileName fileNameT=FSJNIStrtoA(env, fileName);
#endif
		linguistic = new CLinguistic();
		linguistic->Open(fileNameT);
		FSJNISetLongField(env, jobj, "lingId", (jlong)linguistic);
		return JNI_TRUE;
	} catch(...) {
		return JNI_FALSE;
	}}

JNIEXPORT void JNICALL Java_ee_filosoft_vabamorf_Linguistic_close(JNIEnv *env, jobject jobj)
{
	FUNCTION_HEADER;

	if (linguistic) {
		FSJNISetLongField(env, jobj, "lingId", (jlong)0);
		IGNORE_FSEXCEPTION( delete linguistic; )
	}
}

// Speller
jboolean JNICALL Java_ee_filosoft_vabamorf_Linguistic_spellWord(JNIEnv *env, jobject jobj, jstring word)
{
	FUNCTION_HEADER;
	if (!linguistic) return NULL;

	try {
		if (linguistic->SpellWord(FSJNIStrtoW(env, word))==SPL_NOERROR) return JNI_TRUE;
		return JNI_FALSE;
	} catch (...) {
		return NULL;
	}
}

jobjectArray JNICALL Java_ee_filosoft_vabamorf_Linguistic_suggest(JNIEnv *env, jobject jobj, jstring word, jboolean startSentence)
{
	FUNCTION_HEADER;
	if (!linguistic) return NULL;

	try {
		CFSWStringArray suggestions = linguistic->Suggest(FSJNIStrtoW(env, word), startSentence!=JNI_FALSE);
		jobjectArray suggs = (jobjectArray)env->NewObjectArray(suggestions.GetSize(), env->FindClass("java/lang/String"), 0);
		for (INTPTR ip=0; ip<suggestions.GetSize(); ip++) {
			env->SetObjectArrayElement(suggs, ip, FSJNIWtoStr(env, suggestions[ip]));
		}
		return suggs;
	} catch(...) {
		return 0;
	}
}

JNIEXPORT jobjectArray JNICALL Java_ee_filosoft_vabamorf_Linguistic_analyze(JNIEnv *env, jobject jobj, jstring word)
{
	FUNCTION_HEADER;
	if (!linguistic) return NULL;

	try {
		jclass MorphInfoClass = env->FindClass("ee/filosoft/vabamorf/Linguistic$MorphInfo");
		jmethodID MorphInfoClassConstructor = (MorphInfoClass ? env->GetMethodID(MorphInfoClass, "<init>", "()V") : 0);
		if (!MorphInfoClassConstructor) return NULL;

		CFSArray<CMorphInfo> results = linguistic->Analyze(FSJNIStrtoW(env, word));
		jobjectArray analyzes=(jobjectArray)env->NewObjectArray(results.GetSize(), env->FindClass("java/lang/Object"), 0);
		for (INTPTR ip=0; ip<results.GetSize(); ip++) {
			jobject analyze1=env->NewObject(MorphInfoClass, MorphInfoClassConstructor);
			FSJNISetStringField(env, analyze1, "root", FSJNIWtoStr(env, results[ip].m_szRoot));
			FSJNISetStringField(env, analyze1, "ending", FSJNIWtoStr(env, results[ip].m_szEnding));
			FSJNISetStringField(env, analyze1, "clitic", FSJNIWtoStr(env, results[ip].m_szClitic));
			FSJNISetCharField(env, analyze1, "pos", results[ip].m_cPOS);
			FSJNISetStringField(env, analyze1, "form", FSJNIWtoStr(env, results[ip].m_szForm));
			env->SetObjectArrayElement(analyzes, ip, analyze1);
		}
		return analyzes;
	} catch(...) {
		return NULL;
	}
}

JNIEXPORT jobjectArray JNICALL Java_ee_filosoft_vabamorf_Linguistic_synthesize(JNIEnv *env, jobject jobj, jobject info, jstring hint)
{
	FUNCTION_HEADER;
	if (!linguistic) return NULL;

	try {
		jclass MorphInfoClass = env->FindClass("ee/filosoft/vabamorf/Linguistic$MorphInfo");
		jmethodID MorphInfoClassConstructor = (MorphInfoClass ? env->GetMethodID(MorphInfoClass, "<init>", "()V") : 0);
		if (!MorphInfoClassConstructor) return NULL;

		CMorphInfo morphInfo;
		morphInfo.m_szRoot = FSJNIStrtoW(env, FSJNIGetStringField(env, info, "root"));
		morphInfo.m_szClitic = FSJNIStrtoW(env, FSJNIGetStringField(env, info, "clitic"));
		morphInfo.m_cPOS = FSJNIGetCharField(env, info, "pos");
		morphInfo.m_szForm = FSJNIStrtoW(env, FSJNIGetStringField(env, info, "form"));

		CFSArray<CMorphInfo> results = linguistic->Synthesize(morphInfo, FSJNIStrtoW(env, hint));

		jobjectArray synths=(jobjectArray)env->NewObjectArray(results.GetSize(), env->FindClass("java/lang/Object"), 0);
		for (INTPTR ip=0; ip<results.GetSize(); ip++) {
			jobject analyze1=env->NewObject(MorphInfoClass, MorphInfoClassConstructor);
			FSJNISetStringField(env, analyze1, "root", FSJNIWtoStr(env, results[ip].m_szRoot));
			FSJNISetStringField(env, analyze1, "ending", FSJNIWtoStr(env, results[ip].m_szEnding));
			FSJNISetStringField(env, analyze1, "clitic", FSJNIWtoStr(env, results[ip].m_szClitic));
			FSJNISetCharField(env, analyze1, "pos", results[ip].m_cPOS);
			FSJNISetStringField(env, analyze1, "form", FSJNIWtoStr(env, results[ip].m_szForm));
			env->SetObjectArrayElement(synths, ip, analyze1);
		}
		return synths;
	} catch(...) {
		return NULL;
	}
}

} // extern "C"
