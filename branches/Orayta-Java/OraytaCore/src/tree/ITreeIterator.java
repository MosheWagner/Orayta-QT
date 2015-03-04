package tree;

import java.util.Iterator;

public interface ITreeIterator<T> extends Iterator<T>
{
	public boolean hasPrevious();
	public T previous();
	public T current();
}
