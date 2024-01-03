#if !defined(_STLSPELLER_H_jfahsdjfgqwurwajdnfe_)
#define _STLSPELLER_H_jfahsdjfgqwurwajdnfe_

#include "../../../lib/etana/fsjsoncpp.h"

#include "../../../lib/proof/proof.h"
#include <string>
#include <vector>

#include <algorithm> 
#include <cctype>
#include <iostream>  
#include <sstream>
#include <assert.h>
#include <iterator>

namespace vabamorf 
{
	class exception : public std::exception 
	{
	public:
		exception() : std::exception() { }
	};

	// spelleri süvatuum, opereerib utf-16 stringidega
	class speller 
	{
	public:
		DECLARE_FSNOCOPY(speller);

	#if defined(UNICODE)
		speller(const std::wstring &lex);
	#else
		speller(const std::string &lex);
	#endif

	#if defined(UNICODE)
		speller(void) {};
	#else
		speller(void) {};
	#endif

	#if defined(UNICODE)
		void Start(const std::wstring &lex);
	#else
		void Start(const std::string &lex);
	#endif

		virtual ~speller() { }

		bool spell(const std::wstring &word);
		std::vector< std::wstring > suggest(const std::wstring &word);

	protected:
		CLinguistic linguistic;
	};

	// spelleri tuum, pealisehitus süvatuumale, opereerib utf-8 stringidega
	class SPELLER_UTF8
	{
	public:
		DECLARE_FSNOCOPY(SPELLER_UTF8);

		SPELLER_UTF8(void) {}

		/**
		 * @brief Construct a new speller utf8 object
		 * 
		 * @param lex -- Leksikoni pathname
		 */
		SPELLER_UTF8(const std::string &lex)
		{
			Start(lex);
		}

		/**
		 * @brief Leksikoni avamine
		 * 
		 * @param lex -- Leksikoni pathname
		 */
		void Start(const std::string &lex)
		{	
			try 
			{
				spl.Start(lex.c_str());
			} 
			catch (...) 
			{
				throw vabamorf::exception();
			}
		}

		virtual ~SPELLER_UTF8() { }

		/**
		 * @brief Speller
		 * 
		 * @param word_utf8 -- Kontrollitav sõne.
		 * @return true -- Tundub normaalne eestikeelne sõna.
		 * @return false -- Pigem kirjaviga.
		 */
		bool spell(const std::string &word_utf8)
		{
			return spl.spell((const wchar_t*)FSStrAtoW(word_utf8.c_str(), FSCP_UTF8));
		}

		/**
		 * @brief Anna soovitusi
		 * 
		 * @param word_utf8 -- Sõna millele otsime soovitusi
		 * @return std::vector< std::string > -- Massiiv sõnadest, mida pakume algse sõna asemele
		 */
		std::vector< std::string > suggest(const std::string &word_utf8)
		{
			std::vector<std::string> results;
			std::vector<std::wstring> suggestions = spl.suggest((const wchar_t*)FSStrAtoW(word_utf8.c_str(), FSCP_UTF8));
			for (size_t j = 0; j < suggestions.size(); j++) 
			{
				results.push_back((const char*)FSStrWtoA(suggestions[j].c_str(), FSCP_UTF8));
			}
			return results;
		}

	private:
		vabamorf::speller spl; /** spelleri süvatuum, opereerib utf-16 stringidega */
	};

//-------------------------------------------

	/**
	 * @brief JSONit käsitleva käsureaprogrammi templiit
	 * 
	 * @tparam MAIN -- VMETAJSON või VMETYJSON (realiseerimata)
	 * @param argc -- käsurea parameetrite arv
	 * @param argv -- käsurea parameetrite massiiv
	 * @return int 
	 */
	template <class MAIN>
	int MTemplateJson(int argc, FSTCHAR ** argv)
	{
		try
		{
			FSCInit();
			MAIN m;
			m.Start(argc, argv);
			m.Run();
			FSCTerminate();
			return EXIT_SUCCESS;
		}
		catch (VEAD& viga)
		{
			FSJSONCPP().JsonError((const char*)viga.Teade());
			FSCTerminate();
			return EXIT_FAILURE;
		}
		catch (CFSFileException& isCFSFileException)
		{
			FSJSONCPP().JsonError("FSC: S/V viga");
			FSCTerminate();
			return EXIT_FAILURE;
		}
		catch (CFSMemoryException&)
		{
			FSJSONCPP().JsonError("FSC: Liiga vähe vaba mälu");
			FSCTerminate();
			return EXIT_FAILURE;
		}
		catch (CFSRuntimeException&)
		{
			FSJSONCPP().JsonError("FSC: Jooksis kokku");
			FSCTerminate();
			return EXIT_FAILURE;
		}
		catch (...)
		{
			FSJSONCPP().JsonError("Tundmatu throw()");
			FSCTerminate();
			return EXIT_FAILURE;
		}
	}


