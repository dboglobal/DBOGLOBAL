------------------------------------------------------
--
-- 트랭크스
-- Author : YeoHyeonJoon (souled@ntl-inc.com)
--
------------------------------------------------------


Scene( 1, "TLQ1" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "ts")
		Param( "event id", 16210)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 891.02)
		Param( "loc z", -982.22)
		Param( "run mode", 1 )
    --]
    End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 2, "TMQ1" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 11175)
	--]
	End()
	
--트랭크스 퇴장을 위한 이동
	Action( "point move" )
    --[
		Param( "loc x", 639.89)
		Param( "loc z", -529.81)
		Param( "run mode", 1 )
    --]
	End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End() 

End()


------------------------------------------------------


Scene( 3, "TMQ1" )

-- 트랭크스 이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 11280)
	--]
	End()
	
--트랭크스 이동
	Action( "point move" )
    --[
		Param( "loc x", 709.80)
		Param( "loc z", -309.71)
		Param( "run mode", 1 )
    --]
	End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End() 

End()

------------------------------------------------------


Scene( 4, "TMQ 3 Trunks" )

--트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13070)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 874.40)
		Param( "loc z", 43.60)
		Param( "run mode", 1 )
    --]
    End()
	
--트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 5, "TMQ 3 Trunks" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13190)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 844.67)
		Param( "loc z", -422.55)
		Param( "run mode", 1 )
    --]
    End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 6, "TMQ 3 Trunks" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13240)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 832.78)
		Param( "loc z", -606.30)
		Param( "run mode", 1 )
    --]
    End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 7, "TMQ 3 Trunks" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13300)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 889)
		Param( "loc z", -662)
		Param( "run mode", 1 )
    --]
    End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 8, "TMQ 3 Trunks" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 13360)
	--]
	End()

	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 9, "TMQ 2 Trunks" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 12430)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 905.78)
		Param( "loc z", -467.01)
		Param( "run mode", 1 )
    --]
    End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 10, "TMQ 4 stage2" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14180)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 978.69)
		Param( "loc z", -879.99)
		Param( "run mode", 1 )
    --]
    End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 11, "TMQ 4 stage2" )

-- 트랭크스 퇴장 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14390)
	--]
	End()

--트랭크스 퇴장을 위한 이동
    Action( "point move" )
    --[
		Param( "loc x", 891.66)
		Param( "loc z", -542.26)
		Param( "run mode", 1 )
    --]
    End()
	
-- 트랭크스 퇴장 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()
	
	Action( "leave" )
	--[    
	--]
	End()  
	
End()


------------------------------------------------------


Scene( 12, "TMQ 4 stage4" )

-- 이동 명령까지 대기
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 14650)
	--]
	End()

--이동
    Action( "point move" )
    --[
		Param( "loc x", 426.90)
		Param( "loc z", -356.20)
		Param( "run mode", 1 )
    --]
    End()

	Action( "sps end" )
	--[
	--]
	End()

End()


------------------------------------------------------


Scene( 13, "TMQ 4 stage4" )

-- 쓰러진 연출
	Action( "direct play" )
	--[
		Param( "tblidx", 50051)
	--]     
	End()
	
End()


------------------------------------------------------


Scene( 14, "Tutorial" )

-- 트랭크스 User와 대화 끝날 때까지 대기 : 대화가 끝나면서 tqs로부터 1101102 넘어옴
	Action( "wait" )
	--[
		Param("event type", "tqs")
		Param( "event id", 1101102)
	--]
	End()

--트랭크스가 GBT-Hope까지 이동한다.
    Action( "point move" )
    --[
		Param( "loc x", -114.19)
		Param( "loc z", -97.80)
		Param( "run mode", 1 )
    --]
    End()

--트랭크스가 User를 바라보게 약간 이동
    Action( "point move" )
    --[
		Param( "loc x", -112.19)
		Param( "loc z", -99.80)
		Param( "run mode", 1 )
    --]
    End()

	Action( "sps end" )
	--[
	--]
	End()
	
End()


------------------------------------------------------


Scene( 15, "TMQ 2-2" )
--[
	Action( "wait" )
	--[
		Param( "event type", "tqs")
		Param( "event id", 22020 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 50044)
	--]     
	End()

	Action( "leave" )
	--[    
	
	--]
	End() 
--]
End()

---------------------------------------------------------

Scene( 16, "TMQ 2-2 02" )
--[
	Action( "wait" )
	--[
		Param( "event type", "tqs" )
		Param( "event id", 22340 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 50044 )
	--]
	End()

	Action( "leave" )
	--[

	--]
	End()
--]
End()

-----------------------------------------------------------

Scene( 17, "TMQ 2-2 03" )
--[
	Action( "wait" )
	--[
		Param( "event type", "tqs" )
		Param( "event id", 605 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 50044 )
	--]
	End()

	Action( "leave" )
	--[

	--]
	End()
--]
End()

-----------------------------------------------------------

Scene( 18, "TMQ 2-1 Stage3" )
--[
	Action( "wait" )
	--[
		Param( "event type", "ts" )
		Param( "event id", 598 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 50044 )
	--]
	End()

	Action( "send sps event" )
	--[
		Param( "event type", "tqs" )
		Param( "event id", 599 )
	--]
	End()

	Action( "sps end" )
	--[
	
	--]
	End()
--]
End()

-----------------------------------------------------------

Scene( 19, "TMQ 2-3 Stage1" )
--[
	Action( "wait" )
	--[
		Param( "event type", "tqs" )
		Param( "event id", 23020 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 50044 )
	--]
	End()

	Action( "leave" )
	--[

	--]
	End()
--]
End()

-----------------------------------------------------------

Scene( 20, "TMQ 2-3 Stage4" )
--[
	Action( "wait" )
	--[
		Param( "event type", "tqs" )
		Param( "event id", 23520 )
	--]
	End()

	Action( "direct play" )
	--[
		Param( "tblidx", 50044 )
	--]
	End()

	Action( "leave" )
	--[

	--]
	End()
--]
End()