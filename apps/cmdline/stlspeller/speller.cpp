#include "speller.h"

/* 2023.05.23 Kerge facelift Renee programmile.
*/

namespace vabamorf 
{
	#if defined(UNICODE)
	speller::speller(const std::wstring &lex)
	#else
	speller::speller(const std::string &lex)
	#endif
	{
		try 
		{
			linguistic.Open(lex.c_str());
		} 
		catch (...) 
		{
			throw exception();
		}
	}

	#if defined(UNICODE)
	void speller::open(const std::wstring &lex)
	#else
	void speller::open(const std::string &lex)
	#endif
	{
		try 
		{
			linguistic.Open(lex.c_str());
		} 	
		catch (...) 
		{
			throw exception();
		}
	}

	bool speller::spell(const std::wstring &word)
	{
		try 
		{
			return (linguistic.SpellWord(word.c_str()) == SPL_NOERROR);
		} 
		catch (...) 
		{
			throw exception();
		}
	}

	std::vector< std::wstring > speller::suggest(const std::wstring &word)
	{
		try 
		{
			std::vector< std::wstring > results;
			CFSWStringArray suggs = linguistic.Suggest(word.c_str());
			for (INTPTR ip=0; ip<suggs.GetSize(); ip++) 
			{
				results.push_back((const wchar_t *)suggs[ip]);
			}
			return results;
		} 
		catch (...) 
		{
			throw exception();
		}
	}
} // namespace
