package manager;

import download.DownloadManager;
import book.BookID;
import settings.GeneralSettings;
import tree.TreeNode;

/*
 * This class handles everything.
 * It holds the settings, the booklist, 
 */

public abstract class OraytaCoreManager implements UIBridge, IOraytaCore
{
	private GeneralSettings generalSettings;
	private TreeNode<BookID> bookList;
	
	public abstract DownloadManager downloadManager();
}
