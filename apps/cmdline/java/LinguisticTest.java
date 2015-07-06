import ee.filosoft.vabamorf.Linguistic;
import ee.filosoft.vabamorf.MorphInfo;

class LinguisticTest {
	
	public static void main(String[] args) {

		Linguistic linguistic = new Linguistic();
		// linguistic.guess = true;
		// linguistic.phon = true;
		
		System.out.println("Init: " + linguistic.open("et.dct"));

		int[] spellings = linguistic.spellWords(args);
		System.out.println("spellWords(...)");
		for (int i = 0; spellings != null && i < args.length; i++) {
			System.out.println("  " + args[i] + " - " + spellings[i]);
		}
		
		for (int i = 0; i < args.length; i++) {

			System.out.println("spellWord(" + args[i] + ")");
			int spellResult = linguistic.spellWord(args[i]);
			System.out.println("  " + spellResult);
			if (spellResult > 0) {
				System.out.println("  suggest(" + args[i] + ")");
				String suggs[] = linguistic.suggest(args[i]);
				for (int j = 0; suggs != null && j < suggs.length; j++) {
					System.out.println("    " + suggs[j]);
				}
			}

			System.out.println("analyze(" + args[i] + ")");
			MorphInfo[] analyzes = linguistic.analyze(args[i]);
			for (int j = 0; analyzes != null && j < analyzes.length; j++) {
				System.out.println("  " + analyzes[j].root + " | " +
					analyzes[j].ending + " | " + analyzes[j].clitic + " | " +
					analyzes[j].pos + " | " + analyzes[j].form
				);

				System.out.println("  synthesize(...)");
				analyzes[j].root = analyzes[j].root.replaceAll("_", "").replaceAll("=", "");
				if (analyzes[j].pos == 'V') {
					analyzes[j].root += "ma";
				}
				MorphInfo[] synths = linguistic.synthesize(analyzes[j], "");
				for (int k = 0; synths != null && k < synths.length; k++) {
					System.out.println("    " + synths[k].root + synths[k].ending + synths[k].clitic);
				}
			}

		}

		linguistic.close();
		System.out.println("Done.");
		
	}
	
}
