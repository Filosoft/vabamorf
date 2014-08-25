#include "../../../lib/proof/proof.h"
#include "../../../lib/fsc/fsjni/fsjni.h"

FSDLL_ENTRYPOINT;

#define FUNCTION_HEADER ETMRFAS *mrf=(ETMRFAS *)FSJNIGetLongField(env, jobj, "morphId");

extern "C" {

JNIEXPORT jboolean JNICALL Java_ee_filosoft_vabamorf_Morph_init(JNIEnv *env, jobject jobj, jstring fileName)
{
	FUNCTION_HEADER;

	if (mrf) {
		FSJNISetLongField(env, jobj, "morphId", (jlong)0);
		try { delete mrf; } catch(...) { }
	}

	try {
#if defined (UNICODE)
		CFSFileName fileNameT=FSJNIStrtoW(env, fileName);
#else
		CFSFileName fileNameT=FSJNIStrtoA(env, fileName);
#endif
		mrf=new ETMRFAS(0, fileNameT, FSTSTR(""));
		FSJNISetLongField(env, jobj, "morphId", (jlong)mrf);
		return JNI_TRUE;
	} catch(...) {
		return JNI_FALSE;
	}}

JNIEXPORT void JNICALL Java_ee_filosoft_vabamorf_Morph_terminate(JNIEnv *env, jobject jobj)
{
	FUNCTION_HEADER;

	if (mrf) {
		FSJNISetLongField(env, jobj, "morphId", (jlong)0);
		try { delete mrf; } catch(...) { }
	}
}

JNIEXPORT jobjectArray JNICALL Java_ee_filosoft_vabamorf_Morph_analyze(JNIEnv *env, jobject jobj, jstring word)
{
	FUNCTION_HEADER;
	if (!mrf) return NULL;

	try {
		CFSWString word2=FSJNIStrtoW(env, word);
		if (word2.IsEmpty()) return NULL;

		jclass MorphInfoClass = env->FindClass("ee/filosoft/vabamorf/Morph$MorphInfo");
		jmethodID MorphInfoClassConstructor = (MorphInfoClass ? env->GetMethodID(MorphInfoClass, "<init>", "()V") : 0);
		if (!MorphInfoClassConstructor) return NULL;

		MRF_FLAGS_BASE_TYPE flags=MF_DFLT_MORFA;
		if (FSJNIGetBooleanField(env, jobj, "guess")) flags|=MF_OLETA;
		if (FSJNIGetBooleanField(env, jobj, "phon")) flags|=MF_KR6NKSA;

		mrf->SetFlags(flags);
		mrf->SetMaxTasand();
		mrf->Clr();
		mrf->Set1(word2);
		LYLI lyli;
		jobjectArray analyzes=NULL;

		if (mrf->Flush(lyli)) {
			lyli.ptr.pMrfAnal->StrctKomadLahku();
			analyzes=(jobjectArray)env->NewObjectArray(lyli.ptr.pMrfAnal->idxLast, env->FindClass("java/lang/Object"), 0);
			for (INTPTR ipTul=0; ipTul<lyli.ptr.pMrfAnal->idxLast; ipTul++){
				MRFTUL tul=*(*lyli.ptr.pMrfAnal)[(int)ipTul];
				jobject analyze1=env->NewObject(MorphInfoClass, MorphInfoClassConstructor);
				FSJNISetStringField(env, analyze1, "root", FSJNIWtoStr(env, tul.tyvi));
				FSJNISetStringField(env, analyze1, "ending", FSJNIWtoStr(env, tul.lopp));
				FSJNISetStringField(env, analyze1, "clitic", FSJNIWtoStr(env, tul.kigi));
				FSJNISetCharField(env, analyze1, "pos", tul.sl[0]);
				CFSWString form=tul.vormid; form.TrimRight(L", ");
				FSJNISetStringField(env, analyze1, "form", FSJNIWtoStr(env, form));
				env->SetObjectArrayElement(analyzes, ipTul, analyze1);
			}
		} else {
			analyzes=(jobjectArray)env->NewObjectArray(0, env->FindClass("java/lang/Object"), 0);
		}

		return analyzes;

	} catch(...) {
		return NULL;
	}
}

JNIEXPORT jobjectArray JNICALL Java_ee_filosoft_vabamorf_Morph_synthesize(JNIEnv *env, jobject jobj, jobject info)
{
	FUNCTION_HEADER;
	if (!mrf) return NULL;

	try {
		MRFTUL tul;
		tul.tyvi=FSJNIStrtoW(env, FSJNIGetStringField(env, info, "root"));
		tul.sl=FSJNIGetCharField(env, info, "pos");
		if (tul.sl.IsEmpty()) tul.sl=L"*";
		tul.vormid=FSJNIStrtoW(env, FSJNIGetStringField(env, info, "form"));

		MRF_FLAGS_BASE_TYPE flags=MF_DFLT_GEN;
		if (FSJNIGetBooleanField(env, jobj, "guess")) flags|=MF_OLETA;
		if (FSJNIGetBooleanField(env, jobj, "phon")) flags|=MF_KR6NKSA;

		mrf->SetFlags(flags);
		mrf->SetMaxTasand();
		mrf->Clr();

		MRFTULEMUSED tulemused;
		jobjectArray synths=NULL;

		if (mrf->Synt(tulemused, tul, L"") && tulemused.on_tulem()){
			synths=(jobjectArray)env->NewObjectArray(tulemused.idxLast, env->FindClass("java/lang/String"), 0);
			for (INTPTR ip=0; ip<tulemused.idxLast; ip++){
				env->SetObjectArrayElement(synths, ip, FSJNIWtoStr(env, tulemused[ip]->tyvi+tulemused[ip]->lopp+tulemused[ip]->kigi));
			}
		} else {
			synths=(jobjectArray)env->NewObjectArray(0, env->FindClass("java/lang/String"), 0);
		}

		return synths;

	} catch(...) {
		return NULL;
	}
}

} // extern "C"
