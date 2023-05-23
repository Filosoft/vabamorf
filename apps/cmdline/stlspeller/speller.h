#if !defined(_STLSPELLER_H_jfahsdjfgqwurwajdnfe_)
#define _STLSPELLER_H_jfahsdjfgqwurwajdnfe_

/* 2023.05.23 Kerge facelift Renee programmile.
*/

#include "../../../lib/proof/proof.h"
#include <string>
#include <vector>

namespace vabamorf 
{

	class exception : public std::exception 
	{
	public:
		exception() : std::exception() { }
	};

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
		void open(const std::wstring &lex);
	#else
		void open(const std::string &lex);
	#endif

		virtual ~speller() { }

		bool spell(const std::wstring &word);
		std::vector< std::wstring > suggest(const std::wstring &word);

	protected:
		CLinguistic linguistic;
	};

	class SPELLER_UTF8
	{
	public:
		DECLARE_FSNOCOPY(SPELLER_UTF8);

		SPELLER_UTF8(const std::string &lex)
		{
			try 
			{
				spl.open(lex.c_str());
			} 
			catch (...) 
			{
				throw vabamorf::exception();
			}
		}

		virtual ~SPELLER_UTF8() { }

		bool spell(const std::string &word_utf8)
		{
			return spl.spell((const wchar_t*)FSStrAtoW(word_utf8.c_str(), FSCP_UTF8));
		}

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
		vabamorf::speller spl;
	};

} // namespace

#endif
