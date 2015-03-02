package book;

import search.ISearchQuery;
import search.ISearchResults;

/*
 * THE Book class.
 * From this class you can get a chpter's text (given it's address),
 *  get a tree of the chapters,
 *  and much more.
 */

public abstract class Book 
{
	IBookMetaData metaData;
	String rawText;
	
	public abstract IChapter getChapter(ChapterID address);
	
	public abstract BookID getUID(); 
	
	//public abstract Boolean buildFromFile(File file);
	public abstract Boolean buildFromRawText(String rawText);
	
	public abstract ChapterList getChapterList();
	//public abstract TreeNode<IChapter> getChapterTree();
	//public abstract HashMap<K, V> chapterAddressMap;
	
	public abstract ISearchResults search(ISearchQuery query);
}
