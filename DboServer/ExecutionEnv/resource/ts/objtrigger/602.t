CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 602;
	title = 84302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 5;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 2;
				oidx = 3;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 1;
			prelnk = "12;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 12;
			prelnk = "11;";
			nolnk = 254;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 254;
			prelnk = "7;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 5;
				osh_sh = 1;
				widx = 3;
				osh_uc = 0;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 0;
				oidx = 3;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
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
				mstate = 3;
				oidx = 5;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 3;
				oidx = 3;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 84307;
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSCheckInWorld
			{
				widx = 3;
			}
			CDboTSClickObject
			{
				objidx = "5;";
				widx = 3;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 5;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 3;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 60000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "12;8;";
			type = 1;
		}
	}
}