	/**
	 * @brief trim from start (in place)
	 * 
	 * @param s
	 */
	static inline void ltrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
	}

	/**
	 * @brief trim from end (in place)
	 * 
	 * @param s 
	 */
	static inline void rtrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
	}

	/**
	 * @brief trim from both ends (in place)
	 * 
	 * @param s 
	 */
	static inline void trim(std::string &s)
	{
		ltrim(s);
		rtrim(s);
	}

	class SPELLERJSON
	{
	public:
		SPELLERJSON(void)
		{
			InitClassVariables();
		}

		/**
		 * @brief Käsurealt antud parameetrite käsitlemine
		 * 
		 * @param argc 
		 * @param argv 
		 */
		void Start(int argc, FSTCHAR** argv)
		{
			LipuStringidKasurealt(argc, argv);
			// spelleri sõnastiku avamine
			// speller.Start(....)
			CFSString pohiSonastikuPikkNimi, pohiSonastikuNimi = FSTSTR("et.dct");
    		CFSFileName ps;

			if (Which(&pohiSonastikuPikkNimi, &path, &pohiSonastikuNimi) == false)
			{
				fsJsonCpp.JsonError("Ei saanud põhisõnastiku avamisega hakkama");
				exit(EXIT_FAILURE);
			}
			speller.Start((const char*)pohiSonastikuPikkNimi);
		}

		/**
		 * @brief Korraldame käsurealt või std-sisendist tuleva jsoni läbilaskmist
		 * 
		 */
		void Run(void)
		{ 
			if(json_str_fs.GetLength() > 0)
			{   // JSON sisend tuleb käsurea parameetrist
				Json::Value jsonobj;
				std::string message;
				if(fsJsonCpp.JsonParse((const char*)json_str_fs, message, jsonobj)==false)
					fsJsonCpp.JsonWarning(message.c_str());
				else
					TeeSeda(jsonobj);
			}
			else
			{   // JSON sisend tuleb std-sisendist
				std::string line;  
				while(std::getline(std::cin,line))
				{
					trim(line);
					if(line.length() <= 0 || line[0]=='#') // tühje ridasid ja kommentaare ignoreerime
						continue;
					Json::Value jsonobj;
					std::string message;
					if(fsJsonCpp.JsonParse(line, message, jsonobj)==false)
						fsJsonCpp.JsonWarning(message.c_str());
					else
						TeeSeda(jsonobj);
				} 
			}
		}

		/**
		 * @brief Taastame argumentideta konstruktori järgse seisu
		 * 
		 */
		void Stop(void)
		{
			InitClassVariables();
		}

		const char* VERSION = "2024.01.03";

	private:
		CFSAString path;            // --path=... (ainult käsurealt)
		bool lipp_taanded;          // --formattedjson (käsurealt & std sisendist läbi jsoni)
		bool lipp_utf8;             // --utf8json (käsurealt & std sisendist läbi jsoni)
		bool lipp_version;          // versiooni-info kuvamine
		CFSAString json_str_fs;     // --json=... lipu tagant (käsurealt & std sisendist)

		FSJSONCPP  fsJsonCpp;
		vabamorf::SPELLER_UTF8 speller;

		/**
		 * @brief Panema paika lippude vaikeväärtused
		 * 
		 */
		void VaikeLipudPaika(void)
		{
			lipp_taanded=false;    	// kogu json ühel real
			lipp_utf8=false;        // true: väljund utf8, muidu nagi jsonis ikka  
        	lipp_version=false;     // EI kuva väljundis versiooniinfot 
		}

	/**
		* @brief Kohendame lippude vaikeväärtused käsurealt antutele vastavaks
		* 
		* @param argc -- Lippude massivi pikkus
		* @param argv -- Lippude massiiv
		*/
		void LipuStringidKasurealt(int argc, FSTCHAR** argv)
		{
			assert(EmptyClassInvariant() == true);
			VaikeLipudPaika();

			PATHSTR pathstr;
			path=(const char*)pathstr; // Vaikimisi keskkonnamuutujast PATH
			bool lipudOK=true;  
			Json::Value jsonError;    
			for(int i=1; i<argc; ++i)
			{
				if(strcmp("-h", argv[i])==0 || strcmp("--help", argv[i])==0)
				{
				syntaks:
					// see kirjuta std::cerr peale ümber
					std::cerr << "Programmi kirjeldust vt https://github.com/Filosoft/vabamorf/blob/master/apps/cmdline/stlspellerjson/README.md\n",
					exit(EXIT_FAILURE);
				}
				if(LipuStringPaika(argv[i])==false)
				{
					lipudOK=false;
					std::string errString = "Illegaalne lipp: ";
					errString += argv[i];
					jsonError["failure"]["errors"].append(errString);
				}           
			}
			if(lipudOK==false)
			{
				fsJsonCpp.JsonWriter(jsonError);
				exit(EXIT_FAILURE);
			}
		}   

		/**
		 * @brief Üksiku stringina antud parameetri käitlemine
		 * 
		 * @param lipuString 
		 * @return true -- oli lubatud lipp, false -- ei tundnud sellist
		 */
		bool LipuStringPaika(const FSTCHAR* lipuString)
		{
			//-----------------------------
			if(strncmp("--path=", lipuString, sizeof("--path=")-1)==0)
			{
				path=lipuString+(strchr(lipuString, '=')-lipuString+1);
				return true;
			}
			//=============================
			if(strncmp("--json=", lipuString, sizeof("--json=")-1)==0)
			{
				json_str_fs = lipuString+(strchr(lipuString, '=')-lipuString+1);
				return true;
			}
			if(strcmp("--formattedjson", lipuString)==0)
			{
				lipp_taanded=true;
				return true;
			}
			if(strcmp("--utf8json", lipuString)==0)
			{
				lipp_utf8=true;
				return true;
			}
			//-----------------------------
			if(strcmp("--version", lipuString)==0) // lisa väljundjsonisse versiooniinfo
			{
				lipp_version=true;
				return true;
			}  
			//-----------------------------
			return false;
		}

		/** 
		 * @brief Laseme ühe jsoni päringu läbi
		 */
		void TeeSeda(Json::Value& jsonobj)
		{
			bool ret;
			CFSWString rida;
			LYLI lyli;

			// vaatame, kas JSONist tuleb parameetreid
			if(jsonobj.isMember("params")==true && jsonobj["params"].isMember("stlspellerjson")==true)
			{
				// võtame JSONist  tulemuse kuvamise lipud
				VaikeLipudPaika();
				bool lipudOK=true;           
				for(Json::Value::const_iterator i=jsonobj["params"]["stlspellerjson"].begin(); i != jsonobj["params"]["stlspellerjson"].end(); ++i)
				{
					if(LipuStringPaika(i->asCString())==false)
					{
						lipudOK=false;
						std::string warning = "Illegaalne lipp: " + i->asString();
						jsonobj["warnings"].append(warning);
					}
				}
				if(lipudOK==false)
				{
					fsJsonCpp.JsonWriter(jsonobj);
					return;
				}
			}

			// Teeme mida vaja, sisuline töö JSONi kaudu antud sõnadega
			if(lipp_version==true)
			{
				jsonobj["version"] = VERSION;
			}
			if(jsonobj.isMember("content")==true) // kui "content" on olemas...
			{
				if((jsonobj.isMember("annotations") == false) || (jsonobj["annotations"].isMember("tokens") == false))
				{
					// teeme "content"-ist annoteeritud sõned
					std::stringstream tokens(jsonobj["content"].asString());
					std::istream_iterator<std::string> begin(tokens);
					std::istream_iterator<std::string> end;
					std::vector<std::string> vectokens(begin, end);
				
					for(std::string token : vectokens)
					{
						Json::Value jsonToken;
						jsonToken["features"]["token"] = token;
						jsonobj["annotations"]["tokens"].append(jsonToken); 
					}
				}
			}	
			if(jsonobj.isMember("annotations") && jsonobj["annotations"].isMember("tokens"))
			{
				Json::Value& jsonTokens = jsonobj["annotations"]["tokens"];
				for(Json::Value& jsonToken : jsonTokens)
				{
					Json::Value& jsonFeatures = jsonToken["features"]; 
					if (speller.spell(jsonFeatures["token"].asCString()) == false) 	// seda sõna ei tundnud speller ära...
					{									// ...leiame soovitused...
						Json::Value suggestions_jsonlist;
						std::vector<std::string> suggestions = speller.suggest(jsonFeatures["token"].asCString());
						if(suggestions.size() > 0)
						{
							for (size_t j = 0; j < suggestions.size(); j++) 
							{
								suggestions_jsonlist.append(suggestions[j]);
							}
							jsonToken["features"]["suggestions"] = suggestions_jsonlist;
						}
						else
							jsonToken["features"]["suggestions"] =  Json::arrayValue;
					}
				}
			}
			
			fsJsonCpp.JsonWriter(jsonobj, lipp_taanded, lipp_utf8);		// ...kuvame tulemused
		}

		/** 
		 * @brief Muutujate esialgseks initsialiseerimsieks konstruktoris
		 */
		void InitClassVariables(void)
		{
			VaikeLipudPaika();
		}

		/** 
		 * @brief Argumentideta konstruktori abil starditud klassi invariant
		 */
		bool EmptyClassInvariant(void)
		{
			return true;
		}

		/** 
		 * @brief Initsialiseeritud klassi invariant 
		 */

		bool ClassInvariant(void)
		{
			return true;
		}

		/** 
		 * @brief Copy-konstruktor on illegaalne
		 */
		SPELLERJSON(const SPELLERJSON&)
		{
			assert(false);
		}

		/** 
		 * @brief Omistamisoperaator on illegaalne
		 */
		SPELLERJSON & operator=(const SPELLERJSON&)
		{
			assert(false);
			return *this;
		}
	};	

} // namespace

#endif
