
class iTestBed;
class iPathEngine;
class iAgent;
class iCollisionContext;
class iCollisionInfo;

void TurnAgentUnderMouseControl(iTestBed* testbed, float& agent_heading);
void TurnAgentUnderMouseControl(iTestBed* testbed, float& agent_heading, float& camera_elevation);
void MoveAgentUnderKeyControl(iTestBed* testbed, iPathEngine* pathengine,
                                iAgent* agent, float agent_heading, 
                                iCollisionContext* context,
                                float speed,
                                float& precisionX, float& precisionY
                                );
void MoveAgentUnderKeyControl(iTestBed* testbed, iPathEngine* pathengine,
                                iAgent* agent, float agent_heading, 
                                iCollisionContext* context,
                                float speed);

void GetMovementVector(iTestBed* testbed, float& x, float& y);
void MoveAgent(iPathEngine* pathengine, iCollisionContext* context,
                      iAgent* agent,
                      float agentHeading, float localX, float localY,
                      float& precisionX, float& precisionY
                      );
void MoveAgent(iPathEngine* pathengine, iCollisionContext* context1, iCollisionContext* context2,
                      iAgent* agent,
                      float agentHeading, float localX, float localY,
                      float& precisionX, float& precisionY
                      );
