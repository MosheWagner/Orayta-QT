package download;

public interface IDownloadListener 
{
	public void onDownloadFinished();
	public void onDownloadProgress(int percent);
}
