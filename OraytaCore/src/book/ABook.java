package book;

import java.util.HashMap;
import java.util.Map;

import search.ISearchQuery;
import search.ISearchResults;
import tree.IHasID;

/*
 * THE Book class.
 * From this class you can get a chpter's text (given it's address),
 *  get a tree of the chapters,
 *  and much more.
 */

public abstract class ABook implements Comparable<ABook>, IHasID
{
	protected IBookMetaData metaData;
	protected BookContents mContents;
	protected String rawText;
	protected String mFilePath;
	protected BookID mID;
	protected String mDisplayName;
	protected Map<String, String> mBookSettings;
	
	//public abstract IChapter getChapter(ChapterID address);
	
	public BookID getID() { return mID; }
	public String getUID() { return String.valueOf(mID.getID()); }
	
	public String getPath() { return mFilePath; }
	
	public String getDisplayName() { return mDisplayName; }
	public String toString() {return mDisplayName;}
	
	protected void parseSttings(String settingsString) 
	{
		Map<String, String> map = new HashMap<String, String>();
		for (String line:settingsString.split("\\r?\\n"))
		{
			String[] lineParts = line.split("=");
			if (lineParts.length == 2)
			{
				map.put(lineParts[0], lineParts[1]);
			}
		}

		mBookSettings = map;
	}
	
	//Compare by id
	public int compareTo(ABook other) 
	{
		if (this.getID() == null) return -1;
		if (other.getID() == null) return 1;
		
		return this.getID().compareTo(other.getID());
	}
	
	public abstract ChapterTree getChapterList();
	
	public abstract ISearchResults search(ISearchQuery query);
}
