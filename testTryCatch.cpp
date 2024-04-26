#include <iostream>

int main()
{
    try
    {
        throw 1;
        // throw 1.1;
    }
    catch (int)
    {
        std::cout << "catch int" << std::endl;
        try{
            throw;
        }
        catch (int)
        {
            std::cout << "catch int again" << std::endl;
        }
    }
    catch (...)
    {
        std::cout << "catch ..." << std::endl;
    }
    return 0;
}