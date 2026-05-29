#include <functional>
#include <map>
#include <memory>
#include <string>

#include "EDMInterface.hpp"

namespace olympia::edm
{
    class EDMBackendFactory
    {
      public:
        using BackendCreator = std::function<std::unique_ptr<EDMInterface>(
            const std::string & config_file, const std::string & filename)>;

        static std::unique_ptr<EDMInterface> create(const std::string & backend_name,
                                                    const std::string & config_file,
                                                    const std::string & filename);

        static void registerBackend(const std::string & name, BackendCreator creator);

        const std::string & getDefaultBackend();
        static std::string & getDefault();

      private:
        static std::map<std::string, BackendCreator> & getRegistry_();
    };

    /**
     * The registrar - is global. This registrar uses RAII - Resource aquisition is initliazaiton.
     * This means that the different backends, like pegasus and whisper - are not declared and
     initialize globally. Instead - this registrar is global and when we acquire the backend by
       specifying it - then we have the backend.
    */

    template <typename Implementation> struct BackendRegistrar
    {
        BackendRegistrar(const std::string & name)
        {

            EDMBackendFactory::registerBackend(
                name, [](const std::string & config_file, const std::string & filename)
                { return std::make_unique<Implementation>(config_file, filename); });
        }
    };
} // namespace olympia::edm
