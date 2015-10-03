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
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QApplication>

QString pwd = "ElOB2wAJ!";

int initRequest()
{
    QAndroidJniObject activity =  QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");

    jint success = QAndroidJniObject::callStaticMethod<jint>("org/qtproject/qt5/crypt/Crypter", "init", "(Landroid/app/Activity;)I", activity.object<jobject>());;

    return (int) success;
}

bool testIsKukaytaInstalled()
{
    jboolean installed = QAndroidJniObject::callStaticMethod<jboolean>("org/qtproject/qt5/crypt/Crypter", "isKukaytaInstalled");

    return (bool) installed;
}

void installKukayta()
{
    QAndroidJniObject::callStaticMethod<void>("org/qtproject/qt5/crypt/Crypter","installKukatya");
}

int zipDecrypt(QString zipFilename, QString internalFile, QString target)
{
    QAndroidJniObject zipPath = QAndroidJniObject::fromString(zipFilename);
    QAndroidJniObject intFile = QAndroidJniObject::fromString(internalFile);
    QAndroidJniObject Target = QAndroidJniObject::fromString(target);
    QAndroidJniObject password = QAndroidJniObject::fromString(pwd);

    jint res = QAndroidJniObject::callStaticMethod<jboolean>("org/qtproject/qt5/crypt/Crypter", "zipDecrypt","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",
                                        zipPath.object<jstring>(), intFile.object<jstring>(), Target.object<jstring>(), password.object<jstring>());

    return (int) res;
}




