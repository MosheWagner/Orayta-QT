package ory;

public class Main {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		
		if (args.length == 0) {
    		new Gui();
//    		inputFilename = gui.getFilename();
    	} 
    	else if (args.length > 1) {
    		new Gui(); // change this in future.
//    		inputFilename = gui.getFilename();
    	}
    	else {    	
    	new Odt2Ory(args[0]); 
    	}


	}

}
