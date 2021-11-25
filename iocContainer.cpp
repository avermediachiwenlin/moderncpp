#include <string>
#include <map>
#include <memory>
#include <functional>

using namespace std;
template<calss T>
class IocContainer
{
    map<string, std::function<T*()>> m_creatorMap;
public:
    IocContainer(void) {}
    ~IocContainer(void) {}
    template <class Drived>
    void RegisterType(string strKey)
    {
        std::function<T*()> function = [] {return new Drived(); };
        RegisterType(strKey, function);
    }
    T* Resolve(string strKey)
    {
        if(m_creatorMap.find(strKey) == m_creatorMap.end())
        {
            return nullptr;
        }
        
        std::function<T* ()> function = m_creatorMap[strKey];
        return function;
    }
    std::shared_ptr<T> ResolveShared(string strKey)
    {
        T* ptr = Resolve(strKey);
        return std::shared_ptr<T>(ptr);
    }
private:
    void RegisterType(string strKey, std::function<T* ()> creator)
    {
        if(m_creatorMap.find(strKey)) != m_creatorMap.end())
        {
            throw std::invalid_argument("this key has already exist!");
        }
        
        m_creatorMap.emplace(strKey, creator);
    }
};

// 測試程式碼 (Test cases)
struct ICar
{
    virtual ~ICar() {}
    virtual void test() const = 0;
};

struct Bus: ICar
{
    Bus() {};
    void test() const { std::cout << "Bus::test()"; }
}

struct Car: ICar
{
    Car() {}
    void test() const { std::cout << "Car::test()"; }
};

int main()
{
    IocContainer<ICar> carIoc;
    carIoc.RegisterType<Bus>("bus");
    carIoc.RegisterType<Car>("car");
    
    std::shared_ptr<ICar> bus = carIoc.ResolveShared("bus");
    bus->test();
    
    std::shared_ptr<ICar> car = carIoc.ResolveShared("car");
    car->test();
    
    return 0;
}
