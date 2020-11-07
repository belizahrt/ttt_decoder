#include "taskreader.h"

#include <fstream>

int main()
{
    tasks::task_reader reader("TypDinComp.dat");

    std::ofstream file("TypDinComp2.csv");

    file.write("#; Question; Answer\n", 20);

    for (std::size_t i{0}; i < reader.count(); ++i) {
        auto task = reader.at(i);

        std::string out{};
        out += (std::to_string(i+1) + "; ");
        out += (task.text + "; ");

//        out += (task.variants[task.correct_variant]) + "\n";
//        file.write(out.c_str(), out.length()+1);

        out += (task.variants.front() + "; ");
        out += ((task.correct_variant == 0) ? "1\n" : "\n");
        file.write(out.c_str(), out.length()+1);

        out.erase();
        for (std::size_t j{1}; j < 4; ++j) {
            out += "; ; ";
            out += (task.variants.at(j) + "; ");
            out += ((task.correct_variant == j) ? "1\n" : "\n");
        }

        for (std::size_t k{0}; k < 10; ++k) {
            out += "; ; ; ";
            out += task.flags.at(k) + "\n";
        }

        file.write(out.c_str(), out.length()+1);
    }

    return 0;
}
