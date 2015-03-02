package manager;

/*
 * This interface provides a set on functions that anyone
 *  may expect OraytaCoreManager to implement.
 *  
 * Together with UIBridge, this includes all OraytaCoreManager publicly does.
 */

public interface IOraytaCore 
{
	//Initialize Orayta. 
	// This should be done in a separate thread, and should later update the listener when it's done.
	public void initOrayta();
	
	//Register listener for events
	public abstract void registerListener(ICoreEventsListener listener);
}
