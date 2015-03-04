package book;

import java.util.HashMap;
import java.util.Map;

import search.ISearchQuery;
import search.ISearchResults;

/*
 * THE Book class.
 * From this class you can get a chpter's text (given it's address),
 *  get a tree of the chapters,
 *  and much more.
 */

public abstract class ABook 
{
	IBookMetaData metaData;
	String rawText;
	String mFilePath;
	BookID mID;
	String mDisplayName;
	Map<String, String> mBookSettings;
	
	public abstract IChapter getChapter(ChapterID address);
	
	public BookID getID() { return mID; }
	
	public String getPath() { return mFilePath; }
	
	public String getDisplayName() { return mDisplayName; }
	public String toString() {return mDisplayName;}
	
	protected void parseSttings(String settingsString) 
	{
		Map<String, String> map = new HashMap<String, String>();
		for (String line:settingsString.split("\n"))
		{
			String[] lineParts = line.split("=");
			if (lineParts.length == 2)
			{
				map.put(lineParts[0], lineParts[1]);
			}
		}

		mBookSettings = map;
	}
	
	public abstract ChapterTree getChapterList();
	
	public abstract ISearchResults search(ISearchQuery query);
}
