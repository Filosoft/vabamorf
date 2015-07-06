#if !defined(_JNI_UTILS_ADSF124ADF_)
#define _JNI_UTILS_ADSF124ADF_

jobject MorphInfoToJNI(JNIEnv *env, const CMorphInfo &MorphInfo);
CMorphInfo JNIToMorphInfo(JNIEnv *env, jobject morphInfo);

jobject MorphInfosToJNI(JNIEnv *env, const CMorphInfos &MorphInfos);
CMorphInfos JNIToMorphInfos(JNIEnv *env, jobject morphInfos);

#endif // _JNI_UTILS_ADSF124ADF_