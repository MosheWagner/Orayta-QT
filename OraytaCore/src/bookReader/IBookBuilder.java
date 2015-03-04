package bookReader;

import book.ABook;
import book.BookID;

/*
 * Returns a fully built book from the given book-id.
 */
public interface IBookBuilder 
{
	public ABook buildBook(BookID id);
}
