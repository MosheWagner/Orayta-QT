/* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* Author: Avi Izar <izar00@gmail.com>
*/

/*
 * purpose: this file will hold usefull functions for comunicating with java part of the android app
 */

#include "jnifunc.h"

static JavaVM* s_javaVM = 0;
static jclass s_crypterClassID = 0;
static jmethodID s_crypterConstructorMethodID=0;
static jmethodID s_crypterDecryptMethodID=0;
static jmethodID s_crypterZipDecryptMethodID=0;
static jmethodID s_crypterRequestMethodID=0;
static jmethodID s_crypterInstallKukaytaMethodID=0;
static jmethodID s_crypterIsKukaytaInsatlledMethodID=0;
static jmethodID s_crytperTestMethodID=0;


//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent), ui(new Ui::MainWindow)
//{
//    ui->setupUi(this);
//    {
//            qDebug()<<"helo wrld";
//    }

//    JNIEnv *env;

//    if (s_javaVM->AttachCurrentThread(&env, NULL)<0){

//        qCritical()<<"no attach";
//        return;
//    }

//    m_crypterObject = env->NewGlobalRef(env->NewObject(s_crypterClassID, s_crypterConstructorMethodID));
//    if (!m_crypterObject){
//        qDebug()<<"cant connect";
//        return;
//    }

//    qDebug()<< decrypt ("blblaאבג$2");


//    s_javaVM->DetachCurrentThread();
//}

//QString decrypt(QString str){
//    JNIEnv *env;

//    if (s_javaVM->AttachCurrentThread(&env, NULL)<0){

//        qCritical()<<"no attach";
//        return QString();
//    }


//    jstring jstr= env->NewString(reinterpret_cast<const jchar*>(str.constData()), str.length());
//    jstring res_jstr= (jstring)env->CallStaticObjectMethod(s_crypterClassID, s_crypterDecryptMethodID, jstr);
//    env->DeleteLocalRef(jstr);

//    QString* p_res = jstringToQString(env, res_jstr);

//    s_javaVM->DetachCurrentThread();

//    return *p_res;

//}

jstring qstringToJstring(JNIEnv* env, QString str){
    if (!env)
        return NULL;
    jstring jstr= env->NewString(reinterpret_cast<const jchar*>(str.constData()), str.length());
//    env->DeleteLocalRef(jstr);
    return jstr;
}

QString* jstringToQString(JNIEnv* env, jstring str){

          const jchar *     _jchar_str;
          QString* qstring= new QString();

          if (str == 0L) {
                return qstring;
          }

          _jchar_str = env->GetStringChars(str, 0);
          //maybe:
//          _jchar_str = env->GetStringUTFChars(str, 0);

//          if (_bigEndianUnicode) {
                qstring->setUnicode((QChar *) _jchar_str, env->GetStringLength(str));
//          } else {
//                qstring->setUnicodeCodes((const ushort *) _jchar_str, env->GetStringLength(str));
//          }

          env->ReleaseStringChars(str, _jchar_str);

          return qstring;

}

QString pwd(){
    QString str= "#FE$Ada4";
    return str;
}

int zipDecrypt(QString zipFilename, QString internalFile, QString target){
    JNIEnv *env;

    if (s_javaVM->AttachCurrentThread(&env, NULL)<0){

        qCritical()<<"no attach";
        return 0;
    }


    int res = 0;

    jstring zfn = qstringToJstring(env, zipFilename) ;
    jstring inf = qstringToJstring(env, internalFile) ;
    jstring trgt = qstringToJstring(env, target) ;
    jstring crpt = qstringToJstring(env, "ElOB2wAJ!") ;

    qDebug()<<" calling zip decrypt";

    res= env->CallStaticIntMethod(s_crypterClassID, s_crypterZipDecryptMethodID, zfn, inf, trgt, crpt);

    //TODO!
    //        env->DeleteLocalRef(jstr);


    s_javaVM->DetachCurrentThread();

    return res;
}

int initRequest(){
    JNIEnv *env;

    if (s_javaVM->AttachCurrentThread(&env, NULL)<0){

        qCritical()<<"no attach";
        return 0;
    }

    int res = 0;


    qDebug()<<" calling serivce request";

    res= env->CallStaticIntMethod(s_crypterClassID, s_crypterRequestMethodID);


    s_javaVM->DetachCurrentThread();

    return res;
}

bool isKukaytaInstalled2(){
    JNIEnv *env;

    if (s_javaVM->AttachCurrentThread(&env, NULL)<0){

        qCritical()<<"no attach";
        return false;
    }

    bool res = false;

    int tmp;
    tmp = env->CallStaticIntMethod(s_crypterClassID, s_crypterIsKukaytaInsatlledMethodID);
    qDebug()<< "result: " <<tmp;

    if(tmp>0)
        res=true;

    s_javaVM->DetachCurrentThread();

    return res;
}

void installKukayta(){
    JNIEnv *env;

    if (s_javaVM->AttachCurrentThread(&env, NULL)<0){

        qCritical()<<"no attach";
        return;
    }


    env->CallStaticVoidMethod(s_crypterClassID, s_crypterInstallKukaytaMethodID);

    s_javaVM->DetachCurrentThread();

}



JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void*){

    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK){
        qCritical()<<"cant get env";
        return -1;
    }

    s_javaVM=vm;

    jclass clazz= env->FindClass("org/kde/necessitas/origo/Crypter");
    if (!clazz){
        qCritical()<<"cant find class";
        return -1;
    }

    s_crypterClassID = (jclass)env->NewGlobalRef(clazz);

//    s_crypterConstructorMethodID = env->GetMethodID(s_crypterClassID, "<init>", "()V");
//    if(!s_crypterConstructorMethodID){
//        qCritical()<<"cant find constucter";
//        return -1;
//    }

//    s_crypterDecryptMethodID = env->GetStaticMethodID(s_crypterClassID, "decrypt", "(Ljava/lang/String;)Ljava/lang/String;");
////    s_crypterDecryptMethodID = env->GetMethodID(s_crypterClassID, "decrypt", "(Ljava/lang/String;)V");
//    if (!s_crypterDecryptMethodID){
//        qCritical()<<"cant find decrypt()";
//        return -1;
//    }

    s_crypterZipDecryptMethodID = env->GetStaticMethodID(s_crypterClassID, "zipDecrypt",
                                                         "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if (!s_crypterZipDecryptMethodID){
        qCritical()<<"cant find zipDecrypt()";
        return -1;
    }

    s_crypterRequestMethodID = env->GetStaticMethodID(s_crypterClassID, "decryptRequest", "()I");
    if (!s_crypterRequestMethodID){
        qCritical()<<"cant find reaquest decrypt()";
        return -1;
    }

    s_crypterInstallKukaytaMethodID = env->GetStaticMethodID(s_crypterClassID, "installKukatya", "()V");
    if (!s_crypterInstallKukaytaMethodID){
        qCritical()<<"cant find install kukayta()";
        return -1;
    }

    s_crypterIsKukaytaInsatlledMethodID = env->GetStaticMethodID(s_crypterClassID, "isKukaytaInstalled", "()I");
    if (!s_crypterIsKukaytaInsatlledMethodID){
        qCritical()<<"cant find isKukaytaInstalled()";
        return -1;
    }

    s_crytperTestMethodID = env->GetStaticMethodID(s_crypterClassID, "testInt", "()I");
    if (! s_crytperTestMethodID){
        qCritical()<<"cant find testInt()";
        return -1;
    }


    qDebug()<<"Yahooo !";
       return JNI_VERSION_1_6;

}

