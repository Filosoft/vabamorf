#include "../../../lib/proof/proof.h"
#include "../../../lib/fsc/fsjni/fsjni.h"
#include "utils.h"

jobject MorphInfoToJNI(JNIEnv *env, const CMorphInfo &MorphInfo) {
	jclass MorphInfoClass = env->FindClass("ee/filosoft/vabamorf/MorphInfo");
	jmethodID MorphInfoClassConstructor = (MorphInfoClass ? env->GetMethodID(MorphInfoClass, "<init>", "()V") : 0);
	if (!MorphInfoClassConstructor) throw CFSException();

	jobject morphInfo = env->NewObject(MorphInfoClass, MorphInfoClassConstructor);
	FSJNISetStringField(env, morphInfo, "root", FSJNIWtoStr(env, MorphInfo.m_szRoot));
	FSJNISetStringField(env, morphInfo, "ending", FSJNIWtoStr(env, MorphInfo.m_szEnding));
	FSJNISetStringField(env, morphInfo, "clitic", FSJNIWtoStr(env, MorphInfo.m_szClitic));
	FSJNISetCharField(env, morphInfo, "pos", MorphInfo.m_cPOS);
	FSJNISetStringField(env, morphInfo, "form", FSJNIWtoStr(env, MorphInfo.m_szForm));

	return morphInfo;
}

CMorphInfo JNIToMorphInfo(JNIEnv *env, jobject morphInfo){
	CMorphInfo MorphInfo;
	MorphInfo.m_szRoot = FSJNIStrtoW(env, FSJNIGetStringField(env, morphInfo, "root"));
	MorphInfo.m_szEnding = FSJNIStrtoW(env, FSJNIGetStringField(env, morphInfo, "ending"));
	MorphInfo.m_szClitic = FSJNIStrtoW(env, FSJNIGetStringField(env, morphInfo, "clitic"));
	MorphInfo.m_cPOS = FSJNIGetCharField(env, morphInfo, "pos");
	MorphInfo.m_szForm = FSJNIStrtoW(env, FSJNIGetStringField(env, morphInfo, "form"));

	return MorphInfo;
}

jobject MorphInfosToJNI(JNIEnv *env, const CMorphInfos &MorphInfos) {
	jclass MorphInfosClass = env->FindClass("ee/filosoft/vabamorf/MorphInfos");
	jmethodID MorphInfosClassConstructor = (MorphInfosClass ? env->GetMethodID(MorphInfosClass, "<init>", "()V") : 0);
	if (!MorphInfosClassConstructor) throw CFSException();

	jobject morphInfos = env->NewObject(MorphInfosClass, MorphInfosClassConstructor);
	FSJNISetStringField(env, morphInfos, "word", FSJNIWtoStr(env, MorphInfos.m_szWord));

	jobjectArray morphInfo = (jobjectArray)env->NewObjectArray(MorphInfos.m_MorphInfo.GetSize(), env->FindClass("java/lang/Object"), 0);
	for (INTPTR ip2 = 0; ip2 < MorphInfos.m_MorphInfo.GetSize(); ip2++) {
		env->SetObjectArrayElement(morphInfo, ip2, MorphInfoToJNI(env, MorphInfos.m_MorphInfo[ip2]));
	}
	FSJNISetObjectField(env, morphInfos, "morphInfo", "[Lee/filosoft/vabamorf/MorphInfo;", morphInfo);

	return morphInfos;
}

CMorphInfos JNIToMorphInfos(JNIEnv *env, jobject morphInfos){
	CMorphInfos MorphInfos;
	MorphInfos.m_szWord = FSJNIStrtoW(env, FSJNIGetStringField(env, morphInfos, "word"));

	jobjectArray morphInfo = (jobjectArray)FSJNIGetObjectField(env, morphInfos, "morphInfo", "[Lee/filosoft/vabamorf/MorphInfo;");
	INTPTR ipSize = env->GetArrayLength(morphInfo);
	for (INTPTR ip=0; ip<ipSize; ip++) {
		MorphInfos.m_MorphInfo.AddItem(JNIToMorphInfo(env, env->GetObjectArrayElement(morphInfo, ip)));
	}

	return MorphInfos;
}