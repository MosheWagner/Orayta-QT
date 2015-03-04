package fileManager;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

public class ZipReader implements IFileReader {

	String zipPath = "";
	
	ZipFile zFile;
	
	public ZipReader(String zipFilePath) 
	{
		zipPath = zipFilePath;
		
	}
	
	public static String readComment(String zipFilePath)
	{
		ZipFile zipFile;
		try {
			zipFile = new ZipFile(zipFilePath);
			String comment = zipFile.getComment();
			zipFile.close();
			
			return comment;
		} catch (Exception e) {
			// TODO Auto-generated catch block
			return "";
		}

	}
	
	
	public String readContents(String filePath) throws IOException, FileNotFoundException 
	{
		String contents = "";
		
		zFile = new ZipFile(zipPath);
		
		String innerPath = filePath;
		if (filePath.startsWith(zipPath + "/")) innerPath = filePath.replaceFirst(zipPath + "/", "");
		
        ZipEntry entry = zFile.getEntry(innerPath);
        
        InputStream inStream = zFile.getInputStream(entry);
        
        // create a buffer to improve copy performance later.
        byte[] buffer = new byte[2048];

		while ( inStream.read(buffer) > 0)
		{
		    contents += new String(buffer, "UTF-8");
		}

        // Close the zip file.
		inStream.close();

		return contents;
	}

}
