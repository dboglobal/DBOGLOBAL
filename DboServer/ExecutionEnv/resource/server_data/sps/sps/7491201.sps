-- 서쪽 도시에 돌아다니는 차량 NPC
-- 맵툴에서 실행시키는 sps파일임

Scene(1, "car")
--[
	Action( "char condition")
	--[
		Param( "ui disable", 1 )
	--]
	End()

	Action( "path move" )
	--[
		Param( "tblidx", 134 )
		Param( "run mode", 0 )
	--]
	End()
--]
End()