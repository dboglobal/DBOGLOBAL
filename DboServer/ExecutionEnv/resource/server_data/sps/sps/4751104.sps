-- 2011년 05월 13일 현재 사용 안함 ---
-- 츠보와 구호대
Scene( 1, "1" )
	
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

--	Action( "say" )
	--[
--		 "자 출발 해보자."
	--]     
--	End()


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
--		 "이 앞길은 험난하다구" 
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

--	Action( "say" )
	--[
--		 "2번 패스 종료"
	--]     
--	End()

	Action( "wait" )
	--[
		Param( "time", 4 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 603103)
		Param( "run mode", 1 )
	--]
	End()

--	Action( "say" )
	--[
--		"도착 직후
	--]     
--	End()

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

End()