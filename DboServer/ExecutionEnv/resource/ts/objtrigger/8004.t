CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 8004;
	title = 800402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 254;
			prelnk = "11;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 1000;
			}
			CDboTSActObjState
			{
				mstate = 2;
				oidx = 13;
				osh_sh = 255;
				widx = 200000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "10;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 1000;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 13;
				osh_sh = 255;
				widx = 200000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "12;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 11;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 1000;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 14;
				osh_sh = 255;
				widx = 200000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 10;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 800407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "14;";
				widx = 200000;
			}
		}
	}
}

