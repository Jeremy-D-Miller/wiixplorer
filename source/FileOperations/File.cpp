#include <cstring>
#include "File.hpp"

File::File()
{
    file_fd = NULL;
    mem_file = NULL;
    filesize = 0;
    Pos = 0;
}

File::File(const char * filepath, const char * mode)
{
    file_fd = NULL;
    open(filepath, mode);
}

File::File(const u8 * mem, int size)
{
    file_fd = NULL;
    open(mem, size);
}

File::~File()
{
    close();
}

int File::open(const char * filepath, const char * mode)
{
    close();

    file_fd = fopen(filepath, mode);
    if(!file_fd)
        return -1;

    fseek(file_fd, 0, SEEK_END);
    filesize = ftell(file_fd);
    rewind();

    return 0;
}

int File::open(const u8 * mem, int size)
{
    close();

    mem_file = mem;
    filesize = size;

    return 0;
}

void File::close()
{
    if(file_fd)
        fclose(file_fd);

    file_fd = NULL;
    mem_file = NULL;
    filesize = 0;
    Pos = 0;
}

int File::read(u8 * ptr, size_t size)
{
    if(file_fd)
    {
        int ret = fread(ptr, 1, size, file_fd);
        if(ret > 0)
            Pos += ret;
        return ret;
    }

    int readsize = size;

    if(readsize > (long int) filesize-Pos)
        readsize = filesize-Pos;

    if(readsize <= 0)
        return readsize;

    if(mem_file != NULL)
    {
        memcpy(ptr, mem_file+Pos, readsize);
        Pos += readsize;
        return readsize;
    }

    return -1;
}

int File::write(const u8 * ptr, size_t size)
{
    if(size < 0)
        return size;

    if(file_fd)
    {
        int ret = fwrite(ptr, 1, size, file_fd);
        if(ret > 0)
            Pos += ret;
        return ret;
    }

    return -1;
}

int File::seek(long int offset, int origin)
{
	int ret = 0;

    if(origin == SEEK_SET)
    {
        Pos = offset;
    }
    else if(origin == SEEK_CUR)
    {
        Pos += offset;
    }
    else if(origin == SEEK_END)
    {
        Pos = filesize+offset;
    }
    if(Pos < 0)
    {
        Pos = 0;
        return -1;
    }

    if(file_fd)
        ret = fseek(file_fd, Pos, SEEK_SET);

    if(mem_file != NULL)
    {
        if(Pos > (long int) filesize)
        {
            Pos = filesize;
            return -1;
        }
    }

	return ret;
}

