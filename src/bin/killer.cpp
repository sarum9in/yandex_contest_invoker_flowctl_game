#include "yandex/contest/invoker/flowctl/game/KillerImpl.hpp"
#include "yandex/contest/invoker/flowctl/game/KillerStreamInterface.hpp"

#include "yandex/contest/detail/LogHelper.hpp"

#include "yandex/contest/config/InputArchive.hpp"

#include "yandex/contest/system/Trace.hpp"

#include <boost/assert.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace config = yandex::contest::config;
using namespace yandex::contest::invoker::flowctl::game;

KillerImpl::Options readOptions(const boost::filesystem::path &path)
{
    boost::property_tree::ptree ptree;
    boost::property_tree::read_json(path.c_str(), ptree);
    KillerImpl::Options options;
    config::InputArchive<boost::property_tree::ptree>::loadFromPtree(options, ptree);
    return options;
}

int main(int argc, char *argv[])
{
    try
    {
        yandex::contest::system::Trace::handle(SIGABRT);
        BOOST_ASSERT(argc == 1 + 1);
        KillerImpl killer(readOptions(argv[1]));
        KillerStreamInterface iface(std::cin, std::cout);
        for (;;)
            iface.runOnce(killer);
    }
    catch (EndOfFileError &e)
    {
        STREAM_INFO << "EOF was reached, terminating.";
        STREAM_DEBUG << e.what();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
