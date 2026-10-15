/**
 * @file vmetajsonhttp.cpp
 * @brief cpp-httplib-il põhinev VMETAJSON morfoloogilise analüüsi HTTP-server.
 */
#include <httplib.h>

#include <atomic>
#include <chrono>
#include <cstddef>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "../vmetajson/vmetajson.h"

namespace
{
    // Signaal võib saabuda ka HTTP töölõimele.
    static_assert(ATOMIC_BOOL_LOCK_FREE == 2, "Signaalitöötlus vajab lukuvaba atomic<bool>-i");
    std::atomic<bool> shutdownRequested(false);

    /**
     * @brief Märgib signaali saabumisel serveri sulgemise soovi.
     * @details Signaali numbrit ei kasutata. SIGINT ja SIGTERM seavad sama
     * lukuvaba lipu; serveri tegeliku sulgemise teeb ShutdownWatcher.
     */
    void RequestShutdown(int)
    {
        shutdownRequested.store(true, std::memory_order_relaxed);
    }

    /**
     * @brief Haldab FSC teegi käivitamist ja sulgemist oma eluea jooksul.
     * @note Peab elama kauem kui morfoloogiamootor, HTTP töölõimed ja
     * FSC-d kasutavate käivitusvigade käsitlemine.
     */
    class FscRuntime
    {
    public:
        /**
         * @brief Käivitab FSC teegi enne morfoloogiamootori loomist.
         * @throws std::runtime_error Kui FSCInit() teatab käivitamise nurjumisest.
         */
        FscRuntime()
        {
            if (!FSCInit())
                throw std::runtime_error("FSC käivitamine ebaõnnestus");
        }

        /**
         * @brief Sulgeb FSC teegi ja vabastab selle globaalsed ressursid.
         * @pre Morfoloogiamootor on hävitatud ja HTTP töölõimed lõpetanud.
         */
        ~FscRuntime()
        {
            FSCTerminate();
        }
    };

