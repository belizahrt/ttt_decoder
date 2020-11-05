#include "taskreader.h"

#include <fstream>

int main()
{
    tasks::task_reader reader("Kachestvo.dat");

    std::ofstream file("Kachestvo.csv");

    file.write("#; Question; Variants; Correct\n", 31);

    for (std::size_t i{0}; i < reader.count(); ++i) {
        auto task = reader.at(i);

        std::string out{};
        out += (std::to_string(i) + "; ");
        out += (task.text + "; ");

        out += (task.variants.front() + "; ");
        out += ((task.correct_variant == 0) ? "1\n" : "\n");
        file.write(out.c_str(), out.length()+1);

        out.erase();
        for (std::size_t j{1}; j < 4; ++j) {
            out += " ; ; ";
            out += (task.variants.at(j) + "; ");
            out += ((task.correct_variant == j) ? "1\n" : "\n");
        }
        file.write(out.c_str(), out.length()+1);
    }

    return 0;
}
