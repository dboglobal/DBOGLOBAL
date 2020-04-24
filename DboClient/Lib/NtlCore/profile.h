/*****************************************************************************
*
* File :     Profile.cpp
*
* Abstract : Real time profiling
*
*****************************************************************************
*
* This file is a product of Criterion Software Ltd.
*
* This file is provided as is with no warranties of any kind and is
* provided without any obligation on Criterion Software Ltd. or
* Canon Inc. to assist in its use or modification.
*
* Criterion Software Ltd. will not, under any
* circumstances, be liable for any lost revenue or other damages arising
* from the use of this file.
*
* Copyright (c) 2003 Criterion Software Ltd.
* All Rights Reserved.
*
* RenderWare is a trademark of Canon Inc.
*
*****************************************************************************/

//////////////////////////////////////////////////////////////////
//
// RenderWare Studio Includes
//
#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <rwcore.h>

namespace RWS
{
   /**
   *
   * \ingroup Profiler
   *
   * <BR> <B>How to use RenderWare Studio's Game Framework profiler.</B>   
   *
   * The purpose of this profiler is to perform function profiling.  This is important as it allows you to identify 
   * any bottle-necks in the code and easily spot functions that are taking longer than expected. The profiler 
   * creates a hierarchical tree of all the function calls in the Framework that contain the RWS_FUNCTION and 
   * RWS_RETURN/RWS_RETURNVOID macros. RWS_FUNCTION adds the current function to the  profiler tree and records a 
   * time stamp.  When the function has finished executing and calls RWS_RETURN\RWS_RETURNVOID, another time stamp is 
   * recorded and the total time in milliseconds is worked out.  When a function calls another function, it will 
   * add another node to the profiler tree, thus recording the hierarchy.
   *
   * To enable the profiler,  you must add the RWS_FUNCTIONPROFILE pre-processor command to your project settings. For 
   * Visual Studio (Pc, Xbox, and PS2/Gamecube ProDG) this is done by opening up the project settings 
   * (Project->Settings), and then clicking on the C/C++ tab and then select 'preprocessor' from the drop down box. 
   * Then add RWS_FUNCTIONPROFILE to the end of the list of preprocessor definitions.
   * 
   * For Metroworks build targets (PS2/Gamecube Code Warrior), the RWS_FUNCTIONPROFILE should be added to the prefix 
   * file.
   *
   * When the Framework is connected to the Workspace,  The profile data for the current node in the tree is sent 
   * across the network from the Framework to the Workspace.  This data is buffered up and sent every half a 
   * second.  The reason for this is to reduce the amount of time the network is accessed.
   *
   * Within the Workspace (in the metrics layout) you will find the profiler control.  After connecting to the 
   * Framework, the profiler control will show the connection ID for that connection.  If you right click on that
   * connection ID and select 'View', another window will be displayed showing the profiler data.
   *
   * The initial profile data shown will be the root node of the hierarchy.  The data is split into 5 columns.
   * They are:
   *
   *        Function.       This displays the name of the function being profiled.
   *
   *        Calls.          This shows how many times that function has been called so far.
   *
   *        Time this node. This shows how long this function has taken to execute, minus the time of any other 
   *                        functions called within this function.
   *
   *        Child time.     This shows how long the children of this function have taken to execute.
   *
   *        Total time.     This shows how long this function has taken to execute. This time includes the time of its
   *                        children.
   *
   *        % Total time.   This shows the percentage of time that this function has taken out of the overall time the
   *                        Framework has been running.
   *
   * You can enter any of the children by double clicking the mouse on one of the entries, or right clicking and 
   * selecting 'Enter Profile Child'.  If the function does not contain any functions which have been profiled, you 
   * will not be able to enter it.  To return to a nodes parent, right click in the list view and select 
   * 'Go to Parent'.
   *
   * If you click on any of the profiler entries in the list view, you will see the filename and line number from were
   * that function call originated.  If you right click on that entry and select 'Go to Source'. Visual studio will
   * be opened and and you will be taken to the correct place in the source code.
   *
   */
   namespace FunctionProfile
   {
      /**
      *
      * \ingroup Profiler
      *
      * EnterChild_Command is a command that gets sent from the Workspace to instruct the profiler class to 
      * enter a new child node of the profiler tree.
      *
      */
      const RwUInt32 EnterChild_Command    = 0;

