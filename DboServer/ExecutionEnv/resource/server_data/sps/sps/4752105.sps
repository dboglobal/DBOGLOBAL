--- 예전 전직에 사용된 SPS          ---
--- 2011년 05월 13일 현재 사용 안함 ---
Scene( 1, "1" )
--[	
	Action( "escort" )
	--[    
		Param( "type", "under escort")      
		
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 1)      
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3040 ) 
	--]     
	End()


	Action( "wait" )
	--[
		Param( "time", 10 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 603101)
		Param( "run mode", 1 )
	--]
	End()

--	Action( "say" )
	--[
--		 "앞길이 걱정이군요." 
	--]     
--	End()

	Action( "wait" )
	--[
		Param( "time", 4 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 603102)
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3041 ) 
	--]     
	End()

	Action( "wait" )
	--[
		Param( "time", 5 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3042 ) 
	--]     
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 603103)
		Param( "run mode", 1 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3043 ) 
	--]     
	End()

	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	Action( "say" )
	--[
		Param( "tblidx", 3065 ) 
	--]     
	End()

	Action( "send sps event" )
	--[
		Param( "event type", "escort") 
		Param( "trigger type", "quest") 
		Param( "event id", 603201)      
	--]
	End()

	Action( "char condition" )
	--[
		Param( "ui disable", 0)      
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 10 )
	--]
	End()

	Action( "leave" )
	--[    

	--]
	End()
--]
End()