import ee.filosoft.vabamorf.Linguistic;
import ee.filosoft.vabamorf.Disambiguator;
import ee.filosoft.vabamorf.MorphInfo;
import ee.filosoft.vabamorf.MorphInfos;

class DisambiguatorTest {
	
	static void output(MorphInfos[] morphInfos) {
		if (morphInfos == null) return;
		for (int i = 0; i < morphInfos.length; i++) {
			System.out.println(morphInfos[i].word);
			MorphInfo[] morphInfo = morphInfos[i].morphInfo;
			for (int j = 0; j < morphInfo.length; j++) {
				System.out.println("  " + morphInfo[j].root + " | " +
					morphInfo[j].ending + " | " + morphInfo[j].clitic + " | " +
					morphInfo[j].pos + " | " + morphInfo[j].form
				);
			}
		}
	}
	
	public static void main(String[] args) {

		Linguistic linguistic = new Linguistic();
		linguistic.properName = true;
		linguistic.guess = true;
		System.out.println("Linguistic init: " + linguistic.open("et.dct"));
		
		Disambiguator disambiguator = new Disambiguator();
		System.out.println("Disambiguator init: " + disambiguator.open("et3.dct"));
		
		MorphInfos[] analysis = linguistic.analyzeSentence(args);
		System.out.println("Analysis:");
		output(analysis);

		MorphInfos[] disambiguated = disambiguator.disambiguate(analysis);
		System.out.println("Disambiguated:");
		output(disambiguated);
		
		disambiguator.close();
		linguistic.close();
		System.out.println("Done.");
		
	}
	
}
