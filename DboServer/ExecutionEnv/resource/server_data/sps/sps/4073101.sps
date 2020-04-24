--- 예전 전직에 사용된 SPS          ---
--- 2011년 05월 13일 현재 사용 안함 ---
Scene(1, "6013")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
		Param( "attack disallow", 1 )
		Param( "invincible", 1 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "timid" )
	--]
	End()

	Action( "wait" )
	--[
		Param( "event type", "wps" )
		Param( "event id", 601301 )
	--]
	End()

--	Action( "say" )
	--[
--		"새로운 도전자인가 보군"
	--]
--	End()

	Action( "wait" )
	--[
		Param( "time", 6 )
	--]
	End()

--	Action( "say" )
	--[
--		 "어디 실력을 한번 볼까?" 
	--]
--	End()

	Action( "char condition" )
	--[
		Param( "attack disallow", 0 )
		Param( "invincible", 0 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "offensive" )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()

Scene(2, "6013")
--[
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
		Param( "attack disallow", 1 )
		Param( "invincible", 1 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "timid" )
	--]
	End()

	Action( "wait" )
	--[
		Param( "event type", "wps" )
		Param( "event id", 601302 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

--	Action( "say" )
	--[
--		"응? 아직 어린아이잖아,"
	--]
--	End()

	Action( "wait" )
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "char condition" )
	--[
		Param( "attack disallow", 0 )
		Param( "invincible", 0 )
	--]
	End()

	Action( "ai bit flag" )
	--[
		Param( "attack type", "offensive" )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()
