#include "log_container.h"
#include "zlib.h"

#include "fstream"

BLFSTART
LogContainer::LogContainer()
{
    signature = BL_OBJ_SIGNATURE;
    header_version = 1;
    object_type = BL_OBJ_TYPE_LOG_CONTAINER;
    compression_method = 0;
    compressed_file.resize(COMPRESS_FILE_SIZE);
    uncompressed_file.resize(UNCOMPRESS_FILE_SIZE);

    compression_method = 0;
    reserved_log_container1 = 0;
    reserved_log_container2 = 0;
    uncompressed_file_size = 0;
    reserved_log_container3 = 0;
    compressed_file_size = 0;
    file_position = 0;
}

void LogContainer::read(AbstractFile& is)
{
    is.read(reinterpret_cast<char*>(&signature), sizeof(signature));
    is.read(reinterpret_cast<char*>(&header_size), sizeof(header_size));
    is.read(reinterpret_cast<char*>(&header_version), sizeof(header_version));
    is.read(reinterpret_cast<char*>(&object_size), sizeof(object_size));
    is.read(reinterpret_cast<char*>(&object_type), sizeof(object_type));

    is.read(reinterpret_cast<char*>(&compression_method), sizeof(compression_method));
    is.read(reinterpret_cast<char*>(&reserved_log_container1), sizeof(reserved_log_container1));
    is.read(reinterpret_cast<char*>(&reserved_log_container2), sizeof(reserved_log_container2));
    is.read(reinterpret_cast<char*>(&uncompressed_file_size), sizeof(uncompressed_file_size));
    is.read(reinterpret_cast<char*>(&reserved_log_container3), sizeof(reserved_log_container3));
    compressed_file_size = object_size - log_container_internal_header_size();// LOGCONTAINER_INTERNAL_HEADER_SIZE;
    is.read(reinterpret_cast<char*>(compressed_file.data()), compressed_file_size);

    is.seekg(object_size % 4, std::ios_base::cur);
}

void LogContainer::read(LogContainer& log_container, std::fstream& file)
{
    file.read(reinterpret_cast<char*>(&log_container.signature), sizeof(signature));
    file.read(reinterpret_cast<char*>(&log_container.header_size), sizeof(header_size));
    file.read(reinterpret_cast<char*>(&log_container.header_version), sizeof(header_version));
    file.read(reinterpret_cast<char*>(&log_container.object_size), sizeof(object_size));
    file.read(reinterpret_cast<char*>(&log_container.object_type), sizeof(object_type));

    file.read(reinterpret_cast<char*>(&log_container.compression_method), sizeof(compression_method));
    file.read(reinterpret_cast<char*>(&log_container.reserved_log_container1), sizeof(reserved_log_container1));
    file.read(reinterpret_cast<char*>(&log_container.reserved_log_container2), sizeof(reserved_log_container2));
    file.read(reinterpret_cast<char*>(&log_container.uncompressed_file_size), sizeof(uncompressed_file_size));
    file.read(reinterpret_cast<char*>(&log_container.reserved_log_container3), sizeof(reserved_log_container3));
    log_container.compressed_file_size = log_container.object_size - log_container_internal_header_size();// LOGCONTAINER_INTERNAL_HEADER_SIZE;
    file.read(reinterpret_cast<char*>(log_container.compressed_file.data()), log_container.compressed_file_size);
    file.seekg(object_size % 4, std::ios_base::cur);
}

void LogContainer::write(AbstractFile& os)
{
    os.write(reinterpret_cast<char*>(&signature), sizeof(signature));
    os.write(reinterpret_cast<char*>(&header_size), sizeof(header_size));
    os.write(reinterpret_cast<char*>(&header_version), sizeof(header_version));
    os.write(reinterpret_cast<char*>(&object_size), sizeof(object_size));
    os.write(reinterpret_cast<char*>(&object_type), sizeof(object_type));

    os.write(reinterpret_cast<char*>(&compression_method), sizeof(compression_method));
    os.write(reinterpret_cast<char*>(&reserved_log_container1), sizeof(reserved_log_container1));
    os.write(reinterpret_cast<char*>(&reserved_log_container2), sizeof(reserved_log_container2));
    os.write(reinterpret_cast<char*>(&uncompressed_file_size), sizeof(uncompressed_file_size));
    os.write(reinterpret_cast<char*>(&reserved_log_container3), sizeof(reserved_log_container3));
    os.write(reinterpret_cast<char*>(compressed_file.data()), compressed_file_size);

    os.skipp(object_size % 4);
}

void LogContainer::compress(const uint16_t compressionMethod, int compressionLevel)
{
    this->compression_method = compressionMethod;
    switch (compressionMethod)
    {
    case 0:
        compressed_file = uncompressed_file;
        compressed_file_size = uncompressed_file_size;
        break;
    case 2:
        {
            uLong size = compressBound(uncompressed_file_size);
            int ret = ::compress2(compressed_file.data(),
                                    &size,
                                    uncompressed_file.data(),
                                    uncompressed_file_size,
                                    compressionLevel);
            if (ret != Z_OK)
                throw std::runtime_error("compression failed");
            compressed_file_size = static_cast<uint32_t>(size);
            break;
        }
    default:
        break;
    }
}

void LogContainer::uncompress()
{
    switch (compression_method)
    {
    case 0:
        uncompressed_file = compressed_file;
        break;
    case 2:
        {
            uLong size = uncompressed_file_size;
            int ret = ::uncompress( uncompressed_file.data(), &size, compressed_file.data(), compressed_file_size);
            if (size != static_cast<uLong>(uncompressed_file_size))
                throw std::runtime_error("compression failed");
            if (ret != Z_OK)
                throw std::runtime_error("uncompression failed");
            break;
        }
    default:
        throw std::runtime_error("uncompression failed");
        break;
    }
}

uint32_t LogContainer::log_container_internal_header_size() const
{
    return object_header_base_header_size() +
        sizeof(compression_method) +
        sizeof(reserved_log_container1) +
        sizeof(reserved_log_container2) +
        sizeof(uncompressed_file_size) +
        sizeof(reserved_log_container3);
}

uint32_t LogContainer::log_container_object_size() const
{
    return log_container_internal_header_size() + compressed_file_size;
}

BLFEND
