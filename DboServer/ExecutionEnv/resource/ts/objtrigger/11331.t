CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11331;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 242;
			elnk = 255;
			nextlnk = 7;
			prelnk = "241;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 1;
				osh_sh = 255;
				widx = 193000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 242;
			prelnk = "240;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 1;
				osh_sh = 255;
				widx = 193000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 240;
			prelnk = "2;";
			nolnk = 5;
			rm = 0;
			yeslnk = 241;

			CDboTSCheckInWorld
			{
				widx = 193000;
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
				oidx = 1;
				osh_sh = 255;
				widx = 100000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "240;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 1;
				osh_sh = 255;
				widx = 100000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 240;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckInWorld
			{
				widx = 80000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "242;6;4;";

			CDboTSActSendSvrEvt
			{
				id = -1;
				tblidx = 7;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 3;
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
				oidx = 1;
				osh_sh = 255;
				widx = 80000;
				osh_uc = 255;
				taid = 1;
			}
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
				oidx = 1;
				osh_sh = 255;
				widx = 80000;
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

