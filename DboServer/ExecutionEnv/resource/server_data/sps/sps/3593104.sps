-- 1층
-- 앞쪽 컨베이어 벨트 위에서 무한 스폰
Scene( 1, "1" )
--[
	Action( "point move" )
	--[
		Param( "loc x", -231.067 )
		Param( "loc y", 0.634 )
		Param( "loc z", -403.934 )
		Param( "dir x", -1.000 )
		Param( "dir z", 0.029 )
		Param( "run mode", 0 )
	--]
	End()

	Action( "leave" )
	--[
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()

-- 뒤쪽 컨베이어 벨트 위에서 무한 스폰
Scene( 2, "2" )
--[
	Action( "point move" )
	--[
		Param( "loc x", -93.078 )
		Param( "loc y", 0.634 )
		Param( "loc z", -438.835 )
		Param( "dir x", 1.000 )
		Param( "dir z", 0.010 )
		Param( "run mode", 0 )
	--]
	End()

	Action( "leave" )
	--[
	--]
	End()

	Action( "sps end" )
	--[
	--]
	End()
--]
End()