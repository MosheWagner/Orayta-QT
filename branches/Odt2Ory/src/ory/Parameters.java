package ory;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.GnuParser;
import org.apache.commons.cli.OptionBuilder;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

public class Parameters {
	private Options runOptions;
	private CommandLine line;
	
	private boolean gui, help;
	//parameters for the extractor;
	private boolean printNotes, printTableOfContents, printAnnotations,
	printXlinks;
	private String inputFilename, outputPath, uid, bookTitle;
	private String source;
	
	
	public Parameters (String[] args) {
//		assignDefaults();
		getOptions();
		
		CommandLineParser parser = new GnuParser();
	    try {
	        // parse the command line arguments
	        line = parser.parse( runOptions, args );
	    }
	    catch( ParseException exp ) {
	        // oops, something went wrong
	        System.err.println( "Parsing failed.  Reason: " + exp.getMessage() );
	    }
	    
	    assignValues();
	    
	    if (args.length == 0)
	    	gui = true;
	    	    
		
	}
	
	private void assignValues() {
		
		inputFilename = line.getOptionValue("i");
		outputPath = line.getOptionValue("o");
		uid = line.getOptionValue("uid", "");
		bookTitle = line.getOptionValue("B");
		source = line.getOptionValue("S");
		
		gui= line.hasOption("G");
		help = line.hasOption("h");
		
		
		printNotes = line.hasOption("no-print-notes")? false : true;
		printTableOfContents = line.hasOption("print-toc");
		printAnnotations = line.hasOption("print-annotations");
		printXlinks = line.hasOption("print-xlinks");
		
	}

//	private void assignDefaults() {
//		
//		inputFilename = null; 
//		outputFilename = null;
//		uid = null;
//		bookTitle = null;
//		
//		gui= false;
//		help = false;
//		
//		printNotes = true;
//		printTableOfContents = false;
//		printAnnotations = false;
//		printXlinks = false;
//		
//		
//		
//	}
	
	
	@SuppressWarnings("static-access")
	private void getOptions(){
		
		runOptions = new Options();
		
		runOptions.addOption("i", true, "input file name");
		runOptions.addOption("o", "output-path", true, "where to save our file");
		runOptions.addOption("uid", true, "uniqueId value for this file");
		runOptions.addOption("B", true, "set book name in conf file");
		runOptions.addOption("S", "source", true, "the source of this file");
		
		runOptions.addOption("G", false, "grafical inteface");
		runOptions.addOption("h", "help", false, "show this help message");
		
		runOptions.addOption( OptionBuilder.withLongOpt( "no-print-notes" )
                .withDescription( "don't get footnotes and end notes (default is to add them)" )
                .create() );
		runOptions.addOption( OptionBuilder.withLongOpt( "print-toc" )
                .withDescription( "extratct also table of contents to the output file" )
                .create() );
		runOptions.addOption( OptionBuilder.withLongOpt( "print-annotations" )
                .withDescription( "extratct also annotations notes to the output file" )
                .create() );
		runOptions.addOption( OptionBuilder.withLongOpt( "print-xlinks" )
                .withDescription( "extratct also internel links to the output file" )
                .create() );
		
		//TODO: add options for these:
//		printNotes = true;
//		printTableOfContents = false;
//		printAnnotations = false;
//		printXlinks = false;
//		
	
	}
	

	/**
	 * @return the runOptions
	 */
	public Options getRunOptions() {
		return runOptions;
	}

	/**
	 * @return the line
	 */
	public CommandLine getLine() {
		return line;
	}

	/**
	 * @return the gui
	 */
	public boolean isGui() {
		return gui;
	}

	/**
	 * @return the help
	 */
	public boolean isHelp() {
		return help;
	}

	/**
	 * @return the printNotes
	 */
	public boolean isPrintNotes() {
		return printNotes;
	}

	/**
	 * @return the printTableOfContents
	 */
	public boolean isPrintTableOfContents() {
		return printTableOfContents;
	}

	/**
	 * @return the printAnnotations
	 */
	public boolean isPrintAnnotations() {
		return printAnnotations;
	}

	/**
	 * @return the printXlinks
	 */
	public boolean isPrintXlinks() {
		return printXlinks;
	}

	/**
	 * @return the inputFilename
	 */
	public String getInputFilename() {
		return inputFilename;
	}

	/**
	 * @return the outputFilename
	 */
	public String getOutputPath() {
		return outputPath;
	}

	/**
	 * @param path the output path to set
	 */
	public void setOutputPath(String path) {
		this.outputPath = path;
	}

	/**
	 * @return the uid
	 */
	public String getUid() {
		return uid;
	}

	/**
	 * @return the bookTitle
	 */
	public String getBookTitle() {
		return bookTitle;
	}

	/**
	 * @return the source
	 */
	public String getSource() {
		return source;
	}

}
