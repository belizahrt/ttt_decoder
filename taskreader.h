#ifndef CLION_TASKREADER_H
#define CLION_TASKREADER_H

#include <string>
#include <fstream>
#include <array>
#include <vector>

namespace tasks {
    struct task_data {
        static constexpr std::size_t variants_count{4};

        std::string                     text;
        std::size_t                     correct_variant{};
        std::array
        <std::string, variants_count>   variants;
    };

    class task_reader{
    public:
        explicit task_reader(const std::string &file_name);

        [[nodiscard]] task_data at(std::size_t pos) const;

        [[nodiscard]] std::size_t count() const;

    private:

        void read();

        std::string read_next();

        void read_unk();

    private:
        std::ifstream m_file;

        std::size_t m_tasks_count{0};
        std::vector<task_data> m_tasks;

        static const std::size_t questions_count_size{4};
        static const std::size_t useless_string_size{0xF * 2};
        static const std::size_t unk_strings_count{5};
    };
}

#endif //CLION_TASKREADER_H
