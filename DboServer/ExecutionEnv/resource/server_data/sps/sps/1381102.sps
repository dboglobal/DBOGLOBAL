-- DWC1
-- 1381102: 대사 부르마. 피격없고, LP바 없음.

-- DWC1 부르마 볼일보다 User 발견하고 이동
Scene( 1, "70001" )
--[
	-- npc ui 안나오도록 함
	Action( "char condition" )
	--[
		Param( "ui disable", 1 )
	--]
	End()
	
	-- User에게 뛰쳐나옴
	Action( "point move" )
	--[
		Param( "loc x", 1696.01 )
		Param( "loc y", -110.44 )
		Param( "loc z", -1499.46 )
		Param( "dir x", 0.8 )
		Param( "dir z", 0.6 )
		Param( "run mode", 1 )
	--]
	End()
	
	Action( "send event to wps" )
	--[
		Param( "wps index", 70001 )
		Param( "event id", 619 )
	--]
	End()
	
	Action( "sps end" )
	--[
	--]
	End()
--]
End()

