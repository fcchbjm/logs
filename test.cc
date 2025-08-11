#include "util.hpp"

int main()
{
    std::cout << cpplogs::util::Date::getTime() << std::endl;
    std::string pathname = "./ab/cd/e.txt";
    cpplogs::util::File::createDirectory(cpplogs::util::File::path(pathname));
    return 0;
}