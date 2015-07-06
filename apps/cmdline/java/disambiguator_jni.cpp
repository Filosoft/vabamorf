#include "../../../lib/proof/proof.h"
#include "../../../lib/fsc/fsjni/fsjni.h"
#include "utils.h"

FSDLL_ENTRYPOINT;

#define FUNCTION_HEADER \
	CDisambiguator *disambiguator = (CDisambiguator *)FSJNIGetLongField(env, jobj, "objId");

extern "C" {

JNIEXPORT jboolean JNICALL Java_ee_filosoft_vabamorf_Disambiguator_open(JNIEnv *env, jobject jobj, jstring fileName)
{
	FUNCTION_HEADER;

	if (disambiguator) {
		FSJNISetLongField(env, jobj, "objId", (jlong)0);
		IGNORE_FSEXCEPTION( delete disambiguator; )
	}

	try {
#if defined (UNICODE)
		CFSFileName fileNameT=FSJNIStrtoW(env, fileName);
#else
		CFSFileName fileNameT=FSJNIStrtoA(env, fileName);
#endif
		disambiguator = new CDisambiguator();
		disambiguator->Open(fileNameT);
		FSJNISetLongField(env, jobj, "objId", (jlong)disambiguator);
		return JNI_TRUE;
	} catch(...) {
		return JNI_FALSE;
	}}

JNIEXPORT void JNICALL Java_ee_filosoft_vabamorf_Disambiguator_close(JNIEnv *env, jobject jobj)
{
	FUNCTION_HEADER;

	if (disambiguator) {
		FSJNISetLongField(env, jobj, "objId", (jlong)0);
		IGNORE_FSEXCEPTION( delete disambiguator; )
	}
}

JNIEXPORT jobjectArray JNICALL Java_ee_filosoft_vabamorf_Disambiguator_disambiguate(JNIEnv *env, jobject jobj, jobjectArray analysis)
{
	FUNCTION_HEADER;
	if (!disambiguator) return NULL;

	try {
		CFSArray<CMorphInfos> Analysis;
		INTPTR ipSize = env->GetArrayLength(analysis);
		for (INTPTR ip = 0; ip < ipSize; ip++) {
			Analysis.AddItem(JNIToMorphInfos(env, env->GetObjectArrayElement(analysis, ip)));
		}

		CFSArray<CMorphInfos> MorphInfos = disambiguator->Disambiguate(Analysis);

		jobjectArray analyzes = (jobjectArray)env->NewObjectArray(MorphInfos.GetSize(), env->FindClass("java/lang/Object"), 0);
		for (INTPTR ip = 0; ip < MorphInfos.GetSize(); ip++) {
			env->SetObjectArrayElement(analyzes, ip, MorphInfosToJNI(env, MorphInfos[ip]));
		}
		return analyzes;
	} catch (...) {
		return NULL;
	}
}

} // extern "C"
