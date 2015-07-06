package ee.filosoft.vabamorf;

public class Linguistic {
	
/**
* Analyze abbrevations stricktly. Applies to spell, analyze.
*/
	public boolean abbrevations = true;

/**
* Analyze Roman numerals. Applies to spell, analyze
*/
	public boolean romanNumerals = true;

/**
* Guess words. Applier to analyze, synthesize
*/
	public boolean guess = false;

/**
* Generates phenetic transcription. Applies to analyze, synthesize
*/
	public boolean phonetic = false;

/**
* Executes additional name analysis, useful for disambiguator. Applies to Analyze, requires full sentense
*/
	public boolean properName = false;

/**
* Initiates linguistic.
* @param fileName Path to lexicon file.
* @return true on success
*/
	public native boolean open(String fileName);

/**
* De-initiates linguistic and frees up resources.
*/
	public native void close();
	
/**
* Checks spelling of a single word.
* @param word Word to check.
* @return 0 if Word is correct; >0 if spelling is invalid; <0 in case of error.
*/
	public native int spellWord(String word);

/**
* Checks spelling of word sequence. Has primitive knowledge of phrases like New York.
* @param words Words to check.
* @return list of results per work. 0 if specific word is correct. Result size is equal to word list size.
* null in case of error.
*/
	public native int[] spellWords(String words[]);

/**
* Suggests replacements for misspelled words.
* @param word Word to suggest upon.
* @return list of suggestions ordered by likelyhood. null in case of error.
*/
	public String[] suggest(String word) { return suggest(word, false); }

/**
* Suggests replacements for misspelled words.
* @param word Word to suggest upon.
* @param startSentence true if the word is the first in the sentence.
* @return list of suggestions ordered by likelyhood. null in case of error.
*/
	public native String[] suggest(String word, boolean startSentence);
	
/**
* Morphologically analyzes a word.
* @param word Word to analyze.
* @return list of morphological information. null in case of error.
*/
	public native MorphInfo[] analyze(String word);

/**
* Morphologically analyzes a sentence.
* @param words Words to analyze.
* @return list of morphological information per word.
*/
	public native MorphInfos[] analyzeSentence(String words[]);

/**
* Synthesizes a word according to provided morphological information.
* @param info Morphological information about the word.
* @param hint Paradigm hint for a word (sg g) eg. palgi/palga
* @return list of morphologically detailed generated words. Form a real word by combining root+ending+clitic. null in case of error.
*/
	public native MorphInfo[] synthesize(MorphInfo info, String hint);

	protected void finalize() {
		close();
	}
	private long objId;
	static {
		System.loadLibrary("linguistic_jni");
	}	
}
