package ory;

import org.apache.commons.cli.HelpFormatter;

public class Main {
	
	public static Parameters parameters;

	

	/**
	 * @param args
	 * @throws Exception 
	 */
	public static void main(String[] args) throws Exception {
		
		parameters = new Parameters(args);
		
		if (parameters.isGui()){
			new Gui();
		}
		else if (parameters.isHelp() || 
				parameters.getInputFilename() == null){
			usage();
		}
		else {
			Odt2Ory odt2ory = new Odt2Ory();
	    	odt2ory.process(parameters.getInputFilename());
			
		}
		
//		if (args.length == 1) {
//			Odt2Ory odt2ory = new Odt2Ory();
//	    	odt2ory.process(args[0]);
//		}
//		else {
//			new Gui();
//		}
    	



	}



	private static void usage() {
		// automatically generate the help statement
		HelpFormatter formatter = new HelpFormatter();
		formatter.printHelp( "odt2ory [OPTIONS] -i <filename>", parameters.getRunOptions());
		
	}

}
