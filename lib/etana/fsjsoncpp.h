#if !defined(FSJSONCPP_H)
#define FSJSONCPP_H

#include <jsoncpp/json/json.h> 
#include <iostream>
#include <string> 

class FSJSONCPP
{
    public:
        /**
         * @brief String Jsoniks
         * 
         * @param str JSON-kujule teisendatav (char *) string 
         * @param message Äpardumise korral veateade
         * @param jsonobj Õnnestumise korral vastav JSON
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
         * @param str JSON-kujule teisendatav (std::string&) string
         * @param message  Äpardumise korral veateade
         * @param jsonobj  Õnnestumise korral vastav JSON
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
         * @param jsonobj Kuvatav JSON
         * @param use_StyledWriter true -- Kujundatult, false -- Kõik ühel real
         */
        void JsonWriter(const Json::Value& jsonobj, bool use_StyledWriter = false)
        {
            if(use_StyledWriter)
            {
                Json::StyledWriter styled;
                std::cout << styled.write(jsonobj) << std::endl;
            }
            else
            {
                Json::FastWriter fast;
                std::cout << fast.write(jsonobj) << std::endl;
            }
        }

        /**
         * @brief Viga, üldjuhul programm pärast seda edasi ei tööta.
         * 
         * TJSON:
         * {"failure":{"errors":[array of status messages]}}
         * 
         * @param msg sõnum
         */
        void JsonError(const char* msg)
        {
            Json::Value json_err;
            json_err["failure"]["errors"].append(msg);
            FSJSONCPP().JsonWriter(json_err);
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
            FSJSONCPP().JsonWriter(json_warning);
        }        

    private:
            Json::Reader jsonreader;
};

#endif
