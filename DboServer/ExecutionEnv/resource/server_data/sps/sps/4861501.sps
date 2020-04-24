
Scene( 1, "34007" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	-- 이제 진정한 힘을 보여주마...
	Action( "say" )
	--[
		Param( "tblidx", 3381 )
	--]
	End()

	-- 마장 가멜란 중앙으로 이동
	Action( "point move" )
	--[
		Param( "loc x", -402.78 )
		Param( "loc y", 144.86 )
		Param( "loc z", 704.6 )
		Param( "dir x", -0.71 )
		Param( "dir z", -0.71 )
		Param( "run mode", 1 )
	--]
	End()

	Action( "send event to wps" )
	--[
		Param( "wps index", 34007 )
		Param( "event id", 992 )
	--]
	End()
	
	-- 내 안의 사악한 상념들아! 깨어나라!
	Action( "say" )
	--[
		Param( "tblidx", 3382 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()


Scene( 2, "34007" )
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	-- 으으으윽... 머리가... 아프다...
	Action( "say" )
	--[
		Param( "tblidx", 3383 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 10 )
	--]
	End()

	-- 기다려라... 곧 산산히 부숴주마...
	Action( "say" )
	--[
		Param( "tblidx", 3384 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 10 )
	--]
	End()
	
	-- 도대체 위크는 언제 오는 거지...
	Action( "say" )
	--[
		Param( "tblidx", 3385 )
	--]
	End()
	
	Action( "wait" )
	--[
		Param( "time", 10 )
	--]
	End()
	
--]
End()