    /**
     * @brief Jälgib sulgemissoovi ja peatab serveri eraldi lõimest.
     * @details server.stop() kutsutakse tavalisest lõimest, mitte signaalitöötlusest.
     */
    class ShutdownWatcher
    {
    public:
        /**
         * @brief Käivitab serveri sulgemissoovi jälgiva lõime.
         * @param[in,out] server HTTP-server, mille kuulaja sulgemissoovi korral peatatakse.
         * @details Jälgija ootab serveri käivitumist ja kontrollib sulgemissoovi
         * ligikaudu iga 100 millisekundi järel.
         * @pre server peab elama kauem kui ShutdownWatcher.
         * @throws std::system_error Kui jälgimislõime loomine ebaõnnestub.
         */
        explicit ShutdownWatcher(httplib::Server& server)
            : finished(false), worker([this, &server]
            {
                while (!finished.load())
                {
                    if (shutdownRequested.load(std::memory_order_relaxed) && server.is_running())
                    {
                        server.stop();
                        return;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            })
        {
        }

        /**
         * @brief Lõpetab sulgemissoovi jälgimise ja ootab jälgimislõime lõppu.
         * @details HTTP töölõimede lõpetamise eest vastutab server.
         */
        ~ShutdownWatcher()
        {
            finished.store(true);
            worker.join();
        }

    private:
        std::atomic<bool> finished;
        std::thread worker;
    };

    /**
     * @brief Koostab UTF-8 JSON-kehaga HTTP-vastuse.
     * @param[out] response Vastus, mille staatus, keha ja sisutüüp määratakse.
     * @param[in] json Vastuse kehasse serialiseeritav JSON-väärtus.
     * @param[in] status HTTP staatuskood; vaikimisi 200.
     */
    void JsonResponse(httplib::Response& response, const Json::Value& json, int status = 200)
    {
        response.status = status;
        response.set_content(FSJSONCPP().Json2string(json, false, true), "application/json");
    }

    /**
     * @brief Koostab JSON-veavastuse, mille errors-massiiv sisaldab ühte teadet.
     * @param[out] response Koostatav HTTP-veavastus.
     * @param[in] status HTTP staatuskood.
     * @param[in] message errors-massiivi lisatav veateade.
     */
    void JsonError(httplib::Response& response, int status, const std::string& message)
    {
        Json::Value json(Json::objectValue);
        json["errors"].append(message);
        JsonResponse(response, json, status);
    }

    /**
     * @brief Teisendab käsurealt saadud kümnendstringi TCP-pordi numbriks.
     * @param[in] text Ainult numbritest koosnev pordi string.
     * @return Pordi number vahemikus 1..65535.
     * @throws std::invalid_argument Kui string on tühi, sisaldab muid märke
     * või pordi number jääb lubatud vahemikust välja.
     * @throws std::out_of_range Kui arv ei mahu std::stoul() tulemuse tüüpi.
     */
    int ParsePort(const std::string& text)
    {
        if (text.empty() || text.find_first_not_of("0123456789") != std::string::npos)
            throw std::invalid_argument("--port peab olema täisarv vahemikus 1..65535");
        size_t end;
        unsigned long port = std::stoul(text, &end);
        if (end != text.size() || port == 0 || port > 65535)
            throw std::invalid_argument("--port peab olema täisarv vahemikus 1..65535");
        return static_cast<int>(port);
    }

    /**
     * @brief Teisendab käsurealt saadud päringu keha ülempiiri baitide arvuks.
     * @param[in] text Positiivse kümnendsüsteemi täisarvu string.
     * @return Päringu keha lubatud suurim pikkus baitides.
     * @throws std::invalid_argument Kui string on tühi, sisaldab muid märke
     * kui numbrid või väärtus on null.
     * @throws std::out_of_range Kui väärtus ei mahu std::size_t tüüpi.
     */
    std::size_t ParseMaxContentLength(const std::string& text)
    {
        if (text.empty() || text.find_first_not_of("0123456789") != std::string::npos)
            throw std::invalid_argument("--max_content_length peab olema positiivne täisarv baitides");

        std::size_t length = 0;
        for (char c : text)
        {
            const std::size_t digit = static_cast<std::size_t>(c - '0');
            if (length > (std::numeric_limits<std::size_t>::max() - digit) / 10)
                throw std::out_of_range("--max_content_length väärtus on liiga suur");
            length = length * 10 + digit;
        }
        if (length == 0)
            throw std::invalid_argument("--max_content_length peab olema positiivne täisarv baitides");
        return length;
    }

    /**
     * @brief Kontrollib, kas päringu Content-Type on application/json.
     * @param[in] request Kontrollitav HTTP-päring.
     * @details Võrdlus ei erista suur- ja väiketähti. Sisutüübi ümbert
     * eemaldatakse tühimärgid ning semikoolonile järgnevaid parameetreid eiratakse.
     * @return true sobiva sisutüübi korral, false puuduva või muu sisutüübi korral.
     */
    bool IsJsonContentType(const httplib::Request& request)
    {
        std::string contentType = request.get_header_value("Content-Type");
        contentType.resize(contentType.find(';') == std::string::npos
            ? contentType.size() : contentType.find(';'));
        trim(contentType);
        std::transform(contentType.begin(), contentType.end(), contentType.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return contentType == "application/json";
    }
}

/**
 * @brief Käivitab VMETAJSON HTTP-serveri ja haldab selle elutsüklit.
 * @param[in] argc Käsurea argumentide arv koos programmi nimega.
 * @param[in] argv Käsurea argumendid: --host, --port, --max_content_length
 * ja VMETAJSON käivituslipud.
 * @details Vaikimisi kuulab server aadressil 0.0.0.0:8080. Sõnastik laaditakse
 * üks kord ning analüüsipäringud järjestatakse morfoloogiamootori mutex-i abil.
 * Päringu keha ülempiir on vaikimisi 8388608 baiti (8 MiB).
 * --json on keelatud; JSON-sisend loetakse HTTP-päringu kehast.
 * SIGINT ja SIGTERM sulgevad kuulaja ning server lõpetab alustatud päringud.
 * FSC suletakse pärast mootori hävitamist ja käivitusvigade käsitlemist.
 *
 * @par HTTP otspunktid
 * - POST /process võtab vastu JSON-objekti ja tagastab morfoloogilise analüüsi.
 * - GET /version tagastab version_vmetajson välja.
 * - GET /health tagastab status välja väärtusega ok.
 *
 * @return EXIT_SUCCESS abi kuvamisel või serveri korrektsel sulgemisel;
 * EXIT_FAILURE käivitus- või kuulamisvea korral.
 */
int main(int argc, char** argv)
{
    // FSC jääb käivitusvea korral alles ka VEAD-teate ja erandi destruktori jaoks.
    std::unique_ptr<FscRuntime> runtime;
    try
    {
        std::string host = "0.0.0.0";
        int port = 8080;
        std::size_t maxContentLength = 8 * 1024 * 1024;
        std::vector<char*> analyzerArgs(1, argv[0]);
        for (int i = 1; i < argc; ++i)
        {
            const std::string arg(argv[i]);
            if (arg == "--help" || arg == "-h")
            {
                std::cerr << "Kasutamine: " << argv[0]
                    << " [--host=0.0.0.0] [--port=8080] [--max_content_length=MAX]"
                    << " [--path=SÕNASTIKE_KATALOOG] [VMETAJSON lipud]\n"
                    << "MAX: päringu keha ülempiir baitides, vaikimisi 8388608 (8 MiB)\n"
                    << "POST /process, GET /version, GET /health\n";
                return EXIT_SUCCESS;
            }
            if (arg.compare(0, 7, "--host=") == 0)
            {
                host = arg.substr(7);
                if (host.empty())
                    throw std::invalid_argument("--host ei tohi olla tühi");
            }
            else if (arg.compare(0, 7, "--port=") == 0)
                port = ParsePort(arg.substr(7));
            else if (arg.compare(0, 21, "--max_content_length=") == 0)
                maxContentLength = ParseMaxContentLength(arg.substr(21));
            else if (arg.compare(0, 7, "--json=") == 0)
                throw std::invalid_argument("Serveri JSON-sisend tuleb POST /process päringust");
            else
                analyzerArgs.push_back(argv[i]);
        }

        // Runtime peab elama kauem kui mootor ja kõik HTTP töölõimed.
        runtime.reset(new FscRuntime());
        VMETAJSON analyzer;
        analyzer.Start(static_cast<int>(analyzerArgs.size()), analyzerArgs.data());
        const std::string version(analyzer.VERSION);
        std::mutex analyzerMutex;

        httplib::Server server;
        server.set_payload_max_length(maxContentLength);
        server.set_read_timeout(5);
        server.set_write_timeout(5);
        server.set_keep_alive_timeout(1);

        server.Get("/health", [](const httplib::Request&, httplib::Response& response)
        {
            Json::Value json(Json::objectValue);
            json["status"] = "ok";
            JsonResponse(response, json);
        });

        server.Get("/version", [&version](const httplib::Request&, httplib::Response& response)
        {
            Json::Value json(Json::objectValue);
            json["version_vmetajson"] = version;
            JsonResponse(response, json);
        });

        server.Post("/process", [&analyzer, &analyzerMutex](const httplib::Request& request,
                                                         httplib::Response& response)
        {
            if (!IsJsonContentType(request))
            {
                JsonError(response, 415, "Content-Type peab olema application/json");
                return;
            }

            Json::CharReaderBuilder builder;
            Json::CharReaderBuilder::strictMode(&builder.settings_);
            builder["collectComments"] = false;
            std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            Json::Value json;
            std::string error;
            if (!reader->parse(request.body.data(), request.body.data() + request.body.size(), &json, &error))
            {
                JsonError(response, 400, "Vigane JSON: " + error);
                return;
            }

            int status;
            {
                std::lock_guard<std::mutex> lock(analyzerMutex);
                status = analyzer.Process(json);
            }
            JsonResponse(response, json, status);
        });

        server.set_error_handler([](const httplib::Request&, httplib::Response& response)
        {
            if (response.body.empty())
                JsonError(response, response.status, "HTTP viga " + std::to_string(response.status));
        });
        server.set_exception_handler([](const httplib::Request&, httplib::Response& response,
                                       std::exception_ptr exception)
        {
            try
            {
                std::rethrow_exception(exception);
            }
            catch (const std::exception& error)
            {
                std::cerr << "Päringu töötlemine ebaõnnestus: " << error.what() << '\n';
            }
            catch (...)
            {
                std::cerr << "Päringu töötlemine ebaõnnestus\n";
            }
            JsonError(response, 500, "Serveri sisemine viga");
        });

        std::signal(SIGINT, RequestShutdown);
        std::signal(SIGTERM, RequestShutdown);
        if (!server.bind_to_port(host, port))
            throw std::runtime_error("HTTP-server ei saanud aadressi siduda: " + host + ":" + std::to_string(port));

        ShutdownWatcher watcher(server);
        std::cerr << "vmetajsonhttp: http://" << host << ':' << port << '\n';
        if (!server.listen_after_bind())
            throw std::runtime_error("HTTP-serveri kuulamine ebaõnnestus");
        return EXIT_SUCCESS;
    }
    catch (VEAD& error)
    {
        std::cerr << "VMETAJSON käivitamine ebaõnnestus: " << static_cast<const char*>(error.Teade()) << '\n';
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "Serveri käivitamine ebaõnnestus\n";
    }
    return EXIT_FAILURE;
}
