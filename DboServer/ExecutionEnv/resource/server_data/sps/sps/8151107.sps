-- DWC1
-- 뿔나로돈의 대사

Scene( 1, "70004" )
--[
	-- 흐흐…. 이번에는 그냥 보내지 않겠다…. 흐흐….
	Action( "say" )
	--[
		Param( "tblidx", 3055 )
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