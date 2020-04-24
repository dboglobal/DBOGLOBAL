--- The SPS battery used in the past         ---
--- 05 May 13, 2011 Currently unused ---
Scene(1, "6004")
--[
	    -- 16. SPS를 받을 때까지 대기 
		Action( "wait" )
		--[
			Param("event type", "wps")
			Param( "event id", 600405)
		--]
		End()	
		

		 Action( "char condition" )
		 --[
				Param( "attack disallow", 1)   
		 --]
		 End()
		 
		 
		-- 17. 세뇌병사의 대화(흐흐 이곳이 수상한 곳의 위치다. 저 고양이를 납치하라. ).
--		Action( "say" )
		--[
--			  "흐흐 이곳이 수상한 곳의 위치다. "
			--Param( "tblidx", 1 )           
		--]     
--		End()

		Action( "wait" )
		--[
			   Param( "time", 2)
		--]
		End()

--     	Action( "say" )
		--[
--			  "저 고양이를 납치하라.
			--Param( "tblidx", 1 )           
		--]     
--		End()

		Action( "wait" )
		--[
			   Param( "time", 2)
		--]
		End()
		
		Action( "char condition" )
		 --[
				Param( "attack disallow", 0)   
		 --]
		 End()
		 
		 
		 
		-- 19. WPS로 EventId 보냄
		Action( "send sps event" )
		--[
				Param( "event type", "wps" ) 
				Param( "event id", 600406 )
		--]
		End()
--]
End()