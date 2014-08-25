
#ifndef NUMCPP_MANAGER_H
#define NUMCPP_MANAGER_H

#include <memory>

namespace numcpp
{

class Manager
{
public:
    
    typedef std::shared_ptr<Manager> Ptr;
    
    Manager(const Manager&) = delete;
    
    virtual unsigned char* data() const = 0;
    
protected:
    explicit Manager() {}
    virtual ~Manager()
    {
        #ifndef NDEBUG
        std::cout << "Manager deleted" << std::endl;
        #endif
    }
};

class SimpleManager : public Manager
{
private:
    std::unique_ptr<unsigned char[]> _array;
    
    explicit SimpleManager(size_t size)
        : _array(new unsigned char[size])
    {}
    
public:
    
    static Manager::Ptr allocate(size_t size)
    {
        Manager::Ptr r(new SimpleManager(size));
        return r;
    }
    
    virtual unsigned char* data() const {return _array.get();}    
};

struct NullOwner {};

template<typename U>
class ExternalManager : public Manager, private U
{
public:
    typedef U Owner;
    
    static Manager::Ptr build(unsigned char* data, const Owner& owner)
    {
        Manager::Ptr r(new ExternalManager(data, owner));
        return r;
    }
    
    virtual unsigned char* data() const {return _data;}
    
private:
    explicit ExternalManager(unsigned char* data, const Owner& owner)
        : Owner(owner), _data(data)
    {}
    unsigned char* _data;
};

}

#endif
