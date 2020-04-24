
Scene(1, "2553" )
--[
        Action( "char condition" )
        --[
                Param( "ui disable", 1 )
        --]
        End()

	-- 해당 지점에 도착하자마자 주루루룩 미끄러짐미다
	-- y에 넣은 값은 더미값이므로 다시 조사해서 수정해주십띠오
        Action( "point move" )
        --[
                Param( "loc x", -2395.759 )
                Param( "loc y", -140.957 )
                Param( "loc z", -4509.709 )
                Param( "run mode", 1 )
        --]
        End()
        
        Action( "target fix attack" )
        --[
                Param( "obj type", "npc" )
                Param( "bot tblidx", 3213107 )
                Param( "scan range", 50 )
                Param( "end type", "percent target lp" )
                Param( "end value", 0 )
        --]
        End()
        
        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()