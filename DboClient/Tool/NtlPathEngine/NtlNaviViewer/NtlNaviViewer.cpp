// NtlNaviViewer.cpp : DLL 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector>
#include <math.h>
#include <map>
#include <windows.h>
#include <commdlg.h>


#include <shlwapi.h>
#pragma comment( lib, "shlwapi.lib" )

// FrameWork
#include "MoveAgent.h"
#include "WaitForFrame.h"
#include "Sliding.h"
#include "EntryPoint.h"

#define dFILENAME "\\NtlNaviViewer.ini"

#include "PerfomanceChecker.h"

char g_chMousePosition[256] = {0,};
char g_chFileName[MAX_PATH] = {0,};

// Test용 FirstColliding
cPosition g_FirstColliding;
cPosition g_LineStart;
cPosition g_LineEnd;

void demo(iPathEngine *pathengine, iTestBed *testbed)
{
	HWND hWnd = (HWND)testbed->getWindowHandle();
	//OPENFILENAME fisier;
	//BOOL bret;
	//char NumeFisier[ 1000 ]="";
	//memset (&fisier , 0 , sizeof (fisier) );
	//fisier.lStructSize = sizeof(fisier);
	//fisier.Flags = OFN_HIDEREADONLY ;
	//fisier.lpstrFile = NumeFisier;
	//fisier.nMaxFile = 1000 ;
	//fisier.lpstrFilter = "Fisier de tip text(*.txt)\0*.txt;*.text\0Fisiere de tip C\0*.c;*.cpp\0";
	//bret = GetOpenFileName(&fisier) ;
	//if( bret == FALSE )
	//{
	//	printf("Nimic selectat");
	//	return;
	//}

	char Path[MAX_PATH];
	GetModuleFileName(NULL, Path, MAX_PATH);

	PathRemoveFileSpec(Path);

	char INIPath[MAX_PATH];
	sprintf_s( INIPath, MAX_PATH, "%s\\%s", Path, dFILENAME );

	char FileName[256];
	GetPrivateProfileString("OPEN", "filename", "0.mf", FileName, 256, INIPath );

	sprintf_s( g_chFileName, 256, "%s\\%s", Path, FileName );

	int nRadius = 100;
	nRadius = GetPrivateProfileInt("OPEN", "agent_radius", 100, INIPath );

	bool exitflag=false;

	iShape* agent_shape;
	{
		long array[] =
		{
			nRadius, 0,
			0, -nRadius,
			-nRadius, 0,
			0, nRadius,
		};

		agent_shape = pathengine->newShape( sizeof(array)/sizeof(*array)/2, array );
	}
	
	char filenameBuf[256];

	iMesh* mesh;
	{
		char *buffer;
		unsigned long size;
		buffer = testbed->loadBinary(FileName, size);
		mesh = pathengine->loadMeshFromBuffer("tok", buffer, size, 0);
		testbed->freeBuffer(buffer);
	}

	testbed->setMesh(mesh); // set this mesh as the current mesh for rendering
	testbed->zoomExtents(); // place camera to see all of newly loaded mesh

	// generate preprocess for the agent shape

	mesh->generateCollisionPreprocessFor(agent_shape, 0);
	mesh->generatePathfindPreprocessFor(agent_shape, 0);

	// iAgent objects are used to instantiate shapes on a mesh - either as true agents or as obstructions

	//// values are initialised to 0 to indicate that nothing has yet been placed
	iAgent* agent = 0;
	float agentPrecisionX = 0.f, agentPrecisionY = 0.f;
	
	//std::vector<iAgent*> obstruction_agents;
	//size_t i;
	//for(i = 0; i < obstruction_shapes.size() - 1; i++)
	//{
	//	obstruction_agents.push_back(0);
	//}

	// iCollisionContext objects encapsulate a state of collision on a mesh - essentially this is a collection of agents
	iCollisionContext* context = mesh->newContext();

	// target position for pathfinding
	cPosition agent_target;
	// path to target
	iPath* path = 0;

	// direction agent is currently facing
	float agent_heading=0;

	bool drawExpansion = false;
	bool paused = false;

	while(!exitflag)
	{		
		testbed->setColourRGB(0.0f, 0.0f, 1.0f);
		testbed->drawMeshEdges();

		testbed->setColourRGB(1.0f, 1.0f, 1.0f);
		testbed->drawBurntInObstacles();

		testbed->setColourRGB(0.5f, 0.5f, 0.5f);
		testbed->drawLine( g_LineStart, g_LineEnd );

		if(drawExpansion)
		{
			testbed->setColour("orange");
			testbed->drawMeshExpansion(agent_shape);
			testbed->drawSmallConvexExpansions(agent_shape);
		}

		// determine closest unobstructed point to agent target if necessary,
		// draw target and closest unobstructed point as appropriate
		cPosition agent_target_closest_unobstructed_point;
		if(agent_target.cell!=-1)
		{
			if(mesh->testPointCollision(agent_shape,context,agent_target))
			{
				testbed->setColour("red");
				testbed->drawPosition(agent_target,200);
				agent_target_closest_unobstructed_point=mesh->findClosestUnobstructedPosition(
					agent_shape,
					context,
					agent_target,
					20);
				testbed->setColour("orange");
				if(agent_target_closest_unobstructed_point.cell!=-1)
					testbed->drawPosition(agent_target_closest_unobstructed_point,150);
			}
			else
			{
				testbed->setColour("orange");
				testbed->drawPosition(agent_target,200);
				agent_target_closest_unobstructed_point=agent_target;
			}
		}

		// draw agent if it has been placed
		if(agent)
		{
			testbed->setColour("white");
			testbed->drawAgentWithPrecision(agent, 200, agentPrecisionX, agentPrecisionY);
			testbed->setColour("orange");
			testbed->drawAgentHeadingWithPrecision(agent, 300, 200,agent_heading, agentPrecisionX, agentPrecisionY);

			// pathfind to target if it has been placed
			if(agent_target_closest_unobstructed_point.cell!=-1 && path==0)
			{
				path = agent->findShortestPathTo(context,agent_target_closest_unobstructed_point);
			}
		}

		// draw path
		testbed->setColour("green");
		testbed->drawPath(path);

		// draw Position
		testbed->setColour( "white" );
		testbed->printTextLine( 16, g_chFileName );
		testbed->printTextLine( 16, g_chMousePosition );

		// tell the testbed to render this frame
		testbed->update();

		WaitForFrame();

		if(agent)
		{
			if(path && !paused)
			{
				if(path->size()>=2)
				{
					// set heading for the current path section
					cPosition next_target=path->position(1);
					cPosition current=agent->getPosition();
					long dx,dy;
					dx=next_target.x-current.x;
					dy=next_target.y-current.y;
					agent_heading = static_cast<float>(atan2(static_cast<double>(dx),static_cast<double>(dy)));
				}
				agent->advanceAlongPathWithPrecision(path, 100.f, context, agentPrecisionX, agentPrecisionY);
			}
			else
			{
				TurnAgentUnderMouseControl(testbed,agent_heading);
				MoveAgentUnderKeyControl(testbed, pathengine,
					agent, agent_heading, context, 100.f,
					agentPrecisionX, agentPrecisionY);
			}
		}

		// receive and process messages for all keys pressed since last frame
		const char *keypressed;
		while(keypressed=testbed->receiveKeyMessage())
		{
			if(keypressed[0]!='d')
			{// is it a key down message?


				switch(keypressed[1])
				{
				case '_':
					{
						if(!strcmpi("ESCAPE",keypressed+2))
							exitflag=true;
						break;
					}
				case '4':
					{
						// move or place the agent
						cPosition p=testbed->positionAtMouse();
						if(p.cell!=-1 && !mesh->testPointCollision(agent_shape,context,p))
						{
							if(!agent)
								agent=mesh->placeAgent(agent_shape,p);
							else
								agent->moveTo(p);
							agent_target.cell = -1;
							delete path;
							path = 0;
						}
					}
					break;
				case '5':
					{
						// set a target for pathfinding
						cPosition p=testbed->positionAtMouse();
						if(p.cell!=-1)
							agent_target=p;
						delete path;
						path = 0;
					}
					break;
					// Test
				case 'Q':
					{
						cPosition posSrc = agent->getPosition();
						cPosition posTarget;/* = testbed->positionAtMouse();*/
						posTarget.x = 1000;
						posTarget.y = -8900;
						/*if(posTarget.cell != - 1)*/
						{
							iCollisionInfo* pInfo = mesh->firstCollision( agent_shape, context, posSrc, posTarget.x, posTarget.y, posTarget.cell );
							if( pInfo )
							{
								long coords[4];
								pInfo->getCollidingLine(coords);
								delete pInfo;

								tSigned32 arCoord[3];
								arCoord[0] = (tSigned32)coords[0];
								arCoord[1] = (tSigned32)coords[1];
								arCoord[2] = 0;
								cPosition srcPosition = mesh->positionNear3DPoint( arCoord, 2000, 50000 );

								arCoord[0] = (tSigned32)coords[2];
								arCoord[1] = (tSigned32)coords[3];
								arCoord[2] = 0;
								cPosition destPosition = mesh->positionNear3DPoint( arCoord, 2000, 50000 );

								g_LineStart = srcPosition;
								g_LineEnd = destPosition;
							}
						}
					}
					break;
				case 'W':
				case 'A':
				case 'S':
				case 'D':
					// a direct control key has been pressed
					// delete path
					// control will revert to user control
					agent_target.cell = -1;
					delete path;
					path = 0;
					break;
				case 'E':
					drawExpansion = !drawExpansion;
					break;
				case 'P':
					paused = !paused;
					break;
				case 'Y':
					{
						cPosition p = testbed->positionAtMouse();
						if( p.cell != -1 )
						{
							float fY = mesh->heightAtPositionF( p );
							sprintf_s( g_chMousePosition, 256, "X: %0.2f Y: %0.2f Z: %0.2f", ((float)p.x*0.01f), ((float)fY*0.01f), ((float)p.y*0.01f) );
						}
						else
						{
							sprintf_s( g_chMousePosition, 256, "%s X: %0.2f Y : %0.2f ", "Invalid position ->", ((float)p.x*0.01f), ((float)p.y*0.01f) );
						}
					}
					break;
				case 'Z':
					{
						testbed->zoomExtents();
					}
					break;
				}
			}
		}
	}
}
