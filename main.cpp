#include "taskreader.h"

#include <fstream>
#include <iostream>

//
bool write_csv(const tasks::task_reader&, const std::string&, bool);
//

int main(int argc, char** argv)
{
    if (argc <= 2) {
        std::cerr << "No command line args!";
        return 1;
    }

    std::string in_file_name = argv[1];
    std::string out_file_name = argv[2];
    bool correct_only = [&]() {
        if (argc >= 4)
            return (std::string_view(argv[3]) == "/co");

        return false;
    }();

    tasks::task_reader reader(in_file_name);
    bool success = write_csv(reader, out_file_name, correct_only);

    if (success) {
        std::cout << "Test successfully decoded!\n";
    }
    else {
        std::cerr << "Failed to decode test!\n";
    }

    return 0;
}

bool write_csv(const tasks::task_reader& reader,
               const std::string& file_name,
               bool correct_only = true)
{
    std::ofstream file(file_name, std::ios::out | std::ios::binary);
    if (!file.is_open())
        return false;

    file.write("#;Question;Pic;Variants;Correct\n", 33);

    for (std::size_t t{0}; t < reader.count(); ++t) {
        auto task = reader.at(t);

        std::string variant_text = correct_only ?
                                   task.variants.at(task.correct_variant) : task.variants.at(0);

        std::string question_line =
                std::to_string(t+1) + ";" +
                task.text + ";" +
                task.flags.at(3) + ";" +
                variant_text + ";" +
                ((task.correct_variant == 0 || correct_only) ? "1\n" : "\n");
        file.write(question_line.c_str(), question_line.length() + 1);

        if (correct_only) continue;
        for (std::size_t v{1}; v < tasks::task_data::variants_count; ++v) {
            std::string variant_line = ";;;" +
                                       task.variants.at(v) + ";" +
                                       ((task.correct_variant == v) ? "1\n" : "\n");
            file.write(variant_line.c_str(), variant_line.length() + 1);
        }
    }

    file.close();

    return true;
}