      /**
      *
      * \ingroup Profiler
      *
      * EnterParent_Command is a command that gets sent from the Workspace to instruct the profiler class to 
      * enter the parent node of the current child.
      *
      */
      const RwUInt32 EnterParent_Command   = 1;

      /**
      *
      * \ingroup Profiler
      *
      * ProfilerReset_Command is a command that gets sent from the Workspace to instruct the profiler class to 
      * reset the profiler timings to 0.  This does not detroy the tree.
      *
      */
      const RwUInt32 ProfilerReset_Command = 2;

#ifdef RWS_FUNCTIONPROFILE
      /**
      *
      * \ingroup Profiler
      *
      * The rate in milliseconds in-which profile data is sent back to the Workspace.
      *
      */
      const RwUInt32 BROADCASTRATE = 500;

      /**
      *
      * \ingroup Profiler
      *
      * The number of milliseconds that equals 1 second.
      *
      */
      const RwReal MILLISECONDS_TO_SECONDS = 1000.0f;

      /**
      *
      * \ingroup Profiler
      *
      * This class represents a node in the profile hierarchy tree.  Each new function  that gets profiled
      * has a new node allocated.  These nodes connect together to form a tree.  This tree represents 
      * a call tree showing the structure of the code being profiled.  
      *
      */
      class CProfileNode 
      {         
      public:
         CProfileNode( char * name, CProfileNode * parent );
         ~CProfileNode( void ){};
                  
         /**
         *
         * Returns a pointer to this nodes parent node.
         *
         * \return Returns a pointer to this nodes parent node.
         *
         */         
         CProfileNode * GetParent( void ){return m_pParent; };     
         
         /**
         *
         * Return a pointer to this nodes sibling.
         *
         * \return Return a pointer to this nodes sibling.
         *
         */
         CProfileNode * GetSibling( void ){ return m_pSibling; };
         
         /**
         *
         * Return a pointer to this nodes child.
         *
         * \return Return a pointer to this nodes child.
         *
         */
         CProfileNode * GetChild( void ){ return m_pChild; };          
                  
         /**
         *
         * Get the name of the function associated with this node.
         *
         * \return Returns a pointer to the function name.     
         *
         */
         RwChar * GetName( void ){ return m_pName; };
         
         /**
         *
         * Return the total number of times the function associated with this node has been called.
         *
         * \return Return the number of times the function has been called.
         *
         */
         RwUInt32 GetTotalCalls( void ) { return m_TotalCalls; }  

         /**
         *
         * Return the total amount of time spent in the function associated with this node.
         *
         * \return Returns the amount of time spent in the function in milliseconds.
         *
         */
         RwReal GetTotalTime( void ){ return m_TotalTime; };
                    
         /**
         *
         * Return the file name from the source code for the function associated with this node.
         *
         * \return A pointer to a string containing the filename.      
         *
         */
         RwChar * GetFileName( void ) { return m_pFileName; };
         
         /**
         *
         * Return the line number from the source code for the function associated with this node.
         *
         * \return A line number
         *
         */         
         RwUInt32 GetLineNumber( void ){ return m_LineNumber; };
         
         /**
         *
         * Set the number of times results are accumulated before being sent across the network.
         *
         * \param num Number to set accumulated count too.
         *
         */
         void SetNumAccumulatedResults( const RwUInt32 num) { m_NumAccumulatedResults = num; };
         
         /**
         *
         * Return the number of times profiler results are accumulated before being sent across the network.
         *
         * \return Number of times profiler results are accumulated.
         *
         */
         RwUInt32 GetNumAccumulatedResults( void ) { return m_NumAccumulatedResults; };
         
         /**
         *
         * Accumulate a functions number of call.
         *
         * \param num Number to set accumulated calls too.
         *
         */
         void SetNumAccumulatedCalls( const RwUInt32 num) { m_AccumulatedCalls = num; };
         
         /**
         *
         * Return the number of accumulated calls.
         *
         * \return Returns the number of calls.        
         *      
         */         
         RwUInt32 GetNumAccumulatedCalls( void ) { return m_AccumulatedCalls; };    
         
         /**
         *
         * Accumulate a functions total execution time.
         *
         * \param num A functions time.
         *
         */         
         void SetNumAccumulatedTime( const RwReal num) { m_AccumulatedTime = num; }; 

         /**
         *
         * Return the accumulated function time.
         *
         * \return Returns the accumulated function time in milliseconds.
         *   
         */         
         RwReal GetNumAccumulatedTime( void ) { return m_AccumulatedTime; };     
         
