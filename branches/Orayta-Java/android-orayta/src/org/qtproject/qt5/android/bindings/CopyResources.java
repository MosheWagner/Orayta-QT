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

package org.qtproject.qt5.android.bindings;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.Environment;

public class CopyResources {

	private AssetManager assetManager;
	private Activity mActivity;
	private StringBuffer buffer;
	private final String UNKNOWN = "UNKNOWN";
	private final String VERSION_PATH = ".version";
	public static final File SDCARD = Environment.getExternalStorageDirectory();
//	private File HOME;

	public CopyResources(Activity activity) {
		mActivity = activity;
		assetManager = activity.getAssets();
		buffer = new StringBuffer();
		
//		HOME = mActivity.getFilesDir();		
	}

	/**
	 * @param location - where to put the files i.e. "/sdcard".
	 * 
	 * @param path - a dir name to put the files in, and the dir name inside assets i.e. "Orayta".
	 */
	public void copyAssetsGroup(String location, String path) {
		
		// the full path to target files.
		String fullPath = extendPath(location, path);
		
		// make sure our path exists
		File fullPathFile = new File (fullPath);
		if (! fullPathFile.exists())
			fullPathFile.mkdirs();

		
		
		File versionFile = checkVersion(fullPath);
		if (versionFile == null) {
			log("installed version is the same as current version. nothing to do.");
			
			return;
		}
		
		// remove old version files :
		File versionDir = new File(extendPath(location, path,VERSION_PATH));
		if (versionDir.exists())
			removeChildrean(versionDir);
		log("making dir: " + versionDir.toString());
		versionDir.mkdirs();

		// get the sdcard path:
		String target = fullPath;
		log("copying resources to: " + target);
		
		try {
			
			copyDirOrFile(path, location);

			// if everything went well, create a new version file so that we can
			// remember the process was done.
			log("making file: " + versionFile.toString());
			try {
				versionFile.createNewFile();
			} catch (IOException e) {
				log("can't create version file");
				e.printStackTrace();
			}

		}
		catch (IOException e){
			log("error copying file:"+"\n" + e + "\n" + e.getMessage());
			e.printStackTrace();
		}
		catch (Exception e) {
			log("error copying files\n" + e + "\n" + e.getMessage());
		}

		// buffer.append("<end file list>");
		log("successfully copied the follwing files:\n" + buffer
				+ "<end file list>\n");
		buffer.setLength(0);
		
		

	}

	/**
	 * deletes all files in a dir (but not the dir itself).
	 * @param dir
	 */
	private void removeChildrean(File dir) {
		File[] childran = dir.listFiles();
		for (File child: childran){
			if (child.isDirectory())
				removeChildrean(child);
			child.delete();
		}
	}

	/**
	 * check if the installed files are from the same version as current.
	 * 
	 * @param path
	 *            - path to check for installed version.
	 * @return null if nothing to be done, or a filename
	 */
	private File checkVersion(String path) {

		String version = getVersion();
		if (version.equals(UNKNOWN)) // cant get version, we need to act as
										// though it isn't same as currnet.
			return null;

		
		String versionFileName = extendPath(path,
				VERSION_PATH, version);
		File versionFile = new File(versionFileName);
		if (versionFile.exists())
			return null;
		else
			return versionFile;

	}

	
	/**
	 * 
	 * @param path
	 *            - path in assets folder to copy. i.e. "Orayta". the files will
	 *            be copied to a directory with the same name.
	 * @param outputHead
	 *            - the location where files should be copied to. i.e.
	 *            "/mnt/sdcard".
	 * @throws IOException
	 * @throws Exception
	 */
	public void copyDirOrFile(String path, String outputHead)
			throws IOException, Exception {
		String[] list = assetManager.list(path);
		File output = new File(extendPath(outputHead, path));
		if (list.length == 0) { // no subfolders, this is a file.
			// we don't care if file already exists, we will overwrite it.
			moveFile(path, output.toString());
			buffer.append(output.toString() + "\n");
		} else { // if this is a directory
			if (!output.exists()) {
				output.mkdir();
			}
			for (String file : list) {
				String in = extendPath(path, file);
				copyDirOrFile(in, outputHead);
			}

		}
	}

	/**
	 * copy a file
	 * 
	 * @param inPath
	 *            - where file is now
	 * @param outPath
	 *            - we want it to be
	 * @throws IOException
	 */
	public void moveFile(String inPath, String outPath) throws IOException {
		InputStream inStream;
		OutputStream outStream;
		try {
			inStream = assetManager.open(inPath);
		} catch (Exception e) {
			log("error copying file:" + inPath + "\nis it larger then 1MB?\n"
					+ e + "\n" + e.getMessage());
			return;
		}

		try {
			outStream = new FileOutputStream(outPath);

			byte[] buffer = new byte[1024];
			int count;
			while ((count = inStream.read(buffer)) != -1) {
				outStream.write(buffer, 0, count);
			}
			inStream.close();
			inStream = null;
			outStream.flush();
			outStream.close();
			outStream = null;

			File file = new File(inPath);
			file.delete();
		} catch (Exception e) {
			log("error saving file to: " + outPath
					+ " is it larger then 1MB?\n" + e + "\n" + e.getMessage());
			return;
		}
	}

	
	private String extendPath(String... strings) {
		String res = strings[0];
		for (int i= 1; i<strings.length; i++) {
			res = res + File.separator + trim(strings[i]);
		}
		return res;
	}

	private String getVersion() {

		String version = UNKNOWN;
		try {
			version = mActivity.getPackageManager().getPackageInfo(
					mActivity.getPackageName(), 0).versionName;
		} catch (PackageManager.NameNotFoundException e) {
			log("can't find version");
		}
		return version;
	}

	/**
	 * removes a trailing path separator from a path.
	 * 
	 * @param str
	 *            - string to trim e.g. "/mnt/sdcard/Orayta/"
	 * @return the trimed string e.g. "/mnt/sdcard/Orayta"
	 */
	private String trim(String str) {
		String path = new String(str);
		if (path.endsWith(File.separator)) { // make sure we don't have double
												// separators.
			log("trimming: " + path);
			path = path.substring(0, path.length() - 1);
			log("to: " + path);
		}
		return path;

	}

	private static void log(String string) {
		System.err.println(string);
	}

	
}
