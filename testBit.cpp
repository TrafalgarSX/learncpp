#include <iostream>

typedef uint8_t Bit;

class File{
    Bit mode : 2;
    Bit modified : 1;
    Bit prot_owner : 3;
    Bit prot_group : 3;
    Bit prot_world : 3;
public:
    enum modes {READ = 01, WRITE = 02, EXECUTE = 03};
    File();
    ~File();
    File& open(File::modes m);
    void close();
    void read(Bit* buffer, int size);
    void write(Bit* buffer, int size);
};

void File::write(Bit* buffer, int size){
    modified = 1;
}

File &File::open(File::modes m){
    mode |= READ;
    if(m & WRITE){

    }
    return *this;
}