         /**
         *
         * Accumulate a functions child execution times.
         *
         * \param num A child functions time in milliseconds.
         *
         */         
         void SetNumAccumulatedChildTime( const RwReal num){ m_AccumulatedChildTimes = num; }; 
         
         /**
         *
         * Return the accumulated functions child times.
         *
         * \return Returns the accumulated functions child times.
         * 
         */         
         RwReal GetNumAccumulatedChildTime( void ){ return m_AccumulatedChildTimes; };  
         
         CProfileNode * GetSubNode( const char * name );         
         void           Reset( void );
         void           Call( const char *pFilename, const int Linenumber );
         RwBool         Return( void );
      protected:         
         RwChar       * m_pName;             /**< The name of the function being profiled.*/
         RwChar       * m_pFileName;         /**< The filename of the function being profiled.*/
         RwUInt32       m_LineNumber;        /**< The source code line number of the function being profiled.*/
         
         RwUInt32       m_TotalCalls;        /**< The total number of times the function has been called.*/
         RwReal         m_TotalTime;         /**< The total amount of time spent in the function.*/
         RwUInt32       m_StartTime;         /**< Time sample for when the function being profiled begins execution.*/
         RwUInt32       m_RecursionCounter;  /**< Counter for keeping track of recursion.*/

         RwUInt32       m_AccumulatedCalls;     /**< The call counter that gets accumulated up before being sent 
                                                     across the network.*/

         RwReal         m_AccumulatedTime;      /**< The node time that gets accumulated up before being sent 
                                                     across the network.*/
         
         RwReal         m_AccumulatedChildTimes;/**< The child time that gets accumulated up before being send 
                                                     across the network.*/
         
         RwUInt32       m_NumAccumulatedResults;/**< The number of items accumulated before being sent across the 
                                                     network.*/
        
         CProfileNode * m_pParent;           /**< A pointer to this nodes parent.*/
         CProfileNode * m_pChild;            /**< A pointer to this nodes child.*/
         CProfileNode * m_pSibling;          /**< A pointer to this nodes siblings.*/
      };

      /**
      *
      * \ingroup Profiler
      *
      * An iterator to navigate through the profiler tree.
      *
      */
      class CProfileIterator
      {
      public:
         /**
         *
         * Get the first child of the current parent node.
         *
         * \see Next
         * \see IsDone
         *
         */
         void First(void) { m_pCurrentChild = m_pCurrentParent->GetChild(); };

         /**
         *
         * Get the next sibling of the current child.
         *
         * \see First
         * \see IsDone
         *
         */
         void Next(void) { m_pCurrentChild = m_pCurrentChild->GetSibling(); };
         
         /**
         *
         * If the current child is NULL then return TRUE.  This function is used for iterating through 
         * the profiler tree.
         *
         * \see First
         * \see Next, 
         *
         */
         RwBool IsDone(void) { return m_pCurrentChild == 0; };
         
         /**
         *
         * Return the name of the current child.
         *
         * \param A pointer to a string containing the child names.
         *
         */
         const RwChar * GetCurrentName( void ) { return m_pCurrentChild->GetName(); };
         
         /**
         *
         * Return the total number of times the current nodes function has been called.
         *
         * \return Returns the total number of times the current child function has been called.
         *
         */
         RwUInt32 GetCurrentTotalCalls( void ) { return m_pCurrentChild->GetTotalCalls(); };
         
         /**
         *
         * Return the total time which has been spent in the current child function.
         *
         * \return Return the total time spent in the current child function.
         *
         */         
         RwReal GetCurrentTotalTime( void ) { return m_pCurrentChild->GetTotalTime(); };
         
         /**
         *
         * Return the filename from were the current child function appears in the source code.
         *
         * \return Returns a string containing a filename.
         *
         */
         RwChar * GetCurrentFileName( void ) { return m_pCurrentChild->GetFileName(); };          
         
         /**
         *
         * Return the line number from were the current child function appears in the source code.
         *
         * \return Return the filename from were the current child function appears in the source code.
         *
         */
         RwUInt32 GetLineNumber( void ) { return m_pCurrentChild->GetLineNumber(); };                   
         
         /**
         *
         * Return the name of the current parent.
         *
         * \param A pointer to a string containing the parents names.
         *
         */
         const RwChar * GetCurrentParentName( void ) { return m_pCurrentParent->GetName(); }; 
         
