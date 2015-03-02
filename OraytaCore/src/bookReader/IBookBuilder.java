package bookReader;

import book.Book;
import book.BookID;

/*
 * Returns a fully built book from the given id.
 */
public interface IBookBuilder 
{
	public Book buildBook(BookID id);
}
