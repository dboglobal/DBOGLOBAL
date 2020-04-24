-- DWC1
-- 우두머리 늑대의 대사

Scene( 1, "70003" )
--[
	-- 크르릉...
	Action( "say" )
	--[
		Param( "tblidx", 3054 )
	--]
	End()

	-- 3초 기다렸다가
	Action( "wait" )
	--[
		Param( "time", 3 )
	--]
	End()

	-- sps 종료
	Action( "sps end" )
	--[
	--]
	End()
--]
End()