         /**
         *
         * Return the total number of times the current parents function has been called.
         *
         * \return Returns the total number of times the current parents function has been called.
         *
         */
         RwUInt32 GetCurrentParentTotalCalls( void ) { return m_pCurrentParent->GetTotalCalls(); };
         
         /**
         *
         * Return the total time which has been spent in the current parents function.
         *
         * \return Return the total time spent in the current parents function.
         *
         */
         RwReal GetCurrentParentTotalTime( void ) { return m_pCurrentParent->GetTotalTime(); };  

         void           EnterChild( RwUInt32 Index );
         void           EnterParent( void );

      protected:
         CProfileNode * m_pCurrentChild;           /**< A pointer to the current child node.*/
         CProfileNode * m_pCurrentParent;          /**< A pointer to the current parent node.*/
         
         CProfileIterator( CProfileNode * start );
         friend   class    CProfileManager;
      };      

#else
      class CProfileIterator
      {
      };
#endif
      
      /**
      *
      * \ingroup Profiler
      *
      * Manager class for the profiler.  This class controls the profiling and the construction of the node tree.
      * When a  function gets profiled, the start profile function is called.  If the function being profiled is not
      * already in the tree, a new node is allocated.  If it is already in the tree, then it's node is updated.
      *
      * This class also contains a pointer to an iterator that is used by the SendProfileDataToWorkspace function to
      * send profile data to the Workspace.
      *
      * This manager class also contains a memory block which gets allocated by the OpenProfiler function. The purpose
      * of this memory block is to hold all of the profiler nodes that get allocated.
      *
      */
      class CProfileManager
      {
#ifdef RWS_FUNCTIONPROFILE
      public:
         static void                 OpenProfiler (RwUInt32 NumNodes, RwUInt32 (*pTimeFunc)(void));
         static void                 CloseProfiler ();
       

         static void                 StartProfile( const RwChar * pName, const char *pFilename, const int Linenumber );
         static void                 StopProfile( void );  
         
         static void                 Reset( void );
         static void                 IncrementFrameCounter( void );
         
         static RwUInt32             GetFrameCountSinceReset( void );
         static RwReal               GetTimeSinceReset( void );
         static CProfileIterator   * GetIterator( void );
         static void                 ReleaseIterator( CProfileIterator * pIterator );
         static void                 SendProfileDataToWorkspace( void );
         
         static CProfileIterator   * pIttr;                  /**< Pointer to an itterator for the profiler manager.*/
         static char               * m_NodeBuffer;           /**< A pointer to an area of memory that gets allocated. 
                                                                  This area of memory is for storing the profiler 
                                                                  nodes. */

         static char               * m_NodeBufferOriginalPtr;/**< A pointer to the start of the node memory buffer.*/

         static RwUInt32             m_NumNodesInBuffer;     /**< The number of nodes in the memory buffer.*/
         static RwUInt32             m_NumNodesAllocated;    /**< The total number of nodes that can be allocated into 
                                                                  the memory buffer.*/
      private:
         static RwReal               TotalChildTimes( void );
         static void                 AccumulateProfileTimes( void );
         static void                 SendAccumulatedData( void );
         
         static CProfileNode         m_Root;                /**< The root node in the profiler tree.*/
         static CProfileNode       * m_pCurrentNode;        /**< A pointer to the current node.*/
         static RwUInt32             m_FrameCounter;        /**< The number of frames elapsed since the profiler 
                                                                 started, or was reset,*/
         static RwUInt32             m_ResetTime;           /**< The time when the profiler was reset.*/
#else
      public:
         static void                 OpenProfiler (RwUInt32 NumNodes, RwUInt32 (*pTimeFunc)(void)){};
         static void                 CloseProfiler (){};
         static void                 StartProfile( const char * pName, const char *pFilename, const int Linenumber ){};
         static void                 StopProfile( void ){};
         static void                 Reset( void ){};
         static void                 IncrementFrameCounter( void ){};
         static RwUInt32             GetFrameCountSinceReset( void ){return 0;};
         static RwReal               GetTimeSinceReset( void ){return 0.0f;};
         static CProfileIterator   * GetIterator( void ){return 0;};
         static void                 ReleaseIterator( CProfileIterator * pIterator ){};
         static void                 SendProfileDataToWorkspace( void ){};
      private:
         static RwReal               TotalChildTimes( void ){return 0.0f;};
         static void                 AccumulateProfileTimes( void ){};
         static void                 SendAccumulatedData( void ){};         
#endif
      };

   }  // Profile
} // RWS


#endif // __PROFILE_H__
