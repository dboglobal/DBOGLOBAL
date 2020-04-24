CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	rq = 0;
	tid = 11030;
	title = 4539;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActSendSvrEvt
			{
				id = -1;
				tblidx = 1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 4;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;0;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActObjState
			{
				mstate = 3;
				oidx = 3;
				osh_sh = 255;
				widx = 10000;
				osh_uc = 255;
				taid = 1;
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
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 3;
				osh_sh = 1;
				widx = 10000;
				osh_uc = 255;
				taid = 1;
			}
		}
	}
}

