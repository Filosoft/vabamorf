#if !defined(FSJSONCPP_H)
#define FSJSONCPP_H

#include <jsoncpp/json/json.h> 
#include <iostream>
#include <string> 

class FSJSONCPP
{
    public:
        bool Parse(const char* str, std::string& message, Json::Value& jsonobj)
        {
            message = "";
            if(jsonreader.parse(str, jsonobj) == false)
            {
                message = jsonreader.getFormattedErrorMessages().c_str();
                return false;
            }
            return true;
        }

        bool Parse(const std::string& str, std::string& message, Json::Value& jsonobj)
        {
            message = "";
            if(jsonreader.parse(str, jsonobj) == false)
            {
                message = jsonreader.getFormattedErrorMessages().c_str();
                return false;
            }
            return true;
        }

        void Writer(const Json::Value& jsonobj, bool use_StyledWriter = false)
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

    private:
            Json::Reader jsonreader;
               

};

#endif
