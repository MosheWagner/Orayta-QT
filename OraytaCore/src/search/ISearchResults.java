package search;

import book.ChapterID;

/*
 * This class holds the search results from each specific book.
 */

public interface ISearchResults 
{
	public ChapterID getAddress();
	public String getPreview();
}
