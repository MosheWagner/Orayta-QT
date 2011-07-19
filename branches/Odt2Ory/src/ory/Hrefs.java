package ory;

public class Hrefs {
	private int counter;
	
	Hrefs(){
		counter =1;
		
	}
	
	public Href newHref(String str){
		Href href = new Href (str, counter);
		counter++ ;
		return href;
		
	}

}
