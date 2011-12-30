/** This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 3
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
* Author: abe.izar  <izar00@gmail.com>
* @author abe.izar <izar00@gmail.com>
*/

package org.kde.necessitas.origo;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Environment;

public class CopyResources {

        private AssetManager assetManager;
        private StringBuffer buffer;

        public CopyResources(Activity activity){
                assetManager = activity.getAssets();
                buffer = new StringBuffer();
        }


        /**
         * @param path- the path in the assets folder where the files are located,
         * which will also be the path on the sdcard where files will be put. e.g. "Orayta/Books"
         */
        public void copyAssetsToSDCard(String path){


                //get the sdcard path:
                File sdcard = Environment.getExternalStorageDirectory();
                String target = extendPath(sdcard.toString(), path);
                log("copying resources to: " + target);
//		buffer.append("copied the files:\n");
                try {
//			String[] assets = assetManager.list(path);
//			//copy every file in the given path:
//			for (String file: assets){
//				String from = extendPath(path, file);
//				copyDir(from, target); //will copy both files and directories.
//			}
//			copyDir(path, target);
                        copyDirOrFile(path, sdcard.toString());
                }
                catch (FileAlreadyExistsException e){ //TODO: decide what is the best behaver in this situation.
                        log("we have detected that at least one file already exists and will now stop " +
                                        "copying more files to sdcard.");
                }
                catch (Exception e){
                        log("error copying files\n" + e +"\n"+ e.getMessage());
                }

//		buffer.append("<end file list>");
                log("successfully copied the follwing files:\n" +
                                buffer + "<end file list>\n");


        }
//
//	/**
//	 *
//	 * @param inPath - path to copy from, i.e.: "Books/040_msnh_..."
//	 * @param outPath - path to copy to, i.e.: "/mnt/sdcard/Orayta/Books"
//	 * @throws IOException
//	 */
//	public void copyDir(String inPath,  String outPath) throws IOException {
//
//		String[] list = assetManager.list(inPath);
//		File inDemmyFile = new File(inPath);
//
//		if (list.length == 0) { //no subfolders, this is a file.
//			if (inDemmyFile.exists())
//				log("the file: \"" + inPath+ "\" already exists. skipping...");
//			else
//			copyFile(inPath, outPath);
//		}
//		else {
//			String target = extendPath(outPath, inDemmyFile.getName()); /* i.e.: "/mnt/sdcard/Orayta/Books" + "/" + "040_msnh...*/
//			File directory = new File(target);
//			if (!directory.exists()){
//				log("trying to make dir: " + directory);
//				directory.mkdir();
//			}
//
//			log("in directory: \"" + inDemmyFile.getAbsolutePath() + "\" will now copy childrean");
//			for (String file: list){
//				String inFile = extendPath(inPath, file);
//				String outFile = extendPath(target, file);
//				copyDir(inFile, outFile);
//			}
//		}
//
//	}
        /**
         *
         * @param path - path in assets folder to copy. i.e. "Orayta". the files will be copied to a directory with the same name.
         * @param outputHead - the location where files should be copied to. i.e. "/mnt/sdcard".
         * @throws IOException
         * @throws Exception
         */
        public void copyDirOrFile(String path, String outputHead) throws IOException, Exception{
                String[] list = assetManager.list(path);
                File output = new File(extendPath(outputHead, path));
                if (list.length == 0) { //no subfolders, this is a file.
                        if (output.exists()){
                                log("the file: \"" + output.toString() + "\" already exists. skipping...");
                                throw new FileAlreadyExistsException("file already exists.");
//				return;
                        }
                        else {
                                moveFile(path, output.toString());
                                buffer.append(output.toString() + "\n");
                        }
                }
                else { //if this is a directory
                        if(!output.exists()){
//				log("trying to make dir: " + output);
                                output.mkdir();
                        }
//			log("in directory: \"" + path + "\" will now copy childrean");
                        for (String file: list){
                                String in = extendPath(path, file);
                                copyDirOrFile(in, outputHead);
                        }

                }
        }

        public void moveFile(String inPath, String outPath) throws IOException {
                InputStream inStream;
                OutputStream outStream;
                try {
//			log("trying to open file: " + inPath);
                        inStream = assetManager.open(inPath);
                } catch (Exception e ){
                        log("error copying file:" + inPath +"\nis it larger then 1MB?\n" + e +"\n"+ e.getMessage());
                        return;
                }
//		log("file opened. OPStream... " + outPath);

                try{
                        outStream = new FileOutputStream(outPath);

//			log("trying to write to: " + outPath);
                        byte[] buffer = new byte[1024];
                        int count;
                        while((count = inStream.read(buffer)) != -1){
                                outStream.write(buffer, 0, count);
                        }
                        inStream.close();
                        inStream = null;
                        outStream.flush();
                        outStream.close();
                        outStream= null;

                        File file = new File(inPath);
                        file.delete();
                } catch (Exception e ){
                        log("error saving file to: " + outPath +" is it larger then 1MB?\n" + e +"\n"+ e.getMessage());
                        return;
                }
        }

        private String extendPath(String str1, String str2) {
                String head = new String (str1);
                String tail = new String (str2);

                head = trim (head);
                String res = head + File.separator + tail;
//		log("conected: " +res);
                return res;
        }

        /**
         * automatic method which will copy all resources from "assets/Orayta/" to
         * the path "Orayta" on the sdcard.
         * @param activity the activity that calls this method. usually simply "this".
         */
        public static void CopyOraytaResources(Activity activity){

                CopyResources copyer = new CopyResources(activity);
                        String out = "Orayta";
                        copyer.copyAssetsToSDCard(out);
                        //System.err.println("successfuly copied!");


        }

        /**
         * removes a trailing path separator from a path.
         * @param str - string to trim e.g. "/mnt/sdcard/Orayta/"
         * @return the trimed string e.g. "/mnt/sdcard/Orayta"
         */
        private String trim(String str) {
                String path = new String(str);
                if(path.endsWith(File.separator)) { // make sure we don't have double separators.
                        log ("trimming: " + path);
                        path = path.substring(0, path.length()-1);
                        log ("to: " + path);
                }
                return path;

        }

        private static void log(String string) {
                // TODO Auto-generated method stub
                System.err.println(string);
        }

        private class FileAlreadyExistsException extends Exception {

                /**
                 *
                 */
                private static final long serialVersionUID = -6271028177511149852L;

                FileAlreadyExistsException(String str){
                        super(str);
                }
        }
}
