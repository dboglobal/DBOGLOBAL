-- DWC1
-- 손오공 부르마 루트에서 spawn되어 이동함

Scene( 1, "70001" )
--[
	-- npc ui 안나오도록 함
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()

	-- 어디 먹을만한 것이 안보이네...
	Action( "say" )
	--[
		Param( "tblidx", 3323 )
	--]
	End()

	-- 손오공이 user 근처로 이동
	Action( "point move" )
	--[
		Param( "loc x", 1603.29 )
		Param( "loc y", -84.32 )
		Param( "loc z", -1694.83 )
		Param( "dir x", 0.00 )
		Param( "dir z", 1.00 )
		Param( "run mode", 1 )
	--]
	End()

	-- 손오공 이동 완료 알려줌
	Action( "send event to wps" )
	--[
		Param( "wps index", 70003 )
		Param( "event id", 637 )
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()