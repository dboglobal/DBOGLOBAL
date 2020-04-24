
#ifndef I_BEHAVIOUR_INCLUDED
#define I_BEHAVIOUR_INCLUDED

class cGameState;
class cGameObject;
class cSimpleDOM;

class iBehaviour
{
protected:

    iBehaviour() {}

public:

    virtual ~iBehaviour() {}
    virtual bool update(cGameObject& controlledObject) = 0;
    virtual void draw(cGameObject& controlledObject)
    {
    }

    static iBehaviour* create(const cSimpleDOM& element, cGameObject& controlledObject);
    static iBehaviour* create_IgnoreDuration(const cSimpleDOM& element, cGameObject& controlledObject);
};

class cNullBehaviour : public iBehaviour
{
public:

    bool update(cGameObject& controlledObject)
    {
        return false;
    }
};

#endif
