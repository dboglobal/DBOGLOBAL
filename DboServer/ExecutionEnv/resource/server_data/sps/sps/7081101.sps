------------------------------------------------------
--
-- TMQ1-3 잠자는 공룡, TMQ1-4 미라 더미
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TMQ 3 잠자는 공룡" )

--잠자는 연출
	Action( "sitdown" )
	--[
		Param( "sitdown", "true")
	--]
	End()

	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13202)
	--]
	End()

--일어서
	Action( "sitdown" )
	--[
		Param( "sitdown", "false")
	--]
	End()	

--대기
	Action( "wait" )
	--[
		Param("time", 2 )
	--]
	End()
	
--일어섬 알림
	Action( "send sps event" )
	--[
		Param( "event type", "tqs" ) 
		Param( "event id", 13203 )
	--]
	End()
	
--더미대기
	Action( "wait" )
	--[
		Param("event type", "ts")
		Param( "event id", 11020)
	--]
	End()

End()
	
	
------------------------------------------------------


Scene( 2, "TMQ 3 잠깨는 공룡" )

    Action( "path move" )
    --[
        Param( "tblidx", 4)
        Param( "run mode", 1 )
    --]
    End()

	Action( "sps end" )
	--[
	--]
	End()
	
End()
------------------------------------------------------

Scene( 3,"TMQ 1-4 미라 더미" )

   Action( "direct play" )
   --[
	Param( "tblidx", 1046)
   --]
   End()

End()

------------------------------------------------------