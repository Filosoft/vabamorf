#if !defined(FSJSONCPP_H)
#define FSJSONCPP_H

#include <jsoncpp/json/json.h> 

#include <algorithm> 
#include <cctype>
#include <iostream>  
#include <string> 
#include <assert.h>
#include <iterator>



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

class FSJSONCPP
{
    public:
        /**
         * @brief String Jsoniks
         * 
         * @param str[in] JSON-kujule teisendatav (char *) string 
         * @param message[out] Äpardumise korral veateade
         * @param jsonobj[out] Õnnestumise korral vastav JSON
         * @return true -- Õnnestus, false -- Äpardus 
         */
        bool JsonParse(const char* str, std::string& message, Json::Value& jsonobj)
        {
            message = "";
            if(jsonreader.parse(str, jsonobj) == false)
            {
                message = jsonreader.getFormattedErrorMessages().c_str();
                return false;
            }
            return true;
        }

        /**
         * @brief 
         * 
         * @param str[in] JSON-kujule teisendatav (std::string&) string
         * @param message[out]  Äpardumise korral veateade
         * @param jsonobj[out]  Õnnestumise korral vastav JSON
         * @return true -- Õnnestus, false -- Äpardus
         */
        bool JsonParse(const std::string& str, std::string& message, Json::Value& jsonobj)
        {
            message = "";
            if(jsonreader.parse(str, jsonobj) == false)
            {
                message = jsonreader.getFormattedErrorMessages().c_str();
                return false;
            }
            return true;
        }

        /**
         * @brief Json std::cout faili
         * 
         * @param jsonobj[in] Kuvatav JSON
         * @param use_StyledWriter[in] true Kujundatult, false -- Kõik ühel real
         * @param use_emitUTF8[in] true: väljundisse UTF-8
         */
        void JsonWriter(const Json::Value& jsonobj, const bool use_StyledWriter = false, const bool use_emitUTF8=false)
        {
            Json::StreamWriterBuilder wbuilder;
            if(use_StyledWriter==false)
                wbuilder["indentation"] = "";
            wbuilder["emitUTF8"] = use_emitUTF8;
            std::cout << Json::writeString(wbuilder,jsonobj) << std::endl;
        }

        /**
         * @brief 
         * 
         * @param jsonobj[in] JSONsisend
         * @param use_StyledWriter[in] true Kujundatult, false -- Kõik ühel real
         * @param use_emitUTF8[in] true: väljundisse UTF-8
         *
         * @return const std::string stringiks teisendatud JSONsisend
         */
        const std::string Json2string(const Json::Value& jsonobj, const bool use_StyledWriter = false, const bool use_emitUTF8=false)
        {
            Json::StreamWriterBuilder wbuilder;
            if(use_StyledWriter==false)
                wbuilder["indentation"] = "";
            wbuilder["emitUTF8"] = use_emitUTF8;
            return Json::writeString(wbuilder,jsonobj);
        }

        /**
         * @brief Viga, üldjuhul programm pärast seda edasi ei tööta.
         * 
         * TJSON:
         * {"errors":[array of status messages]}
         * 
         * @param msg[in] sõnum
         */
        void JsonError(const char* msg)
        {
            Json::Value json_err;
            json_err["errors"].append(msg);
            JsonWriter(json_err);
        }

        /**
         * @brief Viga, üldjuhul programm pärast seda edasi ei tööta.
         * 
         * JSON:
         * {"errors":[array of status messages]}
         * 
         * @param msg[in] sõnum
         * @param jsonobj[in/out] sellele JSONobjekti massiivi jsonobj["errors"] lisame veateate msg
         */
        void AddJsonError(const char* msg, Json::Value& jsonobj)
        {
            jsonobj["errors"].append(msg);
        }

        /**
         * @brief Hoiatus, see päring läks metsa, proovi uut.
         * 
         * JSON:
         * {"warnings":[array of status messages]}
         *
         * @param msg sõnum
         */
        void JsonWarning(const char* msg)
        {
            Json::Value json_warning;
            json_warning["warnings"].append(msg);
            JsonWriter(json_warning);
        }        

        /**
         * @brief Hoiatus, see päring läks metsa, proovi uut.
         * 
         * JSON:
         * {"warnings":[array of status messages]}
         *
         * @param msg[in] sõnum
         * @param jsonobj[in/out] sellele JSONobjekti massiivi jsonobj["errors"] lisame hoiatuse msg
         */
        void AddJsonWarning(const char* msg, Json::Value& jsonobj)
        {
            jsonobj["warnings"].append(msg);
        }

    private:
            Json::Reader jsonreader;
};

#endif
