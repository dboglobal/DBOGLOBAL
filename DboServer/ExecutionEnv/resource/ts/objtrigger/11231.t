CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11231;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 239;
			prelnk = "2;";
			nolnk = 5;
			rm = 0;
			yeslnk = 240;

			CDboTSCheckInWorld
			{
				widx = 192000;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 7;
			prelnk = "240;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 23;
				osh_sh = 255;
				widx = 192000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 240;
			elnk = 255;
			nextlnk = 241;
			prelnk = "239;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 23;
				osh_sh = 255;
				widx = 192000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "241;6;4;";

			CDboTSActSendSvrEvt
			{
				id = -1;
				tblidx = 5;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 3;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 23;
				osh_sh = 255;
				widx = 70000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "239;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 23;
				osh_sh = 255;
				widx = 70000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 239;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckInWorld
			{
				widx = 28000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 23;
				osh_sh = 255;
				widx = 28000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;0;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 7;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 23;
				osh_sh = 255;
				widx = 28000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
	}
}

