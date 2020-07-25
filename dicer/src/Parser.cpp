#include <tao/pegtl.hpp>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        pegtl::argv_input in(argv, 1);
        pegtl::parse<pegtl::json::text>(in);
    }
}
