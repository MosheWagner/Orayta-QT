package download;

public interface IDownloadManager 
{
	public void addDownload(String urlPath, String savePath);
	

	//These register listeners for events of ALL download finishing, and TOTAL progress of ALL downloads
	public void registerFinishedListener(IDownloadListener listener);
	//Progress returns the percentage of ALL downloads. So this will go down when a new download is added.
	public void registerProgressListener(IDownloadListener listener);
	
	
}
