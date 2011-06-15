package ory;

public class Main {

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		
		if (args.length == 1) {
			Odt2Ory odt2ory = new Odt2Ory();
	    	odt2ory.process(args[0]);
		}
		else {
			new Gui();
		}
    	



	}

}
