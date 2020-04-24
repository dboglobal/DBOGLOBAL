
Scene(1, "2523" )
--[
        Action( "wait" )
        --[
                Condition( "recv event from item" )
                --[
                        Param( "item index", 98022 )
                --]
                End()
        --]
        End()

        Action( "say" )
        --[
                Param( "tblidx", 3287 )
        --]
        End()

        Action( "send event to wps" )
        --[
                Param( "wps index", 2523 )
                Param( "event id", 565 )
        --]
        End()

        Action( "sps end" )
        --[
        
        --]
        End()
--]
End()
