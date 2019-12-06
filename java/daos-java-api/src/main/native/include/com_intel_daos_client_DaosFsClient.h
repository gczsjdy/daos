/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_intel_daos_client_DaosFsClient */

#ifndef _Included_com_intel_daos_client_DaosFsClient
#define _Included_com_intel_daos_client_DaosFsClient
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    move
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_move
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    mkdir
 * Signature: (JLjava/lang/String;IZ)V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_mkdir
  (JNIEnv *, jobject, jlong, jstring, jint, jboolean);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    createNewFile
 * Signature: (JLjava/lang/String;Ljava/lang/String;IIII)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_createNewFile
  (JNIEnv *, jobject, jlong, jstring, jstring, jint, jint, jint, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    delete
 * Signature: (JLjava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_daos_client_DaosFsClient_delete
  (JNIEnv *, jobject, jlong, jstring, jstring);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    daosCreatePool
 * Signature: (Ljava/lang/String;IIJJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_daos_client_DaosFsClient_daosCreatePool
  (JNIEnv *, jclass, jstring, jint, jint, jlong, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    daosCreateContainer
 * Signature: (JLjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_daos_client_DaosFsClient_daosCreateContainer
  (JNIEnv *, jclass, jlong, jstring);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    daosOpenPool
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_daosOpenPool
  (JNIEnv *, jclass, jstring, jstring, jstring, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    daosOpenCont
 * Signature: (JLjava/lang/String;I)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_daosOpenCont
  (JNIEnv *, jclass, jlong, jstring, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    daosCloseContainer
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_daosCloseContainer
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    daosClosePool
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_daosClosePool
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsSetPrefix
 * Signature: (JLjava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_intel_daos_client_DaosFsClient_dfsSetPrefix
  (JNIEnv *, jobject, jlong, jstring);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsLookup
 * Signature: (JJLjava/lang/String;IJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsLookup__JJLjava_lang_String_2IJ
  (JNIEnv *, jobject, jlong, jlong, jstring, jint, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsLookup
 * Signature: (JLjava/lang/String;Ljava/lang/String;IJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsLookup__JLjava_lang_String_2Ljava_lang_String_2IJ
  (JNIEnv *, jobject, jlong, jstring, jstring, jint, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsGetSize
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsGetSize
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsDup
 * Signature: (JJI)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsDup
  (JNIEnv *, jobject, jlong, jlong, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsRelease
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsRelease
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsRead
 * Signature: (JJJJJI)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsRead
  (JNIEnv *, jobject, jlong, jlong, jlong, jlong, jlong, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsWrite
 * Signature: (JJJJJI)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsWrite
  (JNIEnv *, jobject, jlong, jlong, jlong, jlong, jlong, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsReadDir
 * Signature: (JJI)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_intel_daos_client_DaosFsClient_dfsReadDir
  (JNIEnv *, jobject, jlong, jlong, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsOpenedObjStat
 * Signature: (JJJ)V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_dfsOpenedObjStat
  (JNIEnv *, jobject, jlong, jlong, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsSetExtAttr
 * Signature: (JJLjava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_dfsSetExtAttr
  (JNIEnv *, jobject, jlong, jlong, jstring, jstring, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsGetExtAttr
 * Signature: (JJLjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_daos_client_DaosFsClient_dfsGetExtAttr
  (JNIEnv *, jobject, jlong, jlong, jstring);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsRemoveExtAttr
 * Signature: (JJLjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_intel_daos_client_DaosFsClient_dfsRemoveExtAttr
  (JNIEnv *, jobject, jlong, jlong, jstring);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsGetChunkSize
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsGetChunkSize
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsGetMode
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_intel_daos_client_DaosFsClient_dfsGetMode
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsIsDirectory
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_intel_daos_client_DaosFsClient_dfsIsDirectory
  (JNIEnv *, jclass, jint);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsMountFs
 * Signature: (JJZ)J
 */
JNIEXPORT jlong JNICALL Java_com_intel_daos_client_DaosFsClient_dfsMountFs
  (JNIEnv *, jclass, jlong, jlong, jboolean);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    dfsUnmountFs
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_dfsUnmountFs
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_intel_daos_client_DaosFsClient
 * Method:    daosFinalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_intel_daos_client_DaosFsClient_daosFinalize
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
