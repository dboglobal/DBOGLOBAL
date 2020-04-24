
#include <vector>
#include <map>
class iShape;
class cSimpleDOM;

class cResources
{
    std::vector<iShape*> _shapes;    

public:

    cResources(const cSimpleDOM& shapesScript);
    ~cResources();

    iShape* getShape(long index)
    {
        return _shapes[index];
    }

    enum eSplashScreen
    {
        SPLASH_BEGIN,
        SPLASH_DIED,
        SPLASH_LEVELCOMPLETE,
        SPLASH_ALLCOMPLETE,
        SPLASH_CAMERA,
    };
};
