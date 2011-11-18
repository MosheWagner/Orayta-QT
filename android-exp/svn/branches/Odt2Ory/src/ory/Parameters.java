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
	printXlinks, useUnoconv, debug, toratHaolah;
	private String inputFilename, outputPath, uid, bookTitle;
	private String source;
	private boolean outAsIn; //output path is the same as input.
	private boolean daat; //option for daat.ac.il conversion.
	private boolean force; //overwrite existing files.
	private boolean skip;
	
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
		setOutAsIn(line.hasOption("oi"));
		setIsDaat(line.hasOption("daat"));
		setForce(line.hasOption("f")); 
		setSkip(line.hasOption("q")); 
		uid = line.getOptionValue("uid", "");
		bookTitle = line.getOptionValue("B");
		source = line.getOptionValue("S");
		
		gui= line.hasOption("G");
		help = line.hasOption("h");
		
		
		printNotes = line.hasOption("no-print-notes")? false : true;
		printTableOfContents = line.hasOption("print-toc");
		printAnnotations = line.hasOption("print-annotations");
		printXlinks = line.hasOption("print-xlinks");
		useUnoconv = line.hasOption("unoconv");
		debug = line.hasOption("debug");
		toratHaolah = line.hasOption("TH");
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
		runOptions.addOption("oi", "output-as-input", false, "save files to the same directory as input");
		runOptions.addOption("f", "force", false, "overwrite existing files without warninig");
		runOptions.addOption("q", "quit", false, "skip existing files without warninig");
		runOptions.addOption("daat", false, "special option for html file from daat.ac.il");
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
		runOptions.addOption( OptionBuilder.withLongOpt( "unoconv" )
                .withDescription( "use unoconv from your os to translate format other then odt" +
                		"(default uses integrated 'jodconverter')")
                .create() );
		runOptions.addOption( OptionBuilder.withLongOpt( "debug" )
                .withDescription( "enable debug information")
                .create() );
		runOptions.addOption( OptionBuilder.withLongOpt( "TH" )
                .withDescription("special version for torat-ha'olah")
                .create() );
		
	
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
	 * @return true if user asked to use unoconv
	 */
	public boolean isUseUnoconv() {
		return useUnoconv;
	}

	/**
	 * @return true if debug enabled
	 */
	public boolean isDebug() {
		return debug;
	}
	
	/**
	 * @return the toratHaolah
	 */
	public boolean isToratHaolah() {
		return toratHaolah;
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
	 * @param outAsIn the outAsIn to set
	 */
	public void setOutAsIn(boolean outAsIn) {
		this.outAsIn = outAsIn;
	}

	/**
	 * @return true if the user asked to set the output path to be the same as input.
	 */
	public boolean isOutAsIn() {
		return outAsIn;
	}

	/**
	 * @param path the output path to set
	 */
	public void setOutputPath(String path) {
		this.outputPath = path;
	}

	/**
	 * @param daat - set daat to use special 'daat' conversion.
	 */
	public void setIsDaat(boolean daat) {
		this.daat = daat;
	}

	/**
	 * @return true if this is a daat converter.
	 */
	public boolean isDaat() {
		return daat;
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

	/** overwrite existing files
	 * @param true to overwrite existing files
	 */
	public void setForce(boolean force) {
		this.force = force;
	}

	/** overwrite existing files
	 * @return true if user wants to overwrite existing files.
	 */
	public boolean isForce() {
		return force;
	}

	public void setSkip(boolean skip) {
		this.skip = skip;
	}

	public boolean isSkip() {
		return skip;
	}

}
