package download;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.file.Files;
import java.util.Collection;
import java.util.LinkedList;

/*
 * Downloads a single file. 
 * NOTE: This class blocks execution until the download finishes, so it must be called in a new thread 
 */

public class SimpleFileDownloader implements Runnable, IFileDownloader
{
	Collection<IDownloadListener> listeners = new LinkedList<IDownloadListener>();
	
	String mDownloadPath;
	String mSavePath;
	String origSavePath;
	boolean mOverWrite;
	
	Thread thread = null;

	public void downloadNewThread(String urlPath, String filePath,
			boolean overWrite) 
	{
		mDownloadPath = urlPath;
		mSavePath = filePath;
		mOverWrite = overWrite;
		
		thread = new Thread(this);
		thread.start();
	}
	
	public void run()
	{
		try {
			downloadFile();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
	public void downloadFile() throws IOException
	{
		addSuffix();
		
		URL downloadUrl = new URL(mDownloadPath);
		
		File downloadFile = new File(mSavePath);
		if (downloadFile.exists())
		{
			if (mOverWrite == false) return;
			
			//else:
			
			//Will throw an IOException if fails
			downloadFile.delete();
		}
		
		ReadableByteChannel rbc;
		rbc = Channels.newChannel(downloadUrl.openStream());
		
        FileOutputStream fos = new FileOutputStream(downloadFile);
        fos.getChannel().transferFrom(rbc, 0, Long.MAX_VALUE);
        fos.close();
        rbc.close();
        
        downloadFinished();
	}

	private void downloadFinished() throws IOException 
	{
		removeSuffix();
		
		//Tell everyone we are done
		for (IDownloadListener l:listeners)
		{
			l.onDownloadFinished();
		}
	}

	private void addSuffix() {
		//Add ".download" to the filename
		origSavePath = mSavePath;
		mSavePath = mSavePath + ".download";
	}
	
	private void removeSuffix() throws IOException  {
		//Remove the ".download" from the filename
		File oldFile = new File(mSavePath);
		File newFile = new File(origSavePath);
		Files.move(oldFile.toPath(), newFile.toPath());
	}

	public void registerProgressListener(IDownloadListener iListener) {
		// TODO Auto-generated method stub

	}

	public void registerFinishedListener(IDownloadListener iListener) {
		listeners.add(iListener);
	}

	public DownloadStatus getStatus() 
	{
		if (thread == null) return DownloadStatus.NOT_STARTED_YET;
		else if(thread.isAlive()) return DownloadStatus.ACTIVE;
		else return DownloadStatus.FINISHED_OK;
	}

	public long getDownloadSize() {
		// TODO Auto-generated method stub
		return -1;
	}

	public int getProgress() {
		// TODO Auto-generated method stub
		return -1;
	}
}
