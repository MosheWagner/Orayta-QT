package bookReader;

import book.ABook;
import book.BookContents;

/*
 * Returns fully built contents for the given book
 */
public interface IBookBuilder <T extends ABook>
{
	public BookContents buildContents(T Book);
}
