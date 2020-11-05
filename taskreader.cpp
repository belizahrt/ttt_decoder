#include "taskreader.h"

using namespace tasks;

task_reader::task_reader(const std::string &file_name)
    : m_file(file_name, std::fstream::binary)
{
    read();
}

std::size_t task_reader::count() const
{
    return m_tasks_count;
}

void task_reader::read()
{
    if (!m_file.is_open())
        return;

    // read questions count
    m_file.read(reinterpret_cast<char*>(&m_tasks_count), questions_count_size);
    m_tasks.reserve(m_tasks_count);

    // read each question
    for (std::size_t t{0}; t < m_tasks_count; ++t) {
        task_data task;

        //read text of the question
        task.text = read_next();

        // skip useless string
        m_file.seekg(useless_string_size, std::ios_base::cur);

        // read variants & correct variant id
        std::tie(task.correct_variant, task.variants) = [this]() {
            std::size_t correct_variant;
            std::array<std::string, task_data::variants_count> variants;

            for (std::size_t v{0}; v < variants.size(); ++v) {
                std::string variant = read_next();
                variants[v] = variant;

                std::string is_correct = read_next();
                if (is_correct == "1")
                    correct_variant = v;
            }
            read_unk();

            return std::move(std::make_pair(correct_variant, variants));
        }();

        m_tasks.push_back(std::move(task));
    }

    m_file.close();
}

std::string task_reader::read_next()
{
    std::string text;
    std::size_t pos{0};
    do {
        wchar_t c{};
        m_file.read(reinterpret_cast<char*>(&c), sizeof(wchar_t));
        c -= ((pos+1) % 2 == 0) ? 0x63 : 0x12;

        text.push_back(static_cast<unsigned char>(c));
    } while (pos++ < static_cast<unsigned char>(text.front()));
    text.erase(text.begin());

    return text;
}

void task_reader::read_unk()
{
    for (std::size_t i{0}; i < unk_strings_count; ++i) {
        read_next(); read_next();
    }
}

task_data task_reader::at(std::size_t pos) const
{
    return m_tasks.at(pos);
}
