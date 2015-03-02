package download;

public interface IFileDownloader 
{
	public enum DownloadStatus
	{
		NOT_STARTED_YET, ACTIVE, FINISHED_OK, FINISHED_ERROR/*, PUASED, CANCELED*/;
	}
	
	public void downloadNewThread(String urlPath, String filePath, boolean overWrite);
	
	public void registerProgressListener(IDownloadListener listener);
	public void registerFinishedListener(IDownloadListener listener);
	
	public DownloadStatus getStatus();
	public int getProgress();
	public long getDownloadSize();
}
