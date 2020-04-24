CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6225;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 622221;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 5;
				osh_sh = 255;
				widx = 600000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 16;
				osh_sh = 255;
				widx = 600000;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 435;
				tblidx = 23005;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 2;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckObjState
			{
				mstate = 1;
				oidx = 5;
				osh = 255;
				widx = 600000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 622220;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;2;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "5;";
				widx = 600000;
			}
		}
	}
}

