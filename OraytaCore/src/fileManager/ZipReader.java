package fileManager;
import java.io.FileNotFoundException;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class ZipReader implements IFileReader {

	String zipPath = "";
	
	public ZipReader(String zipFilePath)
	{
		zipPath = zipFilePath;
	}
	
	public String readContents(String filePath) throws IOException, FileNotFoundException 
	{
		String contents = "";
		
		String innerPath = filePath;
		if (filePath.startsWith(zipPath + "/")) innerPath = filePath.replaceFirst(zipPath + "/", "");
		
		// create a buffer to improve copy performance later.
        byte[] buffer = new byte[2048];
 
        // open the zip file stream
        InputStream theFile;
		theFile = new FileInputStream(zipPath);
        ZipInputStream stream = new ZipInputStream(theFile);

        // now iterate through each item in the stream. The get next
        // entry call will return a ZipEntry for each file in the
        // stream
        ZipEntry entry;
		while((entry = stream.getNextEntry()) != null)
		{
			if (entry.getName().equals(innerPath))
			{
				while ( stream.read(buffer) > 0)
				{
				    contents += new String(buffer, "UTF-8");
				}
			}
		}
        // Close the zip file.
        stream.close();

		return contents;
	}

}
