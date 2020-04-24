--- 2011년 05월 13일 현재 사용 안함 ---
--Ver03

Scene(1, "43")
--[

	Action( "direct play" )
	--[
		Param( "tblidx", 60001 )
		Param( "event type", "ts" )
		Param( "event id", 9998 )
	--]
	End()
	
	Action( "direct play" )
	--[
		Param( "tblidx", 60002 )
	--]
	End()

	Action( "wait" )
	--[
		Param( "event type", "ts" )
		Param( "event id", 9997 )
	--]
	End()

--	Action( "say" )
	--[
--		감사감사 아리가또
	--]
--	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60003 )
		Param( "event type", "ts" )
		Param( "event id", 9995 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 60004 )
	--]
	End()

	Action( "wait")
	--[
		Param( "time", 2 )
	--]
	End()

--	Action( "say" )
	--[
--		덕분에 살았다능
	--]
--	End()

	Action( "wait")
	--[
		Param( "time", 2 )
	--]
	End()

--	Action( "say" )
	--[
--		"넌뭐야텨텨"
	--]
--	End()

	Action( "wait")
	--[
		Param( "time", 2 )
	--]
	End()

	Action( "point move" )
	--[
		Param( "loc x", 6051.83 )
		Param( "loc z", 4253.46 )
		Param( "run mode", 1)
	--]
	End()

	Action( "leave" )
	--[
	
	--]
	End()


--]
End()
