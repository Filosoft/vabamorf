import ee.filosoft.vabamorf.Morph;

class MorphTest {
	
	public static void main(String[] args) {

		Morph morph = new Morph();
		// morph.guess = true;
		// morph.phon = true;
		
		System.out.println("Init: " + morph.init("et.dct"));
		for (int i = 0; i < args.length; i++) {

			System.out.println("analyze(" + args[i] + ")");
			Morph.MorphInfo []analyzes = morph.analyze(args[i]);
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
				String []synths = morph.synthesize(analyzes[j]);
				for (int k = 0; synths != null && k < synths.length; k++) {
					System.out.println("    " + synths[k]);
				}
			}

		}

		morph.terminate();
		System.out.println("Done.");
		
	}
	
}
