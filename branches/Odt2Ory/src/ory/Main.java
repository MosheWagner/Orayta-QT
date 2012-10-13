package ory;

import org.apache.commons.cli.HelpFormatter;

public class Main {
	
	public static Parameters parameters;
	public static Utils ui;

	

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		
		parameters = new Parameters(args);
		
		if (parameters.isGui()){
			ui = new Gui();
		}
		else
			ui = new Utils();
			
		// if user indicated that he needs help, or if he did not provide a input file and we are not in gui mode (in which the user will defined the file latter).
		if (parameters.isHelp() || 
				(parameters.getInputFilename() == null && !parameters.isGui())){
			usage(); //print help message.
		}
		else {
			new Odt2Ory();
	    				
		}
	}
		




	private static void usage() {
		// automatically generate the help statement
		HelpFormatter formatter = new HelpFormatter();
		formatter.printHelp( "odt2ory [OPTIONS] -i <filename>", parameters.getRunOptions());
		
	}

}
