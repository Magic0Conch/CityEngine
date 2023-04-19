#pragma once
namespace EasyEngine {
    class Math{
    public:
        Math();
        
        template<class T>
        static T lerp(T a,T b,T f){
            return a + f*(b-a);
        }

    };

}