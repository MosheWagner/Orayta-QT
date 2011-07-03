/**
 * 
 */
package ory;

/**
 * class that should hold all the notes in a book.
 * @author avi
 *
 */
public class Notes {
	private int counter;
	
	Notes(){
		counter =1;
		
	}
	
	public Note newNote(String str){
		Note note = new Note (str, counter);
		counter++ ;
		return note;
		
	}

}
