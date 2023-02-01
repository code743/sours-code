#include "file_utils.hpp"


bool file_utils::create_file_from_buffer(
    const std::string_view file_path, void* buffer, size_t size
)
{
    std::ofstream stream(file_path.data(), std::ios::binary);

    if (!stream.is_open())
    {
        std::cerr << "Error: Failed to open file '" << file_path.data() << "' for writing." << std::endl;
        return false;
    }

    if (!stream.write((char*)buffer, size))
    {
        std::cerr << "Error: Failed to write data to file '" << file_path.data() << "'." << std::endl;
        stream.close();
        return false;
    }

    stream.close();
    return true